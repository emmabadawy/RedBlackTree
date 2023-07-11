#include "rbtree.h"
#include "rbnode.h"

/*There is no key with 0 value, only TNULL dummy key.
 *0 means BLACK node, 1 means RED node.
 *Parent of root and all leaves is TNULL.
 *TNULL's parent is nullptr.
 *No duplicates allowed. */

RBTree::RBTree()
{
    TNULL = new RBNode(0);
    TNULL -> left = nullptr;
    TNULL -> right = nullptr;
    TNULL -> color = 0;
    TNULL -> parent = nullptr;
    root = TNULL;
    size = 0;
}

bool RBTree::rb_insert(int key)
{
    RBNode* new_node = new RBNode(key);
    new_node ->color = 1;
    new_node ->left = TNULL;
    new_node ->right = TNULL;

    RBNode* prev{TNULL};
    RBNode* current{root};
    while(current != TNULL)
    {
        prev = current;
        int node_key {current -> key};

        //NO duplicates.
        if(key == node_key)
        {
            return false;
        }
        else if(key < node_key)
        {
            current = current -> left;
        }
        else
        {
            current = current -> right;
        }
    }

    //It's very Important to increase size here instead of adding same line in every consecutive condition.
    ++size;

    //This also assigns TNULL as parent of root.
    new_node -> parent = prev;
    if (prev == TNULL)
    {
        root = new_node;
        new_node -> color = 0;
        return true;
    }
    else if (key < prev -> key )
    {
        prev -> left = new_node;
    }
    else
    {
        prev-> right = new_node;
    }

    if (new_node -> parent -> parent == TNULL)
    {
        return true;
    }

    insert_fixup(new_node);
    return true;
}

void RBTree::insert_fixup(RBNode* node)
{
    RBNode* uncle;
    while (node -> parent ->color == 1)
    {
        //Category A
        if (node -> parent == node -> parent -> parent -> left)
        {
            uncle = node -> parent -> parent -> right;
            //Case one.
            if(uncle -> color == 1)
            {
                node -> parent -> color = 0;
                uncle -> color = 0;
                node -> parent -> parent ->color = 1;
                node = node -> parent -> parent;
            }
            else
            {
                //Case two.
                if(node == node -> parent -> right)
                {
                    node = node -> parent;
                    left_rotate(node);
                }
                //Case three.
                node -> parent -> color = 0;
                node -> parent -> parent -> color = 1;
                right_rotate (node -> parent -> parent);

            }

        }
        else
        {
            //Category B
            uncle = node -> parent -> parent -> left;
            //Case one.
            if(uncle -> color == 1)
            {
                node -> parent -> color = 0;
                uncle -> color = 0;
                node -> parent -> parent ->color = 1;
                node = node -> parent -> parent;
            }
            else
            {
                //Case two.
                if(node == node -> parent -> left)
                {
                    node = node -> parent;
                    right_rotate(node);
                }
                //Case three.
                node -> parent -> color = 0;
                node -> parent -> parent -> color = 1;
                left_rotate (node -> parent -> parent);
            }
        }
        //to avoid having red node with red root parent which would cause denied access when TNULL is parent of root and grandparent of node.
        if (node == root)
            break;
    }

    root -> color = 0;
}

/*replaces a sub-tree (rooted at u) with another (rooted at v) while preserving the BST structure.
 *u is the deleted node and v is the one that replaces it.
 *Assigns a parent to TNULL nodes when v is TNULL.
 */
void RBTree::rb_transplant(RBNode* u, RBNode* v)
{
    if (u -> parent == TNULL)
    {
        root = v;
    }
    else if(u == u -> parent -> left )
    {
        u -> parent -> left  = v;
    }
    else
    {
        u -> parent -> right  = v;
    }

    v -> parent = u -> parent;
}

//Handles cases of deleting a black Node.
void RBTree::delete_fixup(RBNode* node)
{
    RBNode* sibling;
    while(node != root && node -> color == 0)
    {
        //Category A
        if(node == node -> parent -> left)
        {
            sibling =  node -> parent -> right;
            //Case one
            if(sibling -> color == 1)
            {
                sibling -> color = 0;
                node -> parent  -> color = 1;
                left_rotate( node -> parent);
                sibling =  node -> parent -> right;
            }
            //Case two
            if(sibling -> left -> color == 0 && sibling -> right -> color == 0)
            {
                sibling -> color = 1;
                node = node ->parent;
            }
            //Case Three.
            else if(sibling -> right -> color == 0)
            {
                sibling ->left -> color = 0;
                sibling ->color = 1;
                right_rotate(sibling);
                sibling = node ->parent -> right;
            }

            //Case four
            sibling -> color = node -> parent -> color;
            node -> parent -> color = 0;
            sibling -> right -> color = 0;
            left_rotate(node -> parent);
            node = root;
        }
        else
            //Category B
        {
            sibling =  node -> parent -> left;
            //Case one
            if(sibling -> color == 1)
            {
                sibling -> color = 0;
                node -> parent  -> color = 1;
                right_rotate( node -> parent);
                sibling =  node -> parent -> left;
            }
            //Case two
            if(sibling -> left -> color == 0 && sibling -> right -> color == 0)
            {
                sibling -> color = 1;
                node = node ->parent;
            }
            //Case Three
            else if(sibling -> left -> color == 0)
            {
                sibling ->right -> color = 0;
                sibling ->color = 1;
                left_rotate(sibling);
                sibling = node ->parent -> left;
            }

            //Case four
            sibling -> color = node -> parent -> color;
            node -> parent -> color = 0;
            sibling -> left -> color = 0;
            right_rotate(node -> parent);
            node = root;
        }
    }

    node -> color = 0;
}

