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

void key_free (elem_t key)
{
  return;
}

void elem_free(elem_t elem)
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
    {return -1;}
  else 
    {return 1;} 
}
  


int main (void)
{  
  tree_t * tree = tree_new(element_copy, key_free, elem_free, int_compare);
  //elem_t result ={.i=1};
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
  
  //printf("%d\n",tree_size(tree));
  tree_insert(tree, key1, elem1);
  print_tree(tree);
  printf("%s, %d\n","tree size:" ,tree_size(tree));      
  //printf("%s %d\n","djuppet på träd", tree_depth(tree)); 
  tree_insert(tree, key7, elem2);
  //printf("%d\n",tree_size(tree));
  //printf("%s %d\n","djuppet på träd", tree_depth(tree)); 
  tree_insert(tree, key8, elem3);
  //printf("%d\n",tree_size(tree));
  //printf("%s %d\n","djuppet på träd", tree_depth(tree)); 
  tree_insert(tree, key9, elem4);
  //printf("%d\n",tree_size(tree));
  // printf("%s %d\n","djuppet på träd", tree_depth(tree));
  tree_insert(tree, key10, elem5);
  print_tree(tree);
  printf("%s\n","storlek på träd efter insert:" );
  printf("%s, %d\n","tree size:" ,tree_size(tree)); 
  print_tree(tree);
  printf("%s\n","balannce" );
  tree_balance(tree);
  print_tree(tree);

  printf("%s\n","tree size" );
  printf("%d\n",tree_size(tree));
   printf("%s\n","tree depth" );
  printf("%d\n",tree_depth(tree) );
  printf("%s\n","alla keys:" );
  //tree_keys(tree);
  printf("%s\n","alla elem:" );
  // tree_elements(tree);
  elem_t test;
  tree_remove(tree,key7, &test);

  //tree_balance(tree);

  //print_tree(tree);
 
  //tree_has_key(tree, key7);
  // printf("%s\n","tree delete" ); 
  //tree_delete(tree, true, true);
  //printf("%s\n","tree size efter delete" );
  //tree_size(tree);
  
  
  return 0;
}
