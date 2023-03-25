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
     void insertfix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
     void removefix(AVLNode<Key,Value>* n, int diff);
    void rotateRight(AVLNode<Key,Value>* n);
    void rotateLeft(AVLNode<Key,Value>* n);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    this->print();
    if(this->internalFind(new_item.first)==NULL){
        AVLNode<Key, Value>* spot = static_cast<AVLNode<Key, Value>*>(this->root_);
        if(spot == NULL){
            
            AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
            this->root_ = newnode;
            newnode->setBalance(0);
            newnode->setLeft(NULL);
            newnode->setLeft(NULL);
        }
        else{
			AVLNode<Key, Value>* spotparent = NULL;

			while(spot!= NULL){
				if (spot->getKey() > new_item.first){
				    spotparent = spot;
                    spot = spot->getLeft();
                }else{
					spotparent = spot;
                    spot = spot->getRight();
					
                }
			}
            int leftorright = 0;	
            AVLNode<Key, Value>* addition = new AVLNode<Key, Value>(new_item.first, new_item.second, spotparent);
            if (spotparent->getKey() > addition->getKey()){
				spotparent->setLeft(addition);
			}else{
				spotparent->setRight(addition);
                leftorright = 1;
			}

            addition->setBalance(0);
            if(spotparent->getBalance()== 0){
                if(leftorright == 0){
                    spotparent->setBalance(-1);
                    insertfix(spotparent, addition);
                }else{
                    spotparent->setBalance(1);
                    insertfix(spotparent, addition);
                }
                
            }else{
                spotparent->setBalance(0);
            }
        }
	}else{
		this->internalFind(new_item.first)->setValue(new_item.second);
	}
    

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(this->internalFind(key)!=NULL){
        
		AVLNode<Key, Value>* spot = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
        AVLNode<Key, Value>* parent = spot->getParent();
        int diff = 0;
        if(parent!=NULL){
            if(spot == parent->getLeft() ){
                diff = 1;
            }else{
                diff = -1;
            }
        }
		if(spot->getRight() == NULL && spot->getLeft() == NULL){
            
            if(spot == static_cast<AVLNode<Key, Value>*>(this->root_)){
				this->root_ = NULL;
			}else if(spot == (spot->getParent()->getRight())){
				spot->getParent()->setRight(NULL);
			}else{
				spot->getParent()->setLeft(NULL);
			}
			delete spot;
		}
		else if(spot->getRight() == NULL){
            if(spot == static_cast<AVLNode<Key, Value>*>(this->root_)){
				this->root_ = spot->getLeft();
				this->root_->setParent(NULL);
			}
            else if(spot == (spot->getParent()->getRight())){
				spot->getParent()->setRight(spot->getLeft());
			}else{
				spot->getParent()->setLeft(spot->getLeft());
			}
			if(spot->getLeft() != NULL){
				spot->getLeft()->setParent(spot->getParent());
			}
                
			delete spot;
		}else if(spot->getLeft() == NULL){
			if(spot==static_cast<AVLNode<Key, Value>*>(this->root_)){
				this->root_ = spot->getRight();
				this->root_->setParent(NULL);
			}
                
            else if(spot == spot->getParent()->getRight()){
				spot->getParent()->setRight(spot->getRight());
			}else{
				spot->getParent()->setLeft(spot->getRight());
			}
			if(spot->getRight() != NULL){
				spot->getRight()->setParent(spot->getParent());
			}
			delete spot;
		}else{
			AVLNode<Key, Value>* switching = static_cast<AVLNode<Key, Value>*>(this->predecessor(spot));
			AVLNode<Key, Value>* parentofswitch = switching->getParent();
			if(parentofswitch == spot){
                nodeSwap(spot, switching);
         	    remove(key);
			}else{
                nodeSwap(spot, switching);
                remove(key);
			}
				
		}
      this->removefix(parent, diff);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertfix( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if(p == NULL){
        return;
    }
    AVLNode<Key,Value>* g = p->getParent();
    if(g==NULL){
        return;
    }
    if(p == g->getLeft()){
        g->updateBalance(-1);
        if(g->getBalance()==0){
            this->print();
            return;
        }else if(g->getBalance()==-1){
            insertfix(g,p);
        }else if(g->getBalance()==-2){
            if(n->getKey()>p->getKey()){
       
                rotateLeft(p);
           
                rotateRight(g);
                if(n->getBalance() == -1){
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }else{
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                    
                }
                p->setBalance(0);
                g->setBalance(0);
            }else{
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
        }
    }else{
        g->updateBalance(1);
        if(g->getBalance()==0){
            return;
        }else if(g->getBalance()==1){
            insertfix(g,p);
        }else if(g->getBalance()==2){
            if(n->getKey()<p->getKey()){
                
                rotateRight(p);
                rotateLeft(g);
                if(n->getBalance() == 1){
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }else if(n->getBalance()==0){
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }else{
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                    
                }
                p->setBalance(0);
                g->setBalance(0);
            }else{
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removefix(AVLNode<Key,Value>* n, int diff)
{
    
    if(n == NULL){
        return;
    }

    AVLNode<Key,Value>* p = n->getParent();
    int ndiff = 0;
    if(p != NULL){
        if(n == p->getLeft()){
            ndiff = 1;
        }else{
            ndiff = -1;
        }
    }

    if(diff == -1){
        if(n->getBalance() + diff == -1){

            n->setBalance(-1);
        }else if(n->getBalance() + diff == 0){
            n->setBalance(0);
            removefix(p, ndiff);
        }else if(n->getBalance() + diff == -2){
            
            AVLNode<Key,Value>* c = n->getLeft();
            if(c->getBalance()==-1){
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removefix(p, ndiff);
            }else if(c->getBalance()==0){
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }else{
                AVLNode<Key,Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance()==1){
                    n->setBalance(0);
                    c->setBalance(-1);
										g->setBalance(0);
                }else if(g->getBalance()==0){
										n->setBalance(0);
                    c->setBalance(0);
										g->setBalance(0);
								}else{
										n->setBalance(1);
                    c->setBalance(0);
										g->setBalance(0);
								}
								removefix(p, ndiff);
            }
        }else if(n->getBalance() + diff == 2){
			AVLNode<Key,Value>* c = n->getLeft();
            if(c->getBalance()==-1){
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removefix(p, ndiff);
            }else if(c->getBalance()==0){
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }else{
                AVLNode<Key,Value>* g = c->getRight();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance()==-1){
                    n->setBalance(0);
                    c->setBalance(1);
										g->setBalance(0);
                }else if(g->getBalance()==0){
										n->setBalance(0);
                    c->setBalance(0);
										g->setBalance(0);
								}else{
										n->setBalance(-1);
                    c->setBalance(0);
										g->setBalance(0);
								}
								removefix(p, ndiff);
            }
				}
    }else{
		if(n->getBalance() + diff == 1){
            n->setBalance(1);
        }else if(n->getBalance() + diff == 0){
            n->setBalance(0);
            removefix(p, ndiff);
        }else if(n->getBalance() + diff == -2){
            AVLNode<Key,Value>* c = n->getRight();
            if(c->getBalance()==-1){
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removefix(p, ndiff);
            }else if(c->getBalance()==0){
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }else{
                AVLNode<Key,Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance()==1){
                    n->setBalance(0);
                    c->setBalance(-1);
										g->setBalance(0);
                }else if(g->getBalance()==0){
										n->setBalance(0);
                    c->setBalance(0);
										g->setBalance(0);
								}else{
										n->setBalance(1);
                    c->setBalance(0);
										g->setBalance(0);
								}
								removefix(p, ndiff);
            }
        }else if(n->getBalance() + diff == 2){
            this->print();
			AVLNode<Key,Value>* c = n->getRight();
            if(c->getBalance()==1){
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removefix(p, ndiff);
            }else if(c->getBalance()==0){
                rotateLeft(n);
                n->setBalance(-1);
                c->setBalance(1);
            }else{
                AVLNode<Key,Value>* g = c->getRight();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance()==-1){
                    n->setBalance(0);
                    c->setBalance(1);
										g->setBalance(0);
                }else if(g->getBalance()==0){
										n->setBalance(0);
                    c->setBalance(0);
										g->setBalance(0);
								}else{
										n->setBalance(-1);
                    c->setBalance(0);
										g->setBalance(0);
								}
								removefix(p, ndiff);
            }
				}
		}
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight( AVLNode<Key,Value>* n)
{
    AVLNode<Key, Value>* rootnow = n->getLeft();
    n->setLeft(rootnow->getRight());
    rootnow->setRight(n);
    rootnow->setParent(n->getParent());
    if(n->getParent()!=NULL){
        n->getParent()->setRight(rootnow);
    }
    n->setParent(rootnow);

    if(static_cast<AVLNode<Key, Value>*>(this->root_) == n){
        this->root_ = rootnow;
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key,Value>* n)
{
    AVLNode<Key, Value>* rootnow = n->getRight();
    n->setRight(rootnow->getLeft());
    rootnow->setLeft(n);

    rootnow->setParent(n->getParent());
    if(n->getParent()!=NULL){
        n->getParent()->setLeft(rootnow);
    }
    n->setParent(rootnow);
    if(static_cast<AVLNode<Key, Value>*>(this->root_) == n){
        this->root_ = rootnow;
    }
}
#endif
