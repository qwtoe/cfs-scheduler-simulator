#include "scheduler.h"
#include "ncurses_ui.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

int main()
{
    std::srand(std::time(nullptr));

    // initialize scheduler and UI
    Scheduler scheduler;
    NCursesUI ui;

    if (!ui.init())
    {
        std::cerr << "Failed to initialize ncurses!" << std::endl;
        return 1;
    }

    // create some initial tasks
    scheduler.add_task(new Task(1, TaskType::CPU_BOUND, 0, 2000));
    scheduler.add_task(new Task(2, TaskType::IO_BOUND, 5, 1500));
    scheduler.add_task(new Task(3, TaskType::CPU_BOUND, -5, 1800));

    while (true)
    {
        ui.handle_input(scheduler);

        scheduler.schedule();

        ui.update(scheduler);

        // control the update rate
        usleep(50 * 1000); // 50ms
    }

    return 0;
}