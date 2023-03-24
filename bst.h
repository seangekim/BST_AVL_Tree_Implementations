#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;


    // Add helper functions here
    int getHeight(Node<Key,Value>* root) const;
	  bool balanceHelper(Node<Key, Value>* node) const; 
    static Node<Key, Value>* successor(Node<Key, Value>* current);



protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    // TODO
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    // TODO
    current_ = nullptr;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    return current_ != rhs.current_;

}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    Node<Key, Value>* temp = nullptr;
    // if right of current is valid, go right
    if(current_->getRight() != nullptr){
      temp = current_->getRight();
      // loop until left most node is found
      while(temp->getLeft() != nullptr){
        temp = temp->getLeft();
      }
      this->current_ = temp;
      return *this;
    }
    else{
      temp = current_;
      //loop until parent is null
      while(temp->getParent() != nullptr){
        if(temp == temp->getParent()->getLeft()){
          this->current_ = temp->getParent();
          return *this;
        }
        temp = temp->getParent();
      }
    }
    current_ = nullptr;
    return *this;
}

    


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    // TODO
    root_ = nullptr;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    // TODO
    this->clear();

}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // if tree is empty, insert node at top
    if(this->empty()){
        Node<Key, Value>* initialNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        root_ = initialNode;
    }
    // if key already exists in tree, override current value
    else if(internalFind(keyValuePair.first) != nullptr) {
        Node<Key, Value>* changedNode = internalFind(keyValuePair.first);
        changedNode->setValue(keyValuePair.second);
    }

    // walk down tree until find place to insert
    else{
        Node<Key, Value>* temp = this->root_;
        // parent of temp
        Node<Key, Value>* tempParent = nullptr;
        // while loop to loop through until temp becomes nullptr
        while(temp != nullptr){
            // if greater than root_, move right
            tempParent = temp;
            if(temp->getKey() < keyValuePair.first){
                temp = temp->getRight();
            }
            // else move left
            else if(temp->getKey() > keyValuePair.first){
                temp = temp->getLeft();
            }
        }
        
        Node<Key, Value>* addedNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, tempParent);

        // connecting parent node ptr to addedNode
        if(tempParent->getKey() < keyValuePair.first){
            tempParent->setRight(addedNode);
        }
        else if(tempParent->getKey() > keyValuePair.first){
            tempParent->setLeft(addedNode);
        }

    }
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    // curr is node to be deleted
    Node<Key, Value> *curr = internalFind(key);
    if(curr == nullptr){
        return;
    }
    // node has two children
    if(curr->getLeft() != nullptr && curr->getRight() != nullptr){  
        // find predecessor, swap two nodes
        Node<Key, Value>* pred = predecessor(curr);
        nodeSwap(pred, curr);


    }
    // node has zero children 
    if(curr->getLeft() == nullptr && curr->getRight() == nullptr){
        // if node is root node
        if(curr->getParent() == nullptr){
            delete curr;
            root_ = nullptr;
            return;
        }
        // else, node is not root node
        else{
            // if node is left node, set parent's left node to nullptr
            if(curr->getParent()->getLeft() == curr){
                // set parent's left node to nullptr
                curr->getParent()->setLeft(nullptr);
                // delete curr
                delete curr;
                return;
            }
            // else, node is right node, set parent's right node to nullptr
            else{
                // set parent's right node to nullptr
                curr->getParent()->setRight(nullptr);
                // delete node
                delete curr;
                return;

            }
        }
    }
    // node has one children
    else{
        // set temp to garbage value
        Node<Key, Value>* temp = nullptr;
        // if left node is null, set temp to right node
        if(curr->getLeft() == nullptr){
            temp = curr->getRight();
        }
        // else, right node is null, set temp to left node
        else{
            temp = curr->getLeft();
        }
        // if curr is root node, promote node and delete. 
        if(curr->getParent() == nullptr){
            temp->setParent(nullptr);
            delete curr;
            root_ = temp;
            return;
        }
        // else, node is non root node
        else{
            temp->setParent(curr->getParent());
            // if curr is a left node
            if(curr->getParent()->getLeft() == curr){  
                curr->getParent()->setLeft(temp);
            }
            // else curr is a right node
            else{
                curr->getParent()->setRight(temp);
            }
            delete curr;
            return;
        }   
    }
}



template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // next smallest value in the tree
    Node<Key, Value>* temp = current;
    // if left child doesnt exist, walk up ancestor chain until a right child is found, then that parent is the pred
    if(temp->getLeft() == nullptr){
        Node<Key, Value>* par_curr = temp->getParent();
        // loop until par_curr is the parent of a right node. 
        while(par_curr->getRight() != temp){
            temp = par_curr;
            par_curr = par_curr->getParent();
        }
        return par_curr;
    }
    // left child exists, find right most node
    else{
        temp = temp->getLeft();
        // loop until temp becomes right most child
        while(temp->getRight() != nullptr){
            temp = temp->getRight();
        }
        return temp;
    }
}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current){
    // next biggest value in the tree
    // if right child doesnt exist, walk up ancestor chain until a left child is found, then that parent is the pred
    if(current->getRight() == nullptr){
        // loop until current is the parent of a left node
        current = current->getParent(); 
        while(current->getParent() != nullptr && current->getParent()->getRight() == current){
            current = current->getParent();
        }
    }
    // right child exists, find left most node
    else if(current->getRight() != nullptr){
        current = current->getRight();
        // loop until temp becomes left most child
        while(current->getLeft() != nullptr){
            current = current->getLeft();
        }
    }
    return current;
}



/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    // while not empty, call remove func
    while(!this->empty()){
        remove(root_->getKey());
    }
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    Node<Key, Value>* temp = root_;
    if(root_ == nullptr){
      return nullptr;
    }
    // while another left node exists, go to that node
    while(temp->getLeft() != nullptr){
        temp = temp->getLeft();
    }
    return temp;
}

// helper function to find height of tree
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key,Value>* root) const{
    if(root == nullptr){
        return 0;
    }
    return 1 + std::max(getHeight(root->getLeft()), getHeight(root->getRight()));
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    // TODO
    Node<Key, Value>* temp = this->root_;
    // while temp is not nullptr
    while(temp != nullptr){
        // if key is leaf node, then value was not found, return nullptr
        if(temp->getKey() == key){
            return temp;
        }
        // if key is greater than value at temp, go right 
        if(key > temp->getKey()){
            temp = temp->getRight();
        }
        // else if, go left
        else if(key < temp->getKey()){
            temp = temp->getLeft();
        }
    }
    return nullptr;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    // call helper function
    return balanceHelper(root_);

}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::balanceHelper(Node<Key, Value>* node) const {
    if(node == nullptr){
        return true;
    }
    int left = getHeight(node->getLeft());
    int right = getHeight(node->getRight());
    
    // bool to see if height has difference of one
    int diff = left - right;
    bool heightDiff;
    // check that height diff is between -1 and 1
    if(diff <= 1 && diff >= -1){
        heightDiff = true;
    }
    else{
        heightDiff = false;
    }
    // recursively run helper function
    if(balanceHelper(node->getLeft()) && balanceHelper(node->getRight()) && heightDiff){
        return true;
    }
    else{
        return false;
    }

}







template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
