#include "rbtree.h"
#include <iostream>

RBTree::~RBTree()
{
    clear(root);
}

void RBTree::clear(RBTreeNode *node)
{
    if (node)
    {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void RBTree::insert(double key, Task *task)
{
    RBTreeNode *new_node = new RBTreeNode(key, task);

    // standard BST insertion
    RBTreeNode *parent = nullptr;
    RBTreeNode *current = root;

    while (current)
    {
        parent = current;
        current = (key < current->key) ? current->left : current->right;
    }

    new_node->parent = parent;

    if (!parent)
    {
        root = new_node;
    }
    else if (key < parent->key)
    {
        parent->left = new_node;
    }
    else
    {
        parent->right = new_node;
    }

    // fix the red-black tree properties
    fix_violation(new_node);
}

Task *RBTree::remove_min()
{
    if (!root)
        return nullptr;

    // find the minimum node (leftmost)
    RBTreeNode *min_node = root;
    while (min_node->left)
    {
        min_node = min_node->left;
    }

    Task *task = min_node->task;

    // simplified: directly delete the node (in actual CFS, it would be reinserted)
    if (min_node == root)
    {
        root = root->right;
        if (root)
            root->parent = nullptr;
    }
    else
    {
        min_node->parent->left = min_node->right;
        if (min_node->right)
        {
            min_node->right->parent = min_node->parent;
        }
    }

    delete min_node;
    return task;
}

void RBTree::rotate_left(RBTreeNode *node)
{
    RBTreeNode *right_child = node->right;
    node->right = right_child->left;

    if (node->right)
    {
        node->right->parent = node;
    }

    right_child->parent = node->parent;

    if (!node->parent)
    {
        root = right_child;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = right_child;
    }
    else
    {
        node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

void RBTree::rotate_right(RBTreeNode *node)
{
    RBTreeNode *left_child = node->left;
    node->left = left_child->right;

    if (node->left)
    {
        node->left->parent = node;
    }

    left_child->parent = node->parent;

    if (!node->parent)
    {
        root = left_child;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = left_child;
    }
    else
    {
        node->parent->right = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
}

void RBTree::fix_violation(RBTreeNode *node)
{
    RBTreeNode *parent = nullptr;
    RBTreeNode *grand_parent = nullptr;

    while (node != root && node->is_red && node->parent->is_red)
    {
        parent = node->parent;
        grand_parent = parent->parent;

        if (parent == grand_parent->left)
        {
            RBTreeNode *uncle = grand_parent->right;

            if (uncle && uncle->is_red)
            {
                // Case 1: uncle is red
                grand_parent->is_red = true;
                parent->is_red = false;
                uncle->is_red = false;
                node = grand_parent;
            }
            else
            {
                if (node == parent->right)
                {
                    // Case 2: node is right child
                    rotate_left(parent);
                    node = parent;
                    parent = node->parent;
                }

                // Case 3: node is left child
                rotate_right(grand_parent);
                std::swap(parent->is_red, grand_parent->is_red);
                node = parent;
            }
        }
        else
        {
            // mirror case
            RBTreeNode *uncle = grand_parent->left;

            if (uncle && uncle->is_red)
            {
                grand_parent->is_red = true;
                parent->is_red = false;
                uncle->is_red = false;
                node = grand_parent;
            }
            else
            {
                if (node == parent->left)
                {
                    rotate_right(parent);
                    node = parent;
                    parent = node->parent;
                }

                rotate_left(grand_parent);
                std::swap(parent->is_red, grand_parent->is_red);
                node = parent;
            }
        }
    }

    root->is_red = false;
}