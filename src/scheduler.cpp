#include "scheduler.h"

Scheduler::Scheduler() {
    last_run = std::chrono::steady_clock::now();
}

void Scheduler::add_task(Task* task) {
    tasks.push_back(task);
    rbtree.insert(task->get_vruntime(), task);
}

void Scheduler::schedule() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_run);
    
    if (elapsed.count() < TIME_SLICE) {
        return; // 未到时间片
    }
    
    if (rbtree.empty()) {
        return; // 无任务可调度
    }
    
    // 选择下一个任务 (最小vruntime)
    Task* task = rbtree.remove_min();
    
    if (!task) {
        return;
    }
    
    // 执行任务
    task->execute(TIME_SLICE);
    
    // 如果任务未完成，重新插入红黑树
    if (!task->is_completed()) {
        rbtree.insert(task->get_vruntime(), task);
    }
    
    last_run = now;
}