#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "tree.h"

extern char* strdup(const char*); //behålla denna?


//_____________
// typdefs
//_____________



typedef struct node
{
  struct tree *tree; 
  elem_t elem;
  tree_key_t key;
  struct node* left;
  struct node* right;
  
} node_t;

typedef struct tree {
  struct node* root;
  int size;
  element_copy_fun copy;
  key_free_fun free_key;
  element_free_fun free_elem;
  element_comp_fun comp;
} tree_t;

typedef bool(*traverse_func)(node_t *node, void *p);
#define UNUSED(x) (void)(x)

/* INTERNAL FUNCTIONS */
// - - - - - - 
//TODO: ADDERA IN INTERNAL FUNCTIONS NÄR DET BEHÖVS...
// - - - - - -

int _depth_children(node_t* node) {
  int leftdepth = 0;
  int rightdepth = 0;

  if (node -> left != NULL) {
    leftdepth = _depth_children(node-> left) + 1;
  }

  if (node -> right != 0) {
    rightdepth = _depth_children(node -> right) + 1;
  }

  return (leftdepth > rightdepth) ? leftdepth : rightdepth;
}

void tree_fun_aux(bool *result, node_t *node, traverse_func fun, void *data)
{
  if (node == NULL)
    {
      return;
    }
  tree_fun_aux (result, node->left, fun, data );
  tree_fun_aux (result, node->right, fun, data );
  if (fun(node, data))
    {
      *result = true;
    };
  
}

bool trav_fun (tree_t *tree, traverse_func fun, void *data)
{
  bool result = false;
  if (tree->root != NULL)
    {
      tree_fun_aux(&result, tree->root, fun, data);
      return result;
    }
  return result;
  
}

void _node_delete(node_t* node, key_free_fun free_key) {//har jag tänkt rätt att key_free_fun kan användas för att freea hela noden?
  if (node == NULL) return;

  if (node->left != NULL) {
    _node_delete(node->left, free_key);
    free(node->left);
  }

  if (node->right != NULL) {
    _node_delete(node->right, free_key);
    free(node->right);
  }
}

bool _free_elem (node_t *node, void *p)
{
  UNUSED(p);
  
  (node)->tree->free_elem((node)->elem);
  return true;
}

bool _free_key (node_t *node , void *p)
{
  UNUSED(p);
  node->tree->free_key(node->key);
  return true;
}


node_t *_new_node(elem_t elem, tree_key_t key)
{
  node_t *n = calloc(1, sizeof(node_t));
  
  n->elem = elem;
  n->key = key;
 
  return n;
  
}


int _count_children(node_t* node) {
  int sum = 0;
  
  if (node->left != NULL) {
    sum += (_count_children(node->left) + 1);
  }

  if (node->right != NULL) {
    sum += (_count_children(node->right) + 1);
  }

  return sum;
}

bool tree_insert_aux(tree_t *tree, node_t **node, tree_key_t key, elem_t elem)
{
  
  if (tree->comp == NULL)
    {
      if (*node == NULL)
        {
          printf("%s\n","tree tomt");
          *node = _new_node (key, elem);
          
          return true;   
        }
      if ((*node)->elem.i == elem.i)
        {
          printf("%s\n","tree samma som input");
          return false;
        }
      else if ((*node)->elem.i > elem.i)
        {
          printf("%s\n","tree mindre än input");
          
          return tree_insert_aux(tree,&(*node)->left, key, elem); 
        }
      else if ((*node)->elem.i < elem.i)
        {
          printf("%s\n","tree större än input");
          
          return tree_insert_aux(tree,&(*node)->right, key, elem); 
        }
      
    }

  if (tree->comp != NULL) {
    if(*node == NULL)
      {
        if (tree->copy != NULL)
          {
            elem_t copy_v = tree->copy(elem);
            (*node) = _new_node(copy_v, key); 
          }
        else
          {
          *node = _new_node(elem, key);
          return true;   
          }
      }
    if (tree->comp((*node)->elem, elem) == 1)
      {
        
        tree_insert_aux(tree, &(*node)->left, key, elem);
      }
    else if(tree->comp((*node)->elem, elem) == 2)
      {
        tree_insert_aux(tree, &(*node)->right, key, elem);    
      }
    else if(tree->comp((*node)->elem, elem) == 0)
      {
        return false;
      }
  }
  return false;
}  



//EXTERNAL FUNCTIONS


tree_t* tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare)
{
  tree_t *tree = calloc(1, sizeof(tree_t));

  
  if (element_copy) {
    tree->copy = element_copy;
  }
  if (key_free) {
    tree->free_key = key_free;
  }
  if (elem_free) {
    tree->free_elem = elem_free;
  }
  if (compare) {
    tree->comp = compare;
  }

  return tree;
}


void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  
  if (delete_keys == true)
    {
      trav_fun(tree, _free_elem , NULL);  
    }
  if (delete_elements == true)
    {
      trav_fun(tree, _free_key, NULL);
    }
  free(tree);
     
}



int tree_size(tree_t *tree)
{
  if (tree == NULL || tree->root == NULL)
    {
      return 0;
    }

  return (1 + _count_children(tree->root));
}


int tree_depth(tree_t* tree) {
  if (tree == NULL || tree->root == NULL) return 0;

  return (1 + _depth_children(tree->root));
}


bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  return tree_insert_aux(tree, &tree->root, key, elem);
}
