#pragma once

class Task;

// red-black tree node
struct RBTreeNode
{
    double key; // key (vruntime)
    Task *task; // task of this node
    RBTreeNode *left;
    RBTreeNode *right;
    RBTreeNode *parent;
    bool is_red; // color of this node (true=red, false=black)

    RBTreeNode(double k, Task *t)
        : key(k), task(t), left(nullptr), right(nullptr),
          parent(nullptr), is_red(true) {}
};

// simple red-black tree class
class RBTree
{
public:
    RBTree() : root(nullptr) {}
    ~RBTree();

    // insert a new node with key and task
    void insert(double key, Task *task);

    // delete a node with the given key, and exclude the next task
    Task *remove_min();

    // check if the tree is empty
    bool empty() const { return root == nullptr; }

    // get the root node (for visualization)
    // add const version of the method
    const RBTreeNode *get_root() const { return root; }
    RBTreeNode *get_root() { return root; } // reserve the non-const version

private:
    RBTreeNode *root;

    // helper methods
    void rotate_left(RBTreeNode *node);
    void rotate_right(RBTreeNode *node);
    void fix_violation(RBTreeNode *node);
    void clear(RBTreeNode *node);
};