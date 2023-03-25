#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int getHeight(AVLNode<Key,Value>* root) const;
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
    bool zigZig(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g);
    bool zigZag(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g);
    void removeFix(AVLNode<Key,Value>* node, int diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // if tree is empty, insert node at top
    if(this->empty()){
        AVLNode<Key, Value>* initialNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this->root_ = initialNode;
        initialNode->setBalance(0);
    }
    // if key already exists in tree, override current value
    else if(internalFind(new_item.first) != nullptr){
        AVLNode<Key, Value> *changedNode = internalFind(new_item.first);
        changedNode->setValue(new_item.second);
    }

    // otherwise, walk down tree, find place to insert
    else{
        AVLNode<Key, Value>* temp = this->root_;
        AVLNode<Key, Value>* tempParent = nullptr;
        while(temp != nullptr){
            // if greater than root_, move right
            tempParent = temp;
            if(temp->getKey() < new_item.first){
                temp = temp->getRight();
            }
            // else, move left
            else{
                temp = temp->getLeft();
            }
        }
        AVLNode<Key, Value>* addedNode = new AVLNode<Key, Value>(new_item.first, new_item.second, tempParent);
        addedNode->setBalance(0);
        // if temp has balance of -1 or 1, set balance to 0
        if(tempParent->getBalance() == -1 || tempParent->getBalance() == 1){
            tempParent->setBalance(0);
        }
        // else, temp is 0 
        else{
            // connecting parent node ptr to added node (right)
            if(tempParent->getKey() < new_item.first){
                tempParent->setRight(addedNode);
                tempParent->setBalance(1);
            }
            // connecting parent node ptr to added node (left)
            else{
                tempParent->setLeft(addedNode);
                tempParent->setBalance(-1);
            }
            insertFix(tempParent, temp);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node){
    if(parent == nullptr || parent->getParent() == nullptr){
        return;
    }
    AVLNode<Key,Value>* grand = parent->getParent();
    bool pLeftofG;
    // check if parent is left child of grand
    if(grand->getLeft() == parent){
        pLeftofG = true;
    }
    // else, parent is right child of grand
    else{
        pLeftofG = false;
    }
    // update grands balance to new value depending on if left or right child
    if(pLeftofG){
        grand->updateBalance(grand->getBalance()-1)
    }
    // else, is right child, and update accordingly. 
    else{
        grand->updateBalance(grand->getBalance()+1)
    }
    // if balance of grand is zero, return
    if(grand->getBalance() == 0){
        return;
    }
    // else if balance of grand is -1 or 1, recurse
    else if(grand->getBalance() == -1 || grand->getBalance() == 1){
        insertFix(grand, parent);
    }
    // else balance is 2 or -2
    else{
        // if zig zig, rotate accordingly
        if(zigZig(node, parent, grand)){
            // if left child, rotateRight grand
            if(pLeftofG){
                rotateRight(grand);
            }
            // else, is right child, rotateLeft grand
            else{
                rotateLeft(grand);
            }
            // set grand balance and parent balance to 0
            parent->setBalance(0);
            grand->setBalance(0);
        }
        // if zig zag, rotate accordingly
        if(zigZag(node, parent, grand)){
            // if left child, rotateLeft parent, rotateRight grand
            if(pLeftofG){
                rotateLeft(parent);
                rotateRight(grand);
            }
            // else, is right child, rotateRight parent, rotateLeft grand
            else{
                rotateRight(parent);
                rotateLeft(grand);
            }
            // if pLeftofG
            if(pLeftofG){
                // if node balance is -1, parent balance = 0, grand balance = 1 node balance = 0
                if(node->getBalance() == -1){
                    parent->setBalance(0);
                    grand->setBalance(1);
                    node->setBalance(0);
                }
                // else if node balance is 0, parent balance = 0, grand balance = 0, node balance = 0
                else if(node->getBalance() == 0){
                    parent->setBalance(0);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
                // else node balance is 1, parent balance = -1, grand balance = 0, node balance = 0
                else{
                    parent->setBalance(-1);
                    grand->setBalance(0);
                    node->setBalance(0);

                }
            }
            // else if, p is right of g
            else{
                // if node balance is 1, parent balance = 0, grand balance = -1 node balance = 0
                if(node->getBalance() == 1){
                    parent->setBalance(0);
                    grand->setBalance(-1);
                    node->setBalance(0);
                }
                // else if node balance is 0, parent balance = 0, grand balance = 0, node balance = 0
                else if(node->getBalance() == 0){
                    parent->setBalance(0);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
                // else node balance is 1, parent balance = 1, grand balance = 0, node balance = 0
                else{
                    parent->setBalance(1);
                    grand->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }   
}


// rotate right
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){
    AVLNode<Key,Value>* parent = node->getParent();
    AVLNode<Key,Value>* leftChild = node->getLeft();

    bool isLeftNode;
    if(parent->getLeft() == node){
        isLeftNode = true;
    }
    else{
        isLeftNode = false;
    }


    // check for right child existence 
    AVLNode<Key,Value>* rightChild = nullptr;
    if(leftChild->getRight() != nullptr){
        // store right child
        rightChild = leftChild->getRight();
        leftChild->setRight(nullptr);
    }
    // if node is root, rotate accordingly
    if(this->root_ == node){
        this->root_ = leftChild;
        node->setParent(leftChild);
        leftChild->setRight(node);
        leftChild->setParent(nullptr);
    }
    // else,rotate accordingly
    else{
        // if node is right child
        if(isLeftNode){
            parent->setLeft(leftChild);
        }
        // else, node is left child
        else{
            parent->setRight(leftChild);
        }
        leftChild->setParent(parent);
        node->setParent(leftChild);
        leftChild->setRight(node);
    }

    // if right child doesnt exist
    if(rightChild == nullptr){
        node->setLeft(nullptr);
    }
    // if left child exists
    else{
        node->setLeft(rightChild);
        temp->setParent(node);
    }
}

// rotate left
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){
    AVLNode<Key,Value>* parent = node->getParent();
    AVLNode<Key,Value>* rightChild = node->getRight();

    bool isRightNode;
    if(parent->getRight() == node){
        isRightNode = true;
    }
    else{
        isRightNode = false;
    }


    // check for left child existence 
    AVLNode<Key,Value>* leftChild = nullptr;
    if(rightChild->getLeft() != nullptr){
        // store left child
        leftChild = rightChild->getLeft();
        rightChild->setLeft(nullptr);
    }
    // if node is root, rotate accordingly
    if(this->root_ == node){
        this->root_ = rightChild;
        node->setParent(rightChild);
        rightChild->setLeft(node);
        rightChild->setParent(nullptr);
    }
    // else, rotate accordingly
    else{
        // if node is right child
        if(isRightNode){
            parent->setRight(rightChild);
        }
        // else, node is left child
        else{
            parent->setLeft(rightChild);
        }
        rightChild->setParent(parent);
        node->setParent(rightChild);
        rightChild->setLeft(node);
    }

    // if left child doesnt exist
    if(leftChild == nullptr){
        node->setRight(nullptr);
    }
    // if right child exists
    else{
        node->setRight(leftChild);
        temp->setParent(node);
    }
}


template<class Key, class Value>
bool AVLTree<Key, Value>::zigZig(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g){
    // left of left case
    if(g->getLeft() == p && p->getLeft() == n){
        return true;
    }
    // right of right case
    if(g->getRight() == p && p->getRight() == n){
        return true;
    }
    return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::zigZag(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p, AVLNode<Key,Value>* g){
    // left then right
    if(g->getLeft() == p && p->getRight() == n){
        return true;
    }
    // right then left
    if(g->getRight() == p && p->getLeft() == n){
        return true;
    }
    return false;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // if key is not found, return
    if(static_cast<AVLNode<Key, Value>*>(internalFind(key)) == nullptr){
        return;
    }
    // if key is found, delete
    AVLNode<Key, Value>* removed = static_cast<AVLNode<Key, Value>*>(internalFind(key));
    else{   
        AVLNode<Key, Value> *node = static_cast<AVLNode<Key, Value>*>(internalFind(key));
        // if two children exist, swap with pred
        if(node->getLeft() != nullptr && node->getRight() != nullptr){
            AVLNode<Key,Value>* temp = static_cast<AVLNode<Key, Value>*>(this->predecessor());
            nodeSwap(temp, removed);
        }

        
        int diff = 0; 
        AVLNode<Key,Value>* parent = removed->getParent();

        bool removedIsLeft;
        if(parent->getLeft() == removed){
            removedIsLeft = true;
        }
        else{
            removedIsLeft = false;
        }

        // if parent exists
        if(parent != nullptr){
            if(parent->getLeft() == removed){
                diff = 1;
            }
            else if(parent->getRight() == removed){
                diff = -1;
            }
        }
    
        // node has no children
        if(node->getLeft() == nullptr && node->getRight() == nullptr){
            // root case
            if(parent == nullptr){
                this->root_ = nullptr;
                delete removed;
                return;
            }

            // removed is a left node
            else if(removedIsLeft){
                parent->setLeft(nullptr);
                delete removed;

            }
            // removed is a right node
            else if(!removedIsLeft){
                parent->setRight(nullptr);
                delete removed;
            }
        }

        // node has single children
        else{
            AVLNode<Key, Value>* child = nullptr;
            bool childIsLeft;
            // if statement to make child ptr 
            if(removed->getRight() == nullptr){
                child = removed->getLeft();
                childIsLeft = true;
            }
            else{
                child = removed->getRight();
                childIsLeft = false;
            }

            // root case
            if(parent == nullptr){
                child->setParent(nullptr);
                this->root_ = child;
                delete removed;
            }
            else{
                // has left child
                if(childIsLeft){
                    if(parent->getLeft() == removed){
                        parent->setLeft(child);
                    }
                    else{
                        parent->setRight(child);
                    }
                    child->setParent(parent);
                    delete removed;
                }
                
                // has right child
                else{
                    if(parent->getRight() == removed){
                        parent->setRight(child);
                    }
                    else{
                        parent->setLeft(child);
                    }
                    child->setParent(parent);
                    delete removed;
                }
            }
            removeFix(parent, diff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* node, int diff) {
    // if node is null, return 
    if(node == nullptr){
        return;
    }
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = node->getLeft();
    
    int ndiff = 0;
    // establishing ndiff
    if(parent != nullptr){
        if(p->getRight() == node){
            ndiff = -1;
        }
        else{
            ndiff = 1;
        }
    }

    // diff == -2
    if(node->getBalance() + diff == -2){
        if(child == nullptr){
            return;
        }
        // case 1a
        if(child->getBalance == -1){
            rotateRight(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, ndiff);
        }
        // case 1b
        else if(child->getBalance() == 0){
            rotateRight(node);
            node->setBalance(-1);
            child->setBalance(1);
            return;
        }
        // case 1c
        else if(child->getBalance() == 1){
            AVLNode<Key, Value>* grand = child->getRight();
            if(grand == nullptr){
                return;
            }
            rotateLeft(child);
            rotateRight(node);
            if(grand->getBalance() == 1){
                node->setBalance(0);
                child->setBalance(-1);
                grand->setBalance(0);
            }
            else if(grand->getBalance() == 0){
                node->setBalance(0);
                child->setBalance(0);
                grand->setBalance(0);
            }
            else if(grand->getBalance() == -1){
                node->setBalance(1);
                child->setBalance(0);
                grand->setBalance(0);
            }
            removeFix(parent, ndiff);
        }


    }
    // diff == 2
    else if(node->getBalance() + diff == 2){
        if(child == nullptr){
            return;
        }
        // case 1a
        if(child->getBalance == 1){
            rotateLeft(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, ndiff);
        }
        // case 1b
        else if(child->getBalance() == 0){
            rotateLeft(node);
            node->setBalance(1);
            child->setBalance(-1);
            return;
        }
        // case 1c
        else if(child->getBalance() == -1){
            AVLNode<Key, Value>* grand = child->getLeft();
            if(grand == nullptr){
                return;
            }
            rotateRight(child);
            rotateLeft(node);
            if(grand->getBalance() == -1){
                node->setBalance(0);
                child->setBalance(1);
                grand->setBalance(0);
            }
            else if(grand->getBalance() == 0){
                node->setBalance(0);
                child->setBalance(0);
                grand->setBalance(0);
            }
            else if(grand->getBalance() == 1){
                node->setBalance(-1);
                child->setBalance(0);
                grand->setBalance(0);
            }
            removeFix(parent, ndiff);
        }


    }
    else if(node->getBalance() + diff == -1){
        node->setBalance(-1);
        return;
    }
    else if(node->getBalance() + diff == 1){
        node->setBalance(1);
        return;
    }
    else{
        node->setBalance(0);
        removeFix(parent, ndiff);
    }

}







// HELPER FUNCTION TO FIND HEIGHT OF TREE
template<typename Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key,Value>* root) const{
    if(root == nullptr){
        return 0;
    }
    return 1 + std::max(getHeight(root->getLeft()), getHeight(root->getRight()));
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
