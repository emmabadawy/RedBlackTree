#ifndef RBTREE_H
#define RBTREE_H
#include"rbnode.h"

class RBTree
{

public:
    RBTree();
    RBTree(RBTree& otherTree);
    const RBTree& operator=(const RBTree& othertree);
    bool rb_insert(int key);
    bool rb_delete(int key);
    bool rb_search (int key) const;
    bool isEmpty() const;
    bool isFull() const;
    int rb_length () const;
    int  rb_minimum () const;
    int rb_maximum () const;
    int rb_successor (int key) const;
    int rb_predecessor (int key) const;
    void rb_inOrder (void (*func)(int)) const;
    void rb_preOrder (void (*func)(int)) const;
    void rb_postOrder (void (*func)(int)) const;
    void rb_destroy();
    virtual ~RBTree();

private:
    RBNode* root;
    RBNode* TNULL;
    int size;


    void left_rotate (RBNode* node);
    void right_rotate (RBNode* node);
    void insert_fixup(RBNode* node);
    void delete_fixup(RBNode* node);
    void rb_transplant(RBNode* u, RBNode* v);
    void inOrder_helper (RBNode* current, void (*func)(int)) const;
    void preOrder_helper(RBNode* current, void (*func)(int)) const;
    void postOrder_helper(RBNode* current, void (*func)(int)) const;
    RBNode* min_helper (RBNode* node) const;
    RBNode* max_helper (RBNode* node) const;
    RBNode* successor_helper(RBNode* node) const ;
    RBNode* predecessor_helper(RBNode* node) const;
    RBNode* search_helper(int key) const;
    RBNode* rb_clone (RBNode* otherTreeRoot, RBNode* otherTreeTNULL, RBNode* parent);
    void destroy_helper (RBNode* &root);

};

#endif // RBTREE_H
