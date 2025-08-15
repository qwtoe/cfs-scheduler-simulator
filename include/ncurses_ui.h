#pragma once
#include <ncurses.h>
#include "scheduler.h"

class NCursesUI
{
public:
    NCursesUI();
    ~NCursesUI();

    bool init();

    void update(const Scheduler &scheduler);

    void handle_input(Scheduler &scheduler);

private:
    WINDOW *main_win;
    WINDOW *task_win;
    WINDOW *tree_win;

    void draw_rbtree(const RBTreeNode *node, int depth, int offset);
    void draw_task_list(const std::vector<Task *> &tasks);
};