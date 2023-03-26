#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    if(root == nullptr) return true;
    if(root->left != nullptr && root->right != nullptr){
        return (treeLength(root->right) == treeLength(root->left)) && equalPaths(root->left) && equalPaths(root->right);
    }
    else if(root->left == nullptr){
        return (equalPaths(root->right));
    }
    else if(root->right == nullptr){
        return (equalPaths(root->left));
    }
}


// find tree length from root to a given node
int treeLength(Node* node){
    if(node == nullptr){
        return 0;
    }
    else{
        return 1 + max(treeLength(node->left), treeLength(node->right));
    }
}

