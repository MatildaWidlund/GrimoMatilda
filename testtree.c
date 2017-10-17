#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "tree.h"

elem_t element_copy(elem_t elem)
{
 return elem;
}


elem_t string_copy(elem_t elem)
{
  char * original = (char*) elem.p;
  void *duplicate = strdup(original);
  return (elem_t) duplicate;
  
}

void free_key (elem_t key)
{
  return;
}

void free_elem(elem_t elem)
{
  return;
}

int str_compare(elem_t a, elem_t b)
{
  if (strcmp((char*)a.p, (char*)b.p) >0)
    {
      return 2 ;
    }
  else if (strcmp((char*)a.p, (char*)b.p) < 0)
      {
      return 1;
    }
  else return 0; 
}

int int_compare(elem_t a, elem_t b)
{
  if (a.i == b.i)
    {return 0;}
  else if  (a.i > b.i)
    {return 1;}
  else 
    {return -1;} 
}
  




//för att göra delete valid
/*
bool delete_keys (tree_key_t *key)
{
if (key != NULL) {
 free(key);
 return true;
 }
 else return false;
}

bool delete_elements(elem_t *elem)
{
if (elem != NULL) {
 free(elem);
 return true;
 }
 else return false;
}
*/


//bool delete_elements = true;

int main (void)
{
  
  tree_t * tree = tree_new(NULL,free_key, free_elem, int_compare);
  elem_t *result = NULL;
  elem_t elem1 = {.i=1};
  elem_t elem2 = {.i=2};
  elem_t elem3 = {.i=3};
  elem_t elem4 = {.i=4};
  elem_t elem5 = {.i=5};
  tree_key_t key1 = {.i=6};
  tree_key_t key7 = {.i=7};
  tree_key_t key8 = {.i=8};
  tree_key_t key9 = {.i=9};
  tree_key_t key10 = {.i=10};
  printf("%d\n",tree_size(tree));
  tree_insert(tree, key1, elem1);
  
  printf("%d\n",tree_size(tree));      
  printf("%s %d\n","djuppet på träd", tree_depth(tree)); 
  tree_insert(tree, key7, elem2);
  printf("%d\n",tree_size(tree));
  printf("%s %d\n","djuppet på träd", tree_depth(tree)); 
  tree_insert(tree, key8, elem3);
  printf("%d\n",tree_size(tree));
  printf("%s %d\n","djuppet på träd", tree_depth(tree)); 
  tree_insert(tree, key9, elem4);
  printf("%d\n",tree_size(tree));
  printf("%s %d\n","djuppet på träd", tree_depth(tree));
  tree_insert(tree, key10, elem5);
  printf("%d\n",tree_size(tree));
  printf("%s %d\n","djuppet på träd", tree_depth(tree));
 printf("%s %d\n", "size före delete" , tree_size(tree));
 tree_has_key(tree, key10);
 tree_get(tree, key10, result);
 
 tree_remove(tree, key10, result);
 printf("%s\n","borttagen key 10" );
 tree_has_key(tree, key10);
 tree_get(tree, key10, result);
 
 tree_delete(tree, true, true);
  printf("%s %d\n", "size efter delete" , tree_size(tree));
  
  return 0;
}
