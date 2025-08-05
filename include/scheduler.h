#pragma once
#include "rbtree.h"
#include "task.h"
#include <vector>
#include <chrono>

class Scheduler
{
public:
    Scheduler();

    // add a task to the scheduler
    void add_task(Task *task);

    // execute the scheduling logic
    void schedule();

    // get all tasks
    const std::vector<Task *> &get_tasks() const { return tasks; }

    // get the red-black tree
    const RBTree &get_rbtree() const { return rbtree; }
    RBTree &get_rbtree() { return rbtree; }

private:
    RBTree rbtree;             // CFS red-black tree
    std::vector<Task *> tasks; // all tasks in the scheduler
    std::chrono::steady_clock::time_point last_run;

    // time slice for each scheduling decision in milliseconds
    static const int TIME_SLICE = 50;
};