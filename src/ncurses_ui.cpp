#include "ncurses_ui.h"
#include <cstdlib>
#include <string>
#include <sstream>

NCursesUI::NCursesUI() : main_win(nullptr), task_win(nullptr), tree_win(nullptr) {}

NCursesUI::~NCursesUI() {
    if (main_win) delwin(main_win);
    if (task_win) delwin(task_win);
    if (tree_win) delwin(tree_win);
    endwin();
}

bool NCursesUI::init() {
    main_win = initscr();
    if (!main_win) return false;
    
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(100); // 非阻塞输入
    
    // 初始化颜色
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    
    // 创建窗口
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    task_win = newwin(max_y - 10, max_x / 2, 0, 0);
    tree_win = newwin(max_y - 10, max_x / 2, 0, max_x / 2);
    
    return true;
}

void NCursesUI::update(const Scheduler& scheduler) {
    werase(task_win);
    werase(tree_win);
    
    // 绘制任务列表
    draw_task_list(scheduler.get_tasks());
    
    // 绘制红黑树
    // RBTreeNode* root = scheduler.get_rbtree().get_root();
    const RBTreeNode* root = scheduler.get_rbtree().get_root();
    if (root) {
        wmove(tree_win, 0, 0);
        wprintw(tree_win, "CFS Red-Black Tree (min vruntime=%.2f):\n", root->key);
        draw_rbtree(root, 0, getmaxx(tree_win) / 2);
    }
    
    // 绘制标题
    wmove(task_win, 0, 0);
    wattron(task_win, A_BOLD);
    wprintw(task_win, "Tasks (ID | Type | Prio | VRuntime | Progress)");
    wattroff(task_win, A_BOLD);
    
    // 刷新窗口
    wrefresh(task_win);
    wrefresh(tree_win);
    
    // 主窗口状态
    mvprintw(LINES - 3, 0, "CFS Scheduler Simulator - Press 'q' to quit, '+' to add task");
    refresh();
}

void NCursesUI::draw_task_list(const std::vector<Task*>& tasks) {
    int line = 2;
    for (const auto& task : tasks) {
        if (line >= getmaxy(task_win) - 1) break;
        
        // 计算进度条
        float progress = (task->get_duration() > 0) 
            ? static_cast<float>(task->get_duration() - task->get_duration()) / task->get_duration() 
            : 0.0f;
        
        int bar_width = 20;
        int pos = bar_width * progress;
        
        // 绘制任务信息
        wmove(task_win, line, 0);
        wattron(task_win, COLOR_PAIR(task->is_completed() ? 4 : 1));
        wprintw(task_win, "%4d | %4s | %4d | %8.2f | [", 
               task->get_id(), 
               task->get_type_str().c_str(),
               task->get_priority(),
               task->get_vruntime());
        
        // 绘制进度条
        for (int i = 0; i < bar_width; ++i) {
            if (i < pos) waddch(task_win, ACS_CKBOARD);
            else waddch(task_win, ' ');
        }
        wprintw(task_win, "] %s", task->is_completed() ? "DONE" : "");
        wattroff(task_win, COLOR_PAIR(task->is_completed() ? 4 : 1));
        
        line++;
    }
}

void NCursesUI::draw_rbtree(const RBTreeNode* node, int depth, int offset) {
    if (!node) return;
    
    // 递归绘制右子树
    draw_rbtree(node->right, depth + 2, offset + 10);
    
    // 绘制当前节点
    wmove(tree_win, depth, offset);
    if (node->is_red) {
        wattron(tree_win, COLOR_PAIR(4)); // 红色节点
    } else {
        wattron(tree_win, COLOR_PAIR(2)); // 黑色节点
    }
    
    wprintw(tree_win, "%.2f", node->key);
    wattroff(tree_win, COLOR_PAIR(node->is_red ? 4 : 2));
    
    // 绘制连接线
    if (node->left) {
        mvwaddch(tree_win, depth + 1, offset - 1, ACS_ULCORNER);
        mvwhline(tree_win, depth + 1, offset - 1, ACS_HLINE, 2);
    }
    
    if (node->right) {
        mvwaddch(tree_win, depth + 1, offset + 5, ACS_URCORNER);
        mvwhline(tree_win, depth + 1, offset + 5, ACS_HLINE, 2);
    }
    
    // 递归绘制左子树
    draw_rbtree(node->left, depth + 2, offset - 10);
}

void NCursesUI::handle_input(Scheduler& scheduler) {
    int ch = getch();
    if (ch == 'q') {
        endwin();
        exit(0);
    } else if (ch == '+') {
        // 添加新任务
        static int task_id = 100;
        TaskType type = (rand() % 2 == 0) ? TaskType::CPU_BOUND : TaskType::IO_BOUND;
        int priority = -10 + rand() % 20; // 随机优先级
        int duration = 500 + rand() % 1500; // 随机持续时间
        
        Task* new_task = new Task(task_id++, type, priority, duration);
        scheduler.add_task(new_task);
    }
}