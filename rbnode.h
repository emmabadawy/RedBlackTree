#ifndef RBNODE_H
#define RBNODE_H

// 0 is BLACK Node, 1 is RED Node.
class RBNode
{
    friend class RBTree;
public:
     RBNode();
    RBNode(int key);
    virtual ~RBNode();

private:
    int key;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    bool color;
};

#endif // RBNODE_H
