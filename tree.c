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


node_t **get_node(tree_t *tree ,node_t **cursor,  tree_key_t key)
{
  if (*cursor == NULL) return NULL;

  int cmp = tree->comp((*cursor)->key, key);

  if (cmp == 0)
    {
      return cursor;
    }

  if (cmp < 0)
    {
      return get_node(tree, &(*cursor)->left, key);
    }
  else
    {
      return get_node(tree, &(*cursor)->right, key);
    }
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


node_t *_new_node( tree_key_t key,elem_t elem)
{
  node_t *n = (node_t*)calloc(1, sizeof(node_t));

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

  if (tree->comp != NULL) {
    if(*node == NULL)
      {
        if (tree->copy != NULL)
          {
            // TODO
            //elem_t copy_v = tree->copy(elem);
            (*node) = _new_node(key, elem);
          }
        else
          {
            *node = _new_node(key, elem);
            return true;
          }
      }
    if (tree->comp((*node)->elem, elem) < 0)
      {

        tree_insert_aux(tree, &(*node)->left, key, elem);
      }
    else if(tree->comp((*node)->elem, elem) < 0)
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

int int_cmp(elem_t a, elem_t b)
{
  return a.i - b.i;
}


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
  } else {
    tree->comp = int_cmp;
  }

  return tree;
}

void tree_delete_helper(node_t *node, bool delete_keys, bool delete_elements, key_free_fun free_key, element_free_fun free_elem)
{
  if (node == NULL)
    {
      return;
    }
  else
    {
      tree_delete_helper(node->left, delete_keys, delete_elements, free_key, free_elem);
      tree_delete_helper(node->right, delete_keys, delete_elements, free_key, free_elem);

      if (delete_elements)
        {
         free_elem(node->elem);
        }
      if (delete_keys)
        {
          free_key(node->key);
        }
      free(node);
    }
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  // remove tree & free up memory
  tree_delete_helper(tree->root, delete_keys, delete_elements, tree->free_key, tree->free_elem);
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


int tree_depth(tree_t* tree)
{
  if (tree == NULL || tree->root == NULL) return 0;

  return (1 + _depth_children(tree->root));
}


bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  return tree_insert_aux(tree, &tree->root, key, elem);
}


int tree_has_key_aux (tree_t *tree, node_t *node1, tree_key_t key)
{
  if (node1 == NULL)
    {
      return 1;
    }

  if (tree->comp(node1->key, key) > 0)
    {
      puts("key smaller than tree-key" );
      tree_has_key_aux(tree, node1->left, key);
    }
  else if (tree->comp(node1->key, key) < 0)
    {
      puts("key bigger than tree->key" );
      tree_has_key_aux(tree, node1->right, key);
    }
  else
    {
      puts("key exists" );
      return 0;
    }

  puts("node doesnt exist" );
  return 1;
}



bool tree_has_key(tree_t *tree, tree_key_t key)
{
  puts("checker" );
  if (tree == NULL || tree->root == NULL)
    {
      puts("tree is empty" );
      return false;
    }
  else if (tree_has_key_aux(tree, tree->root, key) == 0)
    {
      return true;
    }

  else return false;
}



bool tree_get(tree_t *tree, tree_key_t key1, elem_t *result)
{
  if (tree == NULL || tree->root == NULL)
    {
      
      return false;
    }
  node_t *node = tree->root;
  while (node != NULL)
    {
      if (tree->comp == NULL) {
        
        int res = int_cmp(node->key, key1);
        if (res == 0) {
          return false;
        }
        else if (res > 0)
          {
            
            node = node->right;
          
          }
        else if (res < 0)
          {
            
            node = node->left;
          }
        else
          {
            
            *result = node->elem;
            printf("%d\n",(result->i));
            return true;
      
          }
      }
      if (tree->comp != NULL) {
      
        
        int res = tree->comp(node->key, key1);
      
        if (res > 0)
          {
      
            node = node->right;
          }
        else if (res < 0)
          {
      
            node = node->left;
          }
        else {
      
          *result = node->elem;
          printf("%d\n",(result->i));
          return true;
        }
      }
        
    }
  printf("%s\n","node doesnt exist" );
  return false;
}



node_t *balance_remove_node(node_t *node)
{
  node_t *cursor = node;
  while (cursor->left != NULL)
    {
      cursor = cursor->left;
    }
  return cursor;
}

bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result)
{
  node_t **cursor = &tree->root;
  node_t **node = get_node(tree, cursor, key);

  if(node == NULL || *node == NULL)
    { puts("hasjhop");
      return false;
    }


  node_t *remove_node = *node;

  if(remove_node->right == NULL && remove_node->left == NULL)
    {
      puts("hejhop");
      *result =remove_node->elem;
      free(remove_node);
      *node = NULL;
      return true;
    }
  else if (remove_node->right == NULL)
    {
      puts("hare");
      *result = remove_node->elem;
      *node = remove_node->left;
      free(remove_node);
      return true;
    }
  else if (remove_node->left == NULL)
    {
      puts("hullaahulaa");
      *result = remove_node->elem;
      *node = remove_node->right;
      free(remove_node);
      return true;
    }
  else
    {
      puts("waxa");
      node_t *tmp = balance_remove_node(remove_node->right);
      *result = remove_node->elem;
      tmp->left = remove_node->left;
      *node = tmp;
      free(remove_node);
    }
  return false;
}

