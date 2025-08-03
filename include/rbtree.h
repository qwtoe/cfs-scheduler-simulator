#pragma once

class Task;

// 红黑树节点
struct RBTreeNode {
    double key;         // 键值 (vruntime)
    Task* task;         // 关联的任务
    RBTreeNode* left;
    RBTreeNode* right;
    RBTreeNode* parent;
    bool is_red;        // 节点颜色 (true=红, false=黑)
    
    RBTreeNode(double k, Task* t)
        : key(k), task(t), left(nullptr), right(nullptr), 
          parent(nullptr), is_red(true) {}
};

// 简化版红黑树 (仅实现CFS所需操作)
class RBTree {
public:
    RBTree() : root(nullptr) {}
    ~RBTree();
    
    // 插入新节点
    void insert(double key, Task* task);
    
    // 删除最小节点 (CFS选择下一个任务)
    Task* remove_min();
    
    // 检查树是否为空
    bool empty() const { return root == nullptr; }
    
    // 获取根节点 (用于可视化)
    // 添加 const 版本的方法
    const RBTreeNode* get_root() const { return root; }
    RBTreeNode* get_root() { return root; } // 保留非 const 版本

private:
    RBTreeNode* root;
    
    // 辅助函数
    void rotate_left(RBTreeNode* node);
    void rotate_right(RBTreeNode* node);
    void fix_violation(RBTreeNode* node);
    void clear(RBTreeNode* node);
};