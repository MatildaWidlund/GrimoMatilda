#include "list.h"
#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

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

/* hjälpfunktioner */
static
elem_t list_no_copy(elem_t elem)
{
  return elem;
}

static

int convert_index(int index, int length)
{
  if (index <0 && length == 0)
    {
      return 0;
    }
  else if (index <0)
    {
      return length + index;
    }
  else if (index > length)
    {
      return length;
    }
  return index;
}
  
  /*
  if (index <0)
    
    {return convert_index(index + length, length);
    }
  if (index > length)
    {return length;
    }
  return index;
}
*/

static
link_t *link_new(elem_t elem, link_t *next)
{
  link_t *link = calloc(1, sizeof(link_t)); // (*link)?
  if (link)
    {
      *link = (link_t) {.value = elem, .next = next};
    }
  return link;
}
 
static // vandrar i listan och cursorn pekar på nästa adress
link_t **list_find(list_t *list, int index)
{ link_t **cursor = &(list->first);
  int converted_index =
    (0 <= index && index < list->length) ? index : convert_index(index, list->length);

  for (int i = 0; i < converted_index && *cursor; ++i)
    {
      cursor = &(*cursor)->next;
    }
  return cursor;
}

/**/

list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *list = calloc(1, sizeof(list_t));
  if (list)
    { *list = (list_t){
        .copy = copy ? copy : list_no_copy,
        .free = free,
        .compare = compare,
        .length = 0
      };
    }
  return list;
}



void list_insert(list_t *list, int index, elem_t elem)
{
  int converted_index =
    (0<= index && index < list->length) ? index : convert_index(index, list->length +1);

  link_t **cursor = list_find(list, converted_index);
  if (*cursor || converted_index == list->length)
    {
      *cursor =link_new(list->copy(elem), *cursor);
      ++list->length;
    }
}

void list_append(list_t *list, elem_t elem)
{
  list_insert(list, -1, elem);
}

void list_prepend(list_t *list, elem_t elem)
{
  list_insert(list, 0, elem);
}

void list_remove(list_t* list, int index, bool delete)
{
  if (list == NULL)
    {
      return;
    }
  
  int converted_index =
    (0 <= index && index < list->length) ? index : convert_index(index, list->length);

  link_t **cursor = list_find(list, converted_index);
  
  if (*cursor)
    {
      link_t * link_to_remove = *cursor;
      *cursor=(*cursor)->next;
      if (delete)
        {
          list->free(link_to_remove->value);
        }
      free(link_to_remove);
      --list->length;
  }
}

bool list_get(list_t *list, int index, elem_t *result)
{

  int converted_index = (0<= index && index < list->length) ? index : convert_index(index, list->length);
  link_t **cursor = list_find(list, converted_index);
  
  if (*cursor)
    { *result = (*cursor)->value; //? & (*cursor)->value : NULL;
      return true;
    }
  return false;
  
}

bool list_first(list_t *list, elem_t *result)
{
  if(list_get(list,0, result))
    {
      return true;
    }
    return false;
}

bool list_last(list_t *list, elem_t *result)
{
  if (list_get(list, list->length-1, result))
    {
      return true;
    }
  return false;
}

int list_length(list_t *list)
{
  return list->length;
}

void list_delete(list_t *list, bool delete)
{
  while (list_length(list) > 0)
    {
      list_remove(list, 0, delete);
    }
  assert(list->length ==0);
  free(list);
}

bool list_apply(list_t *list, elem_apply_fun fun, void *data)
{if (list == NULL) return false;
  link_t *item = list->first;
  while (item != NULL)
  {
    fun(item->value, data);
    item = item->next;
    return true;
  }
  return false;
 }

int list_contains(list_t *list, elem_t elem)
{
  link_t **cursor = &(list->first);
  if(list->compare != NULL)
    {
      for (int index =0; index <list->length && *cursor; ++index)
        {
          if (list->compare((*cursor)->value, elem) != 0)
            {
              cursor = &(*cursor)->next;
            }
          else
            {
              return index;
            }
        }
    }
  else
    {

      for (int index =0; index <list->length && *cursor; ++index)
        {
          if((*cursor)->value.i == elem.i)
            {
              return index;
            }
          else
            {
              cursor = &(*cursor)->next;
            }
        }}
  return -1;
  
}


/*
int list_contains(list_t *list, elem_t elem)
{
  
  if (list->compare == NULL)
    { if (elem.i == list->first->value.i)
        {
          // elem_index(list, elem); 
          return 1; // fixa en hjälpfunktion
        }
      else return -1;
    }
  else { 
    list->compare(elem, list->first->value);
    return 1;
   }
  
}

int _elem_index(list_t *list, elem_t elem)

{return 0;
}
//vill vandra i listan
//ha ett element & jämföra med element
//hämta dess index






    
//int _list_contains()

int _list_contains()


  
  link_t *item = list->first;
  while (item != NULL)
    {
    if (delete)
      {
        (item->value);
      }
    link_t *next = item->next;
    free(item);
    item = next;
    }
  return;
}



  if (list != NULL)
    {
      *list = (list_t){
        .compare = elem };
    }

 link_t *item = (list->first);
  if (elem == .item->value)
  
  {return index;}
  
  else 
    {
      return -1;
  }
}

int _list_contains()
  */
