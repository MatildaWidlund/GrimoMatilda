#include <stdbool.h>
#include <stdlib.h>

#include "list.h"

// achievement M37

typedef struct list_item {
  L data;
  struct list_item* next;
} list_item_t;

typedef struct list {
  struct list_item* first;
  struct list_item* last;
  int length;
} list_t;

/* INTERNAL FUNCTIONS */

int _length_rec(list_item_t* item, int acc) {
  // achievement F14
  if (item == NULL) return acc;
  else return _length_rec(item->next, acc + 1);
}

int _length_it(list_item_t* item) {
  // achievement F13
  int sum = 0;
  while (item != NULL) {
    ++sum;
    item = item->next;
  }
  return sum;
}

/* INTERFACE FUNCTIONS */

list_t* list_new() {
  list_t* list = calloc(1, sizeof(list_t));

  if (list != NULL) {
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
  }

  return list;
}

void list_append(list_t* list, L elem) {
  list_insert(list, list->length, elem);
}

void list_prepend (list_t* list, L elem) {
  list_insert(list, 0, elem);
}

bool list_insert(list_t* list, int index, L elem) {
  if (list == NULL) return false;

  if (index < 0) {
    // convert negative indices to positive (without wrap-around)
    index = list->length + 1 + index;
  }

  if (index < 0 || index > list->length) {
    // invalid index
    return false;
  }

  list_item_t* item = calloc(1, sizeof(list_item_t));
  
  if (item == NULL) {
    // allocation failed
    return false;
  }

  item->data = elem;

  if (list->length == 0) {
    // special case for an empty list
    list->first = item;
    list->last = item;
    item->next = NULL;
  } else if (index == list->length) {
    // special case for inserting at the end in constant time
    list->last->next = item;
    list->last = item;
    item->next = NULL;
  } else {
    // step through list until we reach the index at which we want to insert
    list_item_t* prev = list->first;
    --index;

    while (index > 0 && prev->next != NULL) {
      prev = prev->next;
      --index;
    }

    if (index != 0) {
      // sanity check: index should always be 0 here
      free(item);
      return false;
    }

    // insert element
    item->next = prev->next;
    prev->next = item;
  }

  ++(list->length);

  return true;
}

bool list_remove(list_t* list, int index, L* elem) {
  // achievement M39
  if (list == NULL) return false;

  if (index < 0) {
    // convert negative indices to positive (without wrap-around)
    index = list->length + 1 + index;
  }

  if (index < 0 || index >= list->length) {
    // invalid index
    return false;
  }

  list_item_t** dptr = &(list->first);

  while (index > 0 && (*dptr)->next != NULL) {
    dptr = &((*dptr)->next);
    --index;
  }
  
  if (index != 0) {
    // sanity check: index should always be 0 at this point
    return false;
  }

  list_item_t* item = *dptr;
  *dptr = (*dptr)->next;
  *elem = item->data;
  free(item);

  return true;
}

L* list_get(list_t* list, int index) {
  if (list == NULL) return NULL;
  
  if (index < 0) {
    // convert negative indices to positive (without wrap-around)
    index = list->length + 1 + index;
  }

  if (index < 0 || index >= list->length) {
    // invalid index
    return false;
  }
  
  if (index == list->length - 1) {
    return &(list->last->data);
  } else {
    list_item_t* item = list->first;
    //--index;

    while (index > 0 && item->next != NULL) {
      item = item->next;
      --index;
    }

    if (index != 0) {
      return NULL;
    }

    return &(item->data);
  }
}

L* list_first(list_t* list) {
  return list_get(list, 0);
}

L* list_last(list_t* list) {
  return list_get(list, list->length - 1);
}

int list_length(list_t* list) {
  return list->length;
}

void list_delete(list_t* list, list_action cleanup) {
  list_item_t* item = list->first;
  
  while (item != NULL) {
    if (cleanup != NULL) {
      cleanup(item->data);
    }

    list_item_t* next = item->next;
    free(item);
    item = next;
  }

  return;
}

void list_apply(list_t* list, list_action2 fun, void* data) {
  if (list == NULL) return;

  list_item_t* item = list->first;

  while (item != NULL) {
    fun(item->data, data);
    item = item->next;
  }
}
