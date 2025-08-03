#pragma once
#include <ncurses.h>
#include "scheduler.h"

class NCursesUI {
public:
    NCursesUI();
    ~NCursesUI();
    
    // 初始化UI
    bool init();
    
    // 更新UI显示
    void update(const Scheduler& scheduler);
    
    // 处理用户输入
    void handle_input(Scheduler& scheduler);

private:
    WINDOW* main_win;
    WINDOW* task_win;
    WINDOW* tree_win;
    
    // 绘制红黑树
    void draw_rbtree(const RBTreeNode* node, int depth, int offset);
    
    // 绘制任务列表
    void draw_task_list(const std::vector<Task*>& tasks);
};