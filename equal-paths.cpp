#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int count(Node* root)
{
if( root == NULL ) return 0;
else {
    if(count(root->left) > count(root->right)){
        return 1 + count(root->left);
    }else{
        return 1 + count(root->right);
    }
    }
} 

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL ) {
        return true;
    }else if ((root->left == NULL) &&(root->right == NULL)){
        return true;
    }else if(root->left == NULL){
        return equalPaths(root->right);
    }else if(root->right == NULL){
        return equalPaths(root->left);
    } 
    else{
        if ((count(root->left))!=(count(root->right))){
            return false;
        }else{
            return true;
        }
    }

}

