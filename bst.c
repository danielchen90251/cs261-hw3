/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Chiu-Chun Chen
 * Email: chenchiu@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  struct bst* bst = malloc(sizeof(struct bst));
  bst->root = NULL;
  return bst;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {
  assert(bst);
  free(bst);
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */

// additional function
int count_node(struct bst_node* curr) {
  return curr != NULL ? 1 + count_node(curr->left) + count_node(curr->right) : 0;
}

int bst_size(struct bst* bst) {
  return count_node(bst->root);
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
  struct bst_node* parent = NULL;
  struct bst_node* curr = bst->root;

  while (curr!= NULL){
    parent = curr;
    if (key < curr->key){
      curr = curr->left;
    }else if (key > curr->key){
      curr = curr->right;
    }
  }
  // new node
  struct bst_node* add = malloc(sizeof(struct bst_node));
  add->left = NULL;
  add->right = NULL;
  add->key = key;
  add->value = value;

  if (parent == NULL){
    bst->root = add;
  }else{
    if (key < parent->key){
      parent->left = add;
    }else if (key > parent->key){
      parent->right = add;
    }
  }
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
  struct bst_node* parent = NULL;
  struct bst_node* curr = bst->root;

  if (curr == NULL){
    return;
  }

  // locate current pointer 
  while (curr->key != key){
    parent = curr;
    if (key < curr->key){
      curr = curr->left;
    }else{
      curr = curr->right;
    }
  }

  if (curr->left == NULL && curr->right == NULL){
  // case 1: no child
    if (curr == bst->root){
      bst->root = NULL;
    }else{
      if (parent->left == curr){
        parent->left = NULL;
      }else if (parent->right == curr){
        parent->right = NULL;
      }
    }
    free(curr);
  }else if (curr->right == NULL && curr->left != NULL){
  // case 2: only left child
    struct bst_node* child = curr->left;
    if (curr == bst->root){
      bst->root = child;
    }else{
      if (parent->left == curr){
        parent->left = child;
      }else if (parent->right == curr){
        parent->right = child;
      }
    }
    free(curr);
  }else if (curr->left == NULL && curr->right != NULL){
  // case 3: only right child 
    struct bst_node* child = curr->right;
    if (curr == bst->root){
      bst->root = child;
    }else{
      if (parent->left == curr){
        parent->left = child;
      }else if (parent->right == curr){
        parent->right = child;
      }
    }
    free(curr);
  }else{
  // case 4: two children -> remove first encountered
    struct bst_node* child = curr->right;
    while (child->left != NULL){
      child = child->left;
    }
    
    int first_key = child->key;
    void* first_val = child->value;

    bst_remove(bst, first_key);
    curr->key = first_key;
    curr->value = first_val;
  }
  
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  struct bst_node* parent = NULL;
  struct bst_node* curr = bst->root;

  while (curr != NULL && curr->key != key){
    parent = curr;
    if (key < curr->key){
      curr = curr->left;
    }else{
      curr = curr->right;
    }
  }

  return curr;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */

// additional function
int count_height(struct bst_node* curr) {
  if (curr == NULL){
    return -1;
  }else{
    // compute height of each subtree
    int left_height = count_height(curr->left);
    int right_height = count_height(curr->right);
    // use the larger height
    if (left_height > right_height){
      return left_height + 1;
    }else{
      return right_height + 1;
    }
  }
}

int bst_height(struct bst* bst) {
  return count_height(bst->root);
}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */

// additional function
int count_sum(struct bst_node* curr, int sum){
  
  // base case: leaf node
  if (curr->left == NULL && curr->right == NULL){
    if (sum == curr->key){
      return 1;
    }else{
      return 0;
    }
  }

  int left_path, right_path = 0;
  // have left path
  if (curr->left != NULL){
    left_path = count_sum(curr->left, sum - curr->key);
  }
  // have right path
  if (curr->right != NULL){
    right_path = count_sum(curr->right, sum - curr->key);
  }
  // if have at least one path, return 1
  if (left_path == 1 || right_path == 1){
    return 1;
  }else{
    return 0;
  }
} 

int bst_path_sum(struct bst* bst, int sum) {
  return count_sum(bst->root, sum);
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */

// additional function
int count_range(struct bst_node* curr, int lower, int upper){
  // base case
  if (curr == NULL){
    return 0;
  }

  if (curr->key >= lower && curr->key <= upper){
    return curr->key + count_range(curr->right, lower, upper) + count_range(curr->left, lower, upper);
  }else if (curr->key < lower){
    return count_range(curr->right, lower, upper);
  }else{
    return count_range(curr->left, lower, upper);
  }
}

int bst_range_sum(struct bst* bst, int lower, int upper) {
  return count_range(bst->root, lower, upper);
}

/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack* stack;
};

/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */

// additional function
void dfs(struct bst_node* curr, struct stack* stack) {
  if (curr == NULL){
    return;
  }
  // stack: order from big to small
  dfs(curr->right, stack);
  stack_push(stack, curr);
  dfs(curr->left, stack);
}

struct bst_iterator* bst_iterator_create(struct bst* bst) {
  struct bst_iterator* iter = malloc(sizeof(struct bst_iterator));
  iter->stack = stack_create();
  dfs(bst->root, iter->stack);
  assert(iter);
  return iter;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator* iter) {
  assert(iter);
  free(iter);
}

/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator* iter) {
  return !stack_isempty(iter->stack);
}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator* iter, void** value) {
  if (bst_iterator_has_next(iter)){
    struct bst_node* curr = stack_pop(iter->stack);
    *value = curr->value;
    return curr->key;
  }
  
  if (value){
    *value = NULL;
  }

  return 0;
}
