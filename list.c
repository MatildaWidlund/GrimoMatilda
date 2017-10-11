#include "list.h"
#include "common.h"
#include <stdbool.h>
#include <stdlib.h>


typedef union element elem_t;
typedef struct link{
    elem_t value;
    struct link* next;
}link_t;
typedef struct list{
    struct link* first;
    struct link* last;
    int length;
    element_copy_fun copy;
    element_free_fun free;
    element_comp_fun compare;
}list_t;
typedef int(*cmp_t)(elem_t, elem_t);

list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *list = calloc(1, sizeof(list_t));
    if (list)
    { *list = (list_t){
        .copy = copy ? copy : NULL,
        .free = free,
        .compare = compare};
    }
    return list;
}

void list_insert(list_t *list, int index, elem_t elem)
{ if(list == NULL) return;
  if (index  < 0)
    {index = list->length +1 + index;
    }
  if (index <0 || index > list -> length)
    {
      return;
    }
  link_t *elem2 = calloc(1, sizeof(link_t));
  if (elem2 == NULL)
    {
      return;
    }
  elem2->value = elem;
  if (list->length ==0)
    {
      list->first = elem2;
      list->last = elem2;
      elem2->next = NULL;
    }
  else if (index == list->length)
    {
      list->last->next = elem2;
      list->last = elem2;
      elem2->next = NULL;
    }
  else
    {
      link_t* prev = list->first;
      --index;
      while (index > 0 && prev->next != NULL)
        {
          prev = prev->next;
          --index;
        }
      if (index != 0){
        free(elem2);
        return;
      }
      elem2->next = prev->next;
      prev->next = elem2;
    }
  ++(list->length);
  return;
}
