#include "scheduler.h"

Scheduler::Scheduler()
{
    last_run = std::chrono::steady_clock::now();
}

void Scheduler::add_task(Task *task)
{
    tasks.push_back(task);
    rbtree.insert(task->get_vruntime(), task);
}

void Scheduler::schedule()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_run);

    if (elapsed.count() < TIME_SLICE)
    {
        return; // time slice not reached
    }

    if (rbtree.empty())
    {
        return; // no tasks to schedule
    }

    // select the task with the minimum vruntime
    Task *task = rbtree.remove_min();

    if (!task)
    {
        return;
    }

    // excute the task for the time slice
    task->execute(TIME_SLICE);

    // if the task is completed, remove it from the scheduler
    if (!task->is_completed())
    {
        rbtree.insert(task->get_vruntime(), task);
    }

    last_run = now;
}