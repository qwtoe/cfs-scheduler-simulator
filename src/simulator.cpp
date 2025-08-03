#include "scheduler.h"
#include "ncurses_ui.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <iostream>

int main() {
    std::srand(std::time(nullptr));
    
    // 初始化调度器和UI
    Scheduler scheduler;
    NCursesUI ui;
    
    if (!ui.init()) {
        std::cerr << "Failed to initialize ncurses!" << std::endl;
        return 1;
    }
    
    // 创建初始任务
    scheduler.add_task(new Task(1, TaskType::CPU_BOUND, 0, 2000));
    scheduler.add_task(new Task(2, TaskType::IO_BOUND, 5, 1500));
    scheduler.add_task(new Task(3, TaskType::CPU_BOUND, -5, 1800));
    
    // 主循环
    while (true) {
        // 处理输入
        ui.handle_input(scheduler);
        
        // 执行调度
        scheduler.schedule();
        
        // 更新UI
        ui.update(scheduler);
        
        // 控制刷新率
        usleep(50 * 1000); // 50ms
    }
    
    return 0;
}