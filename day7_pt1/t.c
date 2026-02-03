#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULTTREESIZE (128)

typedef enum {
  LEFT = -1,
  L_HEAVY = -2,
  RIGHT = 1,
  R_HEAVY = 2,
  EQUAL = 0,
} Balance;

typedef struct _TreeNode{
  int data;
  Balance b;
  struct _TreeNode* left;
  struct _TreeNode* right;
} TreeNode;

typedef struct {
  TreeNode* root;
  int allocated;
  int used;
} AVLTree_int;

// returns NULL on failure
AVLTree_int* AVLTree_intInit(void){
  AVLTree_int* ret = malloc(sizeof(AVLTree_int));
  TreeNode* _t = malloc(sizeof(TreeNode)*DEFAULTTREESIZE); 
  if (_t == NULL || ret == NULL) {
    fprintf(stderr, "Couldn't allocate memory for tree\n");
    return NULL;
  }
  ret->root = _t;
  ret->allocated = DEFAULTTREESIZE;
  ret->used = 0;
  return ret;
}

// returns 1 on reallocation error;
// returns 2 if item already in tree;
int AVLTree_intInsert(AVLTree_int* tree, int to_i) {
  if (tree->used >= tree->allocated){
    TreeNode* _t = realloc(tree->root, tree->allocated*sizeof(TreeNode)*2);
    if(_t == NULL){
      fprintf(stderr, "Couldn't re-allocate memory for tree\n");
      return 1;
    }
    tree->root = _t;
  }
  TreeNode* r = tree->root;
  while((r->data < to_i && r->left != NULL) || (r->data > to_i && r->right != NULL)) {
    r = r->data < to_i ? r->left : r->right;
  }
  if (r->data == to_i){
    // fprintf(stdout, "Already have that element %d\n", to_i);
    return 2;
  }
  TreeNode** to_modify = r->data < to_i ? &(r->left) : &(r->right);
  *to_modify = (tree->root+used);
  (*to_modify)->data = to_i;
  (*to_modify)->left = NULL;
  (*to_modify)->right = NULL;
  tree->used += 1;
  return 0;
}

void testAvl(){
}

int main(void) {
  // after each row, reasses where we are splitting from.
  // so we have a list of indexes where lazers are dropping from.
  // we can then usethe next row of the input file and 
  // by looping through the list of indexes, if the next row of text
  // has a '^' at that index, then we split it into it's left and right
  // until the last line of the text file.
  FILE* f = fopen("input.txt", "r");
  if(f == NULL){
    fprintf(stderr, "couldn't find input file");
  }
  char c;
  while((c = fgetc(f)) != EOF) {
    printf("%c", c);
  }
  testAvl();
}
