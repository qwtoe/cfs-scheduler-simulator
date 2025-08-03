#include "task.h"
#include <iostream>

// Linux 的优先级权重表 (简化版)
static const int prio_to_weight[40] = {
    /* -20 */ 88761, 71755, 56483, 46273, 36291,
    /* -15 */ 29154, 23254, 18705, 14949, 11916,
    /* -10 */ 9548,  7620,  6100,  4904,  3906,
    /*  -5 */ 3121,  2501,  1991,  1586,  1277,
    /*   0 */ 1024,  820,   655,   526,   423,
    /*   5 */ 335,   272,   215,   172,   137,
    /*  10 */ 110,   87,    70,    56,    45,
    /*  15 */ 36,    29,    23,    18,    15,
};

Task::Task(int id, TaskType type, int priority, int duration_ms)
    : id(id), type(type), priority(priority), duration_ms(duration_ms), vruntime(0) {}

void Task::execute(int time_slice) {
    if (completed) return;
    
    executed_ms += time_slice;
    
    // 更新 vruntime (CFS核心算法)
    int weight = get_weight();
    vruntime += (time_slice * 1024) / weight;  // NICE_0_LOAD = 1024
    
    // 检查任务是否完成
    if (executed_ms >= duration_ms) {
        completed = true;
    }
}

int Task::get_weight() const {
    // 将nice值映射到权重表索引 (-20~19 => 0~39)
    int index = priority + 20;
    if (index < 0) index = 0;
    if (index > 39) index = 39;
    return prio_to_weight[index];
}

std::string Task::get_type_str() const {
    return (type == TaskType::CPU_BOUND) ? "CPU" : "IO";
}