/* Handles cases of deleting a red Node and calls delete_fixup to handle cases of deleting a black Node.
* Returns: false if the tree is empty or the key not exist, otherwise returns true.
* @pointer node is the deleted node, @pointer succ is the successor that will replace it.
* This function makes sure all cases assign a parent to the child node when it's equal TNULL.
*/
bool RBTree::rb_delete(int key)
{
    RBNode* node {search_helper(key)};
    if (node == TNULL)
    {
        return false;
    }

    RBNode* child;
    RBNode* succ {node};
    bool succ_original_color {succ -> color};

    if (node -> left == TNULL)
    {
        child = node -> right;
        rb_transplant(node, child);
    }
    else if(node -> right == TNULL)
    {
        child = node -> left;
        rb_transplant(node, child);
    }
    else
    {
        succ = min_helper(node -> right);
        succ_original_color = succ -> color;
        child = succ -> right;
        if(succ -> parent == node)
        {
            child -> parent = succ;
        }
        else
        {
            rb_transplant (succ, child);
            succ -> right = node -> right;
            succ -> right -> parent = succ;
        }

        rb_transplant(node, succ);
        succ -> left = node -> left;
        succ -> left -> parent = succ;
        succ -> color = node -> color;
    }

    delete node;
    --size;
    if (succ_original_color == 0)
    {
        delete_fixup(child);
    }
    return true;
}

void RBTree::left_rotate (RBNode* x)
{
    RBNode* y {x -> right};
    x -> right  = y-> left;
    if(y -> left != TNULL)
        y -> left -> parent = x;
    y -> parent = x -> parent;
    if(x -> parent == TNULL)
    {
        root = y;
    }
    else if(x == x -> parent -> left)
    {
        x -> parent -> left = y;
    }
    else
    {
        x -> parent -> right = y;
    }

    y -> left = x;
    x -> parent = y;
}

void RBTree::right_rotate (RBNode* x)
{
    RBNode* y {x -> left};
    x -> left  = y-> right;
    if(y -> right != TNULL)
        y -> right -> parent = x;
    y -> parent = x -> parent;
    if(x -> parent == TNULL)
    {
        root = y;
    }
    else if(x == x -> parent -> left)
    {
        x -> parent -> left = y;
    }
    else
    {
        x -> parent -> right = y;
    }

    y -> right = x;
    x -> parent = y;
}

int RBTree::rb_length () const
{
    return size;
}

bool RBTree::isEmpty() const
{
    if (root == TNULL)
        return true;
    return false;
}

bool RBTree::isFull() const
{
    return false;
}

int  RBTree::rb_minimum () const
{
    RBNode* min_node = min_helper(root);
    if(min_node != TNULL)
        return min_node -> key;
    return 0;
}

RBNode* RBTree::min_helper (RBNode* node) const
{
    if(node == TNULL)
        return TNULL;
    while(node-> left != TNULL)
    {
        node = node-> left;
    }
    return node;
}

int  RBTree::rb_maximum () const
{
    RBNode* max_node = max_helper(root);
    if(max_node != TNULL)
        return max_node -> key;
    return 0;
}

RBNode* RBTree::max_helper (RBNode* node) const
{
    if(node == TNULL)
        return TNULL;
    while(node-> right != TNULL)
    {
        node = node-> right;
    }

    return  node;
}

int RBTree::rb_successor (int key) const
{

    RBNode* node = search_helper(key);

    //key not exist.
    if (node == TNULL)
    {
        return 0;
    }

    RBNode* successor_ptr = successor_helper(node);
    if (successor_ptr != TNULL)
        return successor_ptr -> key;

    //maximum key does not have a successor.
    return 0;
}


RBNode* RBTree::successor_helper(RBNode* node) const
{
    if(node -> right != TNULL)
        return min_helper(node -> right);

    RBNode* p {node -> parent};

    //Notice short circuit.
    //This loop will terminate if it's a maximum node, there is no successor, or the successor is found.
    //Parent of root is TNULL.
    while(p != TNULL && node == p->right )
    {
        node = p;
        p = p -> parent;
    }
    return p;
}

int RBTree::rb_predecessor (int key) const
{
    RBNode* node = search_helper(key);

    //key not exist.
    if (node == TNULL)
    {
        return 0;
    }

    RBNode* predecessor_ptr = predecessor_helper(node);
    if (predecessor_ptr != TNULL)
        return predecessor_ptr -> key;

    return 0;
}


