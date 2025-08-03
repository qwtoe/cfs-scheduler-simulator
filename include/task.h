#pragma once
#include <string>

enum class TaskType {
    CPU_BOUND,
    IO_BOUND
};

class Task {
public:
    Task(int id, TaskType type, int priority, int duration_ms);
    
    // 执行任务一个时间片
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
    int id;                 // 任务ID
    TaskType type;          // 任务类型
    int priority;           // 优先级 (nice值)
    double vruntime;        // 虚拟运行时间
    int duration_ms;        // 总需要时间
    int executed_ms = 0;    // 已执行时间
    bool completed = false; // 是否完成

    // 根据优先级计算权重
    int get_weight() const;
};