void print_tree_aux (tree_t *tree, node_t *node)

{
  if (node == NULL)
    {
      return;

    }
  while (node != NULL)
    {
      printf("%s", "key:");
      printf("%s\n", (char *)node->key.p );
      print_tree_aux(tree, node->left);
      print_tree_aux(tree, node->right);
      return;
    }
  return;
}

void print_tree (tree_t *tree)
{
  printf("%s\n","existing keys:" );
  print_tree_aux(tree, tree->root);
  return;
}

void tree_balance_helper(tree_t *tree, tree_key_t *list_of_keys, elem_t *list_of_elements
                         ,int start,int stop)
{
  if (start > stop) return;
  else
    {
      int middle = ((start + stop)/2);
      printf("midde = %d, start = %d, stop = %d\n", middle, start, stop);
      tree_key_t key = list_of_keys[middle];
      elem_t elem = list_of_elements[middle];
      tree_insert(tree,key,elem);
      tree_balance_helper(tree,list_of_keys,list_of_elements,start,middle-1);
      tree_balance_helper(tree,list_of_keys,list_of_elements,middle+1,stop);
    }
}

tree_t *tree_balance(tree_t *tree)
{
  int size = tree_size(tree);
  tree_key_t *list_of_keys = tree_keys(tree);
  elem_t *list_of_elements = tree_elements(tree);

  tree_t *NewTree = tree_new(tree->copy,tree->free_key ,tree->free_elem ,tree->comp);
  tree_balance_helper(NewTree,list_of_keys,list_of_elements,0,size-1);


  tree_t *tmp = tree;
  tree = NewTree;
  NewTree = tmp;

  tree_delete(NewTree,false,false);

  free(list_of_keys);
  free(list_of_elements);
  return tree;
}

void tree_keys_helper(node_t *node, tree_key_t* list_of_keys,int * index)
{
  if(node == NULL)
    {
      return;
    }
  tree_keys_helper(node->left, (list_of_keys), index);
  list_of_keys[*index] = (node->key);
  *index = *index + 1;
  tree_keys_helper(node->right, (list_of_keys), index);

}
tree_key_t *tree_keys(tree_t *tree)
{
  int index = 0;
  int siz = tree_size(tree);
  tree_key_t* list_of_keys = (tree_key_t*)calloc(siz,sizeof(tree_key_t));
  tree_keys_helper(tree->root, list_of_keys,&index);
  return list_of_keys;
}

void tree_elements_helper(node_t *node, elem_t* list_of_elements, int *index)
{
  if(node == NULL)
    {
      return;
    }
  tree_elements_helper(node->left, (list_of_elements),index);
  list_of_elements[*index] = node->elem;
  *index = *index +1;
  tree_elements_helper(node->right, (list_of_elements),index);
}

elem_t *tree_elements(tree_t *tree)
{
  int index = 0;
  int siz = tree_size(tree);
  elem_t* list_of_elements = (elem_t*)calloc(siz,sizeof(elem_t*));
  tree_elements_helper(tree->root, list_of_elements, &index);
  return list_of_elements;
}

void apply_in_order (node_t *node, key_elem_apply_fun fun, void *data, bool *success)
{
  bool tmp = false;
  if (node == NULL)
    return ;

  if (node != NULL)
    {
      apply_in_order(node->left, fun, data, success);
      tmp = fun(node->key, node->elem, data);
      if (tmp) {
        *success = true;
      }
      apply_in_order(node->right, fun, data, success);
      return ;
    }
  else return;
}

void apply_post_order (node_t *node, key_elem_apply_fun fun, void *data, bool *success)
{
  bool tmp = false;
  if (node == NULL)
    {
      return;
    }
  if (node != NULL)
    {
      apply_post_order(node->left, fun, data, success);
      apply_post_order(node->right, fun, data, success);

      tmp = fun(node->key, node->elem, data);
      if (tmp)
        {
          *success = true;
        }
      return;
    }
  else return;

}

void apply_pre_order (node_t *node, key_elem_apply_fun fun, void *data, bool *success)
{
  bool tmp = false;
  if (node == NULL)
    {
      return;
    }
  if (node != NULL)
    {
      tmp = fun(node->key, node->elem, data);
      if (tmp) {
        *success = true;
      }
      apply_post_order(node->left, fun, data, success);
      apply_post_order(node->right, fun, data, success);
      return;
    }
  else return;
}


bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data)
{
  bool success = false;
  if (order == 0)
    {
      apply_in_order(tree->root, fun, data, &success);
      return success;

    }
  if (order == 1)
    {
      apply_post_order(tree->root, fun, data, &success);
      return success;
    }
  if (order == -1)
    {
      apply_pre_order(tree->root, fun, data, &success);
      return success;
    }
  else return false;
}