RBNode* RBTree::predecessor_helper(RBNode* node) const
{
    if(node -> left != TNULL)
        return max_helper(node -> left);

    RBNode* p {node -> parent};

    //Notice short circuit.
    //This loop will terminate if it's a minimum node, there is no predecessor, or the predecessor is found.
    //Parent of root is TNULL.
    while(p != TNULL && node == p-> left )
    {
        node = p;
        p = p -> parent;
    }
    return p;
}

//If not exist or tree is empty, returns TNULL.
//else returns pointer to the node found.
RBNode* RBTree::search_helper(int key) const
{
    RBNode* current{root};
    int node_key;
    while(current != TNULL)
    {
        node_key = current -> key;
        if (key == node_key)
        {
            return current;
        }
        else if(key < node_key)
        {
            current = current -> left;
        }
        else
        {
            current = current -> right;
        }
    }
    return current;
}

bool RBTree::rb_search (int key) const
{
    RBNode* node {search_helper(key)};

    if(node != TNULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RBTree::rb_inOrder (void (*func)(int)) const
{
    inOrder_helper(root, func);
}

void RBTree::rb_preOrder (void (*func)(int)) const
{
    preOrder_helper(root, func);
}
void RBTree::rb_postOrder (void (*func)(int)) const
{
    postOrder_helper(root, func);
}

void RBTree::inOrder_helper (RBNode* current, void (*func)(int) ) const
{
    if (current != TNULL)
    {
        inOrder_helper (current -> left, func);
        (*func) (current -> key);
        inOrder_helper(current -> right, func);
    }
}

void RBTree::preOrder_helper(RBNode* current, void (*func)(int)) const
{
    if (current != TNULL)
    {
        (*func) (current -> key);
        preOrder_helper (current -> left, func);
        preOrder_helper(current -> right, func);
    }
}
void RBTree::postOrder_helper(RBNode* current,void (*func)(int)) const
{
    if (current != TNULL)
    {
        postOrder_helper (current -> left, func);
        postOrder_helper (current -> right, func);
        (*func) (current -> key);
    }

}

//returns a pointer to the root of the new tree.
//We send otherTreeTNULL to avoid shallow copy.
RBNode* RBTree::rb_clone(RBNode* otherTreeNode, RBNode* otherTreeTNULL,RBNode* parent)
{
    if(otherTreeNode == otherTreeTNULL)
        return this ->TNULL;

    RBNode* cloneNode  = new RBNode();
    cloneNode  -> key = otherTreeNode -> key;
    cloneNode  -> color = otherTreeNode -> color;
    cloneNode  -> left   = rb_clone(otherTreeNode -> left, otherTreeTNULL, cloneNode);
    if(cloneNode -> left != this ->TNULL)
    {
        cloneNode ->left -> parent = cloneNode;
    }
    cloneNode -> right = rb_clone (otherTreeNode -> right, otherTreeTNULL, cloneNode);
    if (cloneNode -> right != this ->TNULL)
    {
        cloneNode -> right -> parent = cloneNode;
    }

    return cloneNode;

}

//deep copy.
// In Copy Constructor and Assignment operator, it's very important to declare which TNULL Node you are using, to avoid shallow copy.
RBTree::RBTree (RBTree& otherTree)
{
    TNULL = new RBNode(0);
    TNULL -> left = nullptr;
    TNULL -> right = nullptr;
    TNULL -> color = 0;
    TNULL -> parent = nullptr;

    // If otherTree is empty.
    if(otherTree.root == otherTree.TNULL)
    {
        this -> root = this -> TNULL;
    }
    else
    {
        //This->TNULL value as an Argument is not going to be used in rb-clone.
        this -> root = rb_clone(otherTree.root, otherTree.TNULL, this -> TNULL);
        //This saves the recursive steps of assigning parent to new root in rb-clone.
        this -> root -> parent = this -> TNULL;
    }

    this->size = otherTree.size;
}

const RBTree& RBTree::operator=(const RBTree& otherTree)
{
    //Avoid self Assignment.
    if(this != &otherTree)
    {

        if (this -> root != this -> TNULL)
        {
            rb_destroy();
        }

        if (otherTree.root == otherTree.TNULL)
        {
            this -> root = this -> TNULL;
        }
        else
        {
            this -> root = rb_clone (otherTree.root, otherTree.TNULL, this -> TNULL);
            this -> root -> parent = this -> TNULL;
        }
    }

    this->size = otherTree.size;
    return *this;
}

//Deletes all nodes and assign root to TNULL.
//Does not delete TNULL Node.
void RBTree::rb_destroy()
{
    destroy_helper(this -> root);
    this -> root = this -> TNULL;
    this->size = 0;
}


void RBTree::destroy_helper (RBNode* &root)
{
    if (root != TNULL)
    {
        destroy_helper(root -> left);
        destroy_helper(root -> right);
        delete root;
    }

}

//Deletes all node Including TNULL and assigns root to nullptr.
RBTree::~RBTree()
{
    rb_destroy();
    this -> root = nullptr;
    delete (this -> TNULL);
}
