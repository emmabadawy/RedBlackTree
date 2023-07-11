#include "rbnode.h"

//There should not be any no actual keys with value 0.
RBNode:: RBNode()
{
    this->key = 0;
    this->color = 1;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
}

RBNode::RBNode(int key)
{
    this->key = key;
    this->color = 1;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
}

RBNode::~RBNode()
{
    //dtor
}
