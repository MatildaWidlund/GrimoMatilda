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

void key_free (key_free_fun key)
{
  if (key == NULL) {
    return;
  }
  else {
    free(key);
  }
  return;
}

void elem_free(elem_t *value)
{
  if (value == NULL ) {
    return;
  }
  else {
    free(value);  
  }
  return;
}

int compare(elem_t a, elem_t b)
{
  if (strcmp(a.p, b.p) >0)
    {
      return 2 ;
    }
  else if (strcmp(a.p, b.p) < 0)
      {
      return 1;
    }
  else return 0; 
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
  
  tree_t * tree = tree_new(NULL, NULL, NULL, compare);

  elem_t elem1 = {.p="a"};
  elem_t elem2 = {.p="b"};
  elem_t elem3 = {.p="c"};
  elem_t elem4 = {.p="d"};
  elem_t elem5 = {.p="e"};
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

  // tree_delete(tree, true, true);
  //printf("%s %d\n", "size efter delete" , tree_size(tree));
  return 0;
}
