#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include <algorithm>
#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    if(root == nullptr) return true;
    return (treeLength(root->right) == treeLength(root->left)) && equalPaths(root->left) && equalPaths(root->right);

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

