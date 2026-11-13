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
    void insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void remove_fix(AVLNode<Key,Value>* n, int8_t diff); 
    void rotateRight(AVLNode<Key,Value>* n);
    void rotateLeft(AVLNode<Key,Value>* n);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n)
{
  AVLNode<Key, Value>* p = n->getParent();
  AVLNode<Key, Value>* y = n->getLeft();
  AVLNode<Key, Value>* temp = y->getRight();

  if (p == NULL) this->root_ = y;
  else if (p->getLeft() == n) p->setLeft(y);
  else p->setRight(y);
  y->setParent(p);

  n->setLeft(temp);
  if (temp != NULL) temp->setParent(n);
  y->setRight(n);
  n->setParent(y);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n)
{
  AVLNode<Key, Value>* p = n->getParent();
  AVLNode<Key, Value>* y = n->getRight();
  AVLNode<Key, Value>* temp = y->getLeft();

  if (p == NULL) this->root_ = y;
  else if (p->getLeft() == n) p->setLeft(y);
  else p->setRight(y);
  y->setParent(p);

  n->setRight(temp);
  if (temp != NULL) temp->setParent(n);
  y->setLeft(n);
  n->setParent(y);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) 
{
  if (p == NULL || p->getParent() == NULL) return;
  AVLNode<Key,Value>* g = p->getParent();
  // p is left child of g
  if (g->getLeft() == p) 
  {
    g->updateBalance(-1);
    if (g->getBalance() == 0) 
    {
      return;
    } else if (g->getBalance() == -1) 
    {
      insert_fix(g, p);
    } else if (g->getBalance() == -2) 
    {
      if (p->getLeft() == n) 
      { // zig-zig
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
      } else 
      { // zig-zag
        rotateLeft(p);
        rotateRight(g);
        if (n->getBalance() == -1) {
          p->setBalance(0);
          g->setBalance(1);
          n->setBalance(-1);
        } else if (n->getBalance() == 0) {
          p->setBalance(0);
          g->setBalance(0);
        } else if (n->getBalance() == 1) {
          p->setBalance(-1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  } else  // p is right child of g
  {
    g->updateBalance(1);
    if (g->getBalance() == 0) 
    {
      return;
    } else if (g->getBalance() == 1) 
    {
      insert_fix(g, p);
    } else 
    {
      if (p->getRight() == n) 
      { // zig-zig
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
      } else
      { // zig-zag
        rotateRight(p);
        rotateLeft(g);
        if (n->getBalance() == 1) {
          p->setBalance(0);
          g->setBalance(-1);
          n->setBalance(0);
        } else if (n->getBalance() == 0) {
          p->setBalance(0);
          g->setBalance(0);
        } else if (n->getBalance() == -1) {
          p->setBalance(1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
  if (this->root_ == NULL) {
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    return;
  }

  AVLNode<Key,Value>* temp = static_cast<AVLNode<Key,Value>*>(this->root_);
  AVLNode<Key,Value>* p = NULL;
  while (temp != NULL) {
    p = temp;
    if (new_item.first < temp->getKey()) temp = temp->getLeft();
    else if (new_item.first > temp->getKey()) temp = temp->getRight();
    else {
      temp->setValue(new_item.second);
      return;
    }
  }

  AVLNode<Key,Value>* n = new AVLNode<Key,Value>(new_item.first, new_item.second, p);
  if (new_item.first < p->getKey()) {
    p->setLeft(n);
    p->updateBalance(-1);
  } else {
    p->setRight(n);
    p->updateBalance(1);
  }

  if (p->getBalance() != 0) insert_fix(p, n);
}


template<typename Key, typename Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>* n, int8_t diff)
{
  if (n == NULL) return;
  AVLNode<Key, Value>* p = n->getParent();
  int8_t ndiff = 0;
  if (p != NULL)
  {
    if (p->getLeft() == n) {
      ndiff = 1;
    } else if (p->getRight() == n) {
      ndiff = -1; 
    }
  }

  n->updateBalance(diff);
  if (n->getBalance() == -1 || n->getBalance() == 1) return;
  if (n->getBalance() == 0) {
    remove_fix(p, ndiff);
    return;
  }

  if (n->getBalance() == -2) {
    AVLNode<Key, Value>* c = n->getLeft();
    if (c->getBalance() <= 0) {
      int8_t b = c->getBalance();
      rotateRight(n);
      if (b == 0) {
        n->setBalance(-1);
        c->setBalance(1);
        return;
      }
      n->setBalance(0);
      c->setBalance(0);
    } else {
      AVLNode<Key, Value>* g = c->getRight();
      int8_t b = g->getBalance();
      rotateLeft(c);
      rotateRight(n);
      n->setBalance(b == -1 ? 1: 0);
      c->setBalance(b == 1 ? -1 : 0);
      g->setBalance(0);
    } 
  } else {
    AVLNode<Key, Value>* c = n->getRight();
    if (c->getBalance() >= 0) {
      int8_t b = c->getBalance();
      rotateLeft(n);
      if (b==0) {
        n->setBalance(1);
        c->setBalance(-1);
        return;
      }
      n->setBalance(0);
      c->setBalance(0);
    } else {
      AVLNode<Key, Value>* g = c->getLeft();
      int8_t b = g->getBalance();
      rotateRight(c);
      rotateLeft(n);
      n->setBalance(b == 1 ? -1: 0);
      c->setBalance(b == -1 ? 1 : 0);
      g->setBalance(0);
    }
  }

  remove_fix(p, ndiff);
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if (curr == NULL) return;
  
  if (curr->getLeft() != NULL && curr->getRight() != NULL) {
    AVLNode<Key,Value>* pred =
      static_cast<AVLNode<Key,Value>*>(this->predecessor(curr));
    nodeSwap(curr, pred);
  }

  AVLNode<Key, Value>* p = curr->getParent();
  AVLNode<Key, Value>* child = 
    (curr->getLeft() != NULL ? curr->getLeft() : curr->getRight());
  int8_t diff = 0;
  if (p == NULL) this->root_ = child;
  else if (p->getLeft() == curr) {
    p->setLeft(child);
    diff = 1;
  } else {
    p->setRight(child);
    diff = -1;
  }
  if (child != NULL) child->setParent(p);
  delete curr;
  remove_fix(p, diff);
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