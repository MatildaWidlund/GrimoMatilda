#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "tree.h"
#include <assert.h>


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

node_t *get_node_aux(tree_t *tree, node_t *cursor, tree_key_t key)
{
  cursor = tree->root;
   if (tree->comp(cursor->key, key)  == 0)
      {
        return cursor;
      }

   while (tree->comp(cursor->key, key)  != 0)
     {
       if (tree->comp(cursor->key, key) > 0)
      {
        get_node_aux(tree, cursor->right, key);
      }
       else if (tree->comp(cursor->key, key) < 0) {
      get_node_aux(tree, cursor->left, key);
    }
  }
 return cursor;
}


node_t *get_node (tree_t *tree ,node_t *cursor,  tree_key_t key)
{
  return get_node_aux(tree, cursor, key);

}
 
  



int _depth_children(node_t* node) {
  int leftdepth = 0;
  int rightdepth = 0;

  if (node -> left != NULL) {
    leftdepth = (_depth_children(node-> left) + 1);
  }

  if (node -> right != 0) {
    rightdepth = (_depth_children(node -> right) + 1);
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

node_t *elem_get (tree_t *tree, node_t *node, elem_t elem)
{
  if (tree->comp(node->elem, elem) > 0)
    {
      return elem_get(tree, node->left, elem);
    
  }
  if (tree->comp(node->elem, elem) < 0)
    {
      return elem_get(tree, node->right, elem); 
  }
   else return node;
}

void deleter (tree_t *tree, node_t *node, bool delete_keys, bool delete_elements)
{
if (delete_keys) {
  tree->free_key(node->key);
 }
if (delete_elements) {
  tree->free_elem(node->elem);
 }
 printf("%s\n","deleter, node" );
 free(node);
}

void  _tree_delete_aux(tree_t *tree, node_t *node, bool delete_keys, bool delete_elements)
{
  if (node == NULL)
    {
      return;
    }
 else
    {
      
      _tree_delete_aux(tree,node->right, delete_keys, delete_elements);
      _tree_delete_aux(tree,node->left, delete_keys, delete_elements);
      deleter(tree, node, delete_keys, delete_elements);
    }
  
  
}



node_t *_new_node( tree_key_t key,elem_t elem)
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
          
          return tree_insert_aux(tree,&(*node)->right, key, elem); 
        }
      else if ((*node)->elem.i < elem.i)
        {
          printf("%s\n","tree större än input");
          
          return tree_insert_aux(tree,&(*node)->left, key, elem); 
        }
      
    }

  if (tree->comp != NULL) {
    if(*node == NULL)
      {
        if (tree->copy != NULL)
          {
            elem_t copy_v = tree->copy(elem);
            (*node) = _new_node(key, copy_v); 
          }
        else
          {
          *node = _new_node(key, elem);
          return true;   
          }
      }
    if (tree->comp((*node)->elem, elem) == -1)
      {
        
        tree_insert_aux(tree, &(*node)->right, key, elem);
      }
    else if(tree->comp((*node)->elem, elem) == 1)
      {
        tree_insert_aux(tree, &(*node)->left, key, elem);    
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

bool tree_has_key(tree_t *tree, tree_key_t key)
{
  if (tree == NULL || tree->root == NULL)
    {
      printf("%s\n","tree is empty" );
      return false;
    }
  node_t *node = tree->root;
  while (node != NULL)
    {
      int result = tree->comp(node->key, key);
      if (result > 0)
        {
          printf("%s\n","node bigger" );
          node = node->right;
        }
      else if (result < 0)
        {
          printf("%s\n","node smaller" );
          node = node->left;
        }
      else {
        printf("%s\n","key exists" );
        return true;
      }   
    }
  printf("%s\n","node doesnt exist" );
  return false;
}


bool tree_get(tree_t *tree, tree_key_t key, elem_t *result)
{
  result = NULL;
  
  if (tree == NULL || tree->root == NULL)
    {
      printf("%s\n","tree is empty" );
      return false;
    }
  node_t *node = tree->root;
  while (node != NULL)
    {
      int res = tree->comp(node->key, key);
      if (res > 0)
        {
          printf("%s\n","node bigger" );
          node = node->right;
        }
      else if (res < 0)
        {
          printf("%s\n","node smaller" );
          node = node->left;
        }
      else {
        printf("%s\n","key exists" );
        result = &node->elem;
        printf("%d\n",(result->i));
        return true;
      }   
    }
  printf("%s\n","node doesnt exist" );
  return false;
}


void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  _tree_delete_aux(tree,tree->root, delete_keys, delete_elements);
  free(tree);
}


/// TREE REMOVE TODO!!!!
node_t *remove_node_aux(node_t *node)
{
<<<<<<< HEAD
  node_t *cursor = node;
  do
    {
      cursor = cursor->left;
    }
  while(cursor->left != NULL);
  return cursor;
=======

  return;

  
>>>>>>> db180b4bd7acdbf2ea2a54f0eb357cc49296b8b9
}

 
bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result)
{
<<<<<<< HEAD
  node_t *cursor = NULL;
  node_t *node = get_node(tree,cursor, key);
  node_t *removenode = node;
  if(node == NULL) return false;
  if(node->left == NULL && node->right == NULL)
=======

  return false;
=======
    if(tree_get(tree, key, result))
        return true;
    else
>>>>>>> db180b4bd7acdbf2ea2a54f0eb357cc49296b8b9
    {
      *result = node->elem;
      free(removenode);
      node = NULL;
      return true;
    }
<<<<<<< HEAD
  else if(node->right == NULL)
    {
      *result = node->elem;
      node = node->left;
      free(removenode);
      return true;
    }
  else if(node->left == NULL)
    {
      *result = node->elem;
      node = node->right;
      free(removenode);
      return true;
    }
  else
    {
      node_t *tmp = remove_node_aux(node->right);
      *result = node->elem;
      tmp->left =node->left;
      node = tmp;
      free(removenode);
    }
  return false;
=======

>>>>>>> db180b4bd7acdbf2ea2a54f0eb357cc49296b8b9
}
 



int print_tree (tree_t *tree, tree_key_t key)
{
  node_t *node = tree->root;

  if (node->key.i != 0)
    {
      return
        printf("%s\n", "empty");
        
      
  }
  while (node->key.i != 0)
    {
     printf("%d\n", key.i );
     print_tree(tree, node->left->key);
     print_tree(tree, node->right->key);
  }
  return 0;
}


//BALANSERING AV TRÄD TODO!!!!

 










