#pragma once
#include <string>

enum class TaskType
{
    CPU_BOUND,
    IO_BOUND
};

class Task
{
public:
    Task(int id, TaskType type, int priority, int duration_ms);

    // excute a task for a given time slice
    void execute(int time_slice);

    // Getters
    int get_id() const { return id; }
    double get_vruntime() const { return vruntime; }
    TaskType get_type() const { return type; }
    int get_priority() const { return priority; }
    int get_duration() const { return duration_ms; }
    std::string get_type_str() const;
    bool is_completed() const { return completed; }

private:
    int id;                 // task ID
    TaskType type;          // task type (CPU_BOUND or IO_BOUND)
    int priority;           // priority
    double vruntime;        // virtual runtime
    int duration_ms;        // total duration in milliseconds
    int executed_ms = 0;    // time already executed in milliseconds
    bool completed = false; // is the task completed

    // get the weight of the task based on its priority
    int get_weight() const;
};