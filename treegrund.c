#include <stdlib.h>
#include <string.h>

#include "tree.h"

extern char* strdup(const char*);

typedef struct node {
  K key;
  T elem;
  struct node* left;
  struct node* right;
} node_t;

typedef struct tree {
  struct node* root;
} tree_t;

/* INTERNAL FUNCTIONS */

int _count_children(node_t* node) {
  int sum = 0;
  
  if (node->left != NULL) {
    sum = (_count_children(node->left) + 1);
  }

  if (node->right != NULL) {
    sum += (_count_children(node->right) + 1);
  }

  return sum;
}

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

node_t* _node_new(K key, T elem) {
  node_t* node = calloc(1, sizeof(node_t));
  
  if (node != NULL) {
    node->key = key;
    node->elem = elem;
  }

  return node;
}

void _node_delete(node_t* node, tree_action cleanup) {
  if (node == NULL) return;

  if (node->left != NULL) {
    _node_delete(node->left, cleanup);
    free(node->left);
  }

  if (node->right != NULL) {
    _node_delete(node->right, cleanup);
    free(node->right);
  }

  if (cleanup != NULL) {
    cleanup(node->key, node->elem);
  }
  
  return;
}

void _node_apply(node_t* node, enum tree_order order, tree_action2 fun, void* data) {
  if (order == preorder) {
    fun(node->key, node->elem, data);
    _node_apply(node->left, order, fun, data);
    _node_apply(node->right, order, fun, data);
  } else if (order == inorder) {
    _node_apply(node->left, order, fun, data);
    fun(node->key, node->elem, data);
    _node_apply(node->right, order, fun, data);
  } else if (order == postorder) {
    _node_apply(node->left, order, fun, data);
    _node_apply(node->right, order, fun, data);
    fun(node->key, node->elem, data);
  }
}

void _node_key(node_t* node, K* array, int* index) {
  if (node->left != NULL) {
    _node_key(node->left, array, index);
  }

  array[*index] = node->key;
  ++(*index);

  if (node->right != NULL) {
    _node_key(node->right, array, index);
  }

  return;
}

/* INTERFACE FUNCTIONS */


tree_t* tree_new() {
  tree_t *tree = calloc(1, sizeof(tree_t));
  return tree;
}

void tree_delete(tree_t* tree, tree_action cleanup) {
  _node_delete(tree->root, cleanup);
  free(tree->root);
  free(tree);
  return;
}

int tree_size(tree_t *tree) {
  if (tree == NULL || tree->root == NULL) return 0;

  return (1 + _count_children(tree->root));
}

int tree_depth(tree_t* tree) {
  if (tree == NULL || tree->root == NULL) return 0;

  return (1 + _depth_children(tree->root));
}

bool tree_insert(tree_t* tree, K key, T elem) {
  if (tree == NULL) return false;

  node_t* node = tree->root;

  key = strdup(key);

  if (node == NULL) {
    tree->root = _node_new(key, elem);
    return true;
  } else {
    int cmp = strcmp(key, node->key);

    while (true) {
      if (cmp < 0) {
        if (node->left != NULL) {
          node = node->left;
        } else {
          node->left = _node_new(key, elem);
          return true;
        }
      } else if (cmp > 0) {
        if (node->right != NULL) {
          node = node->right;
        } else {
          node->right = _node_new(key, elem);
          return true;
        }
      } else {
        return false;
      }
    }
  }
}

bool tree_has_key(tree_t* tree, K key) {
  if (tree == NULL || tree->root == NULL) return false;

  node_t* node = tree->root;

  while (node != NULL) {
    int cmp = strcmp(key, node->key);

    if (cmp < 0) {
      node = node->left;
    } else if (cmp > 0) {
      node = node->right;
    } else {
      return true;
    }
  }

  return false;
}

T tree_get(tree_t* tree, K key) {
  if (tree == NULL || tree->root == NULL) return false;

  node_t* node = tree->root;

  while (node != NULL) {
    int cmp = strcmp(key, node->key);

    if (cmp < 0) {
      node = node->left;
    } else if (cmp > 0) {
      node = node->right;
    } else {
      break;
    }
  }

  if (node != NULL) {
    return node->elem;
  } else {
    return NULL;
  }
}

K* tree_keys(tree_t* tree) {
  int size = tree_size(tree);

  if (size == 0 || tree->root == NULL) return NULL;

  K* array = calloc(size, sizeof(K));
  if (array == NULL) return NULL;

  int index_acc = 0;

  _node_key(tree->root, array, &index_acc);

  return array;
}

void tree_apply(tree_t* tree, enum tree_order order, tree_action2 fun, void* data) {
  if (tree == NULL || tree->root == NULL) return;

  _node_apply(tree->root, order, fun, data);
}
