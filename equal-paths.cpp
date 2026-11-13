#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;

// You may add any prototypes of helper functions here

// gets the height of a certain path down to the bottom
bool height(Node* root, int curr_height, int& prev_height) {
  if (root == NULL) {
    if (prev_height == -1) prev_height = curr_height;
    else if (curr_height != prev_height) return false;
    return true;
  }

  if (root->left == NULL && root->right != NULL) return height(root->right, curr_height + 1, prev_height);
  else if (root->left != NULL && root->right == NULL) return height(root->left, curr_height + 1, prev_height);
  else return height(root->left, curr_height + 1, prev_height) && height(root->right, curr_height + 1, prev_height);
}

bool equalPaths(Node * root)
{
    // Add your code below
  if (root == NULL) return true;
  int prev_height = -1;
  return height(root, 0, prev_height);
}