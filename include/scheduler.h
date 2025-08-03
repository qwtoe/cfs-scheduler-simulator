#pragma once
#include "rbtree.h"
#include "task.h"
#include <vector>
#include <chrono>

class Scheduler {
public:
    Scheduler();
    
    // 添加新任务
    void add_task(Task* task);
    
    // 执行调度循环
    void schedule();
    
    // 获取所有任务
    const std::vector<Task*>& get_tasks() const { return tasks; }
    
    // 获取红黑树
    const RBTree& get_rbtree() const { return rbtree; }
    RBTree& get_rbtree() { return rbtree; }

private:
    RBTree rbtree;                   // CFS 红黑树
    std::vector<Task*> tasks;         // 所有任务
    std::chrono::steady_clock::time_point last_run;
    
    // 时间片大小 (毫秒)
    static const int TIME_SLICE = 50;
};