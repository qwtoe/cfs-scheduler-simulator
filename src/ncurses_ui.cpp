#include "ncurses_ui.h"
#include <cstdlib>
#include <string>
#include <sstream>

NCursesUI::NCursesUI() : main_win(nullptr), task_win(nullptr), tree_win(nullptr) {}

NCursesUI::~NCursesUI()
{
    if (main_win)
        delwin(main_win);
    if (task_win)
        delwin(task_win);
    if (tree_win)
        delwin(tree_win);
    endwin();
}

bool NCursesUI::init()
{
    main_win = initscr();
    if (!main_win)
        return false;

    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(100); // non-blocking getch with 100ms delay

    // initialize color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    // create sub-windows
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    task_win = newwin(max_y - 10, max_x / 2, 0, 0);
    tree_win = newwin(max_y - 10, max_x / 2, 0, max_x / 2);

    return true;
}

void NCursesUI::update(const Scheduler &scheduler)
{
    werase(task_win);
    werase(tree_win);

    draw_task_list(scheduler.get_tasks());

    // draw the red-black tree
    const RBTreeNode *root = scheduler.get_rbtree().get_root();
    if (root)
    {
        wmove(tree_win, 0, 0);
        wprintw(tree_win, "CFS Red-Black Tree (min vruntime=%.2f):\n", root->key);
        draw_rbtree(root, 0, getmaxx(tree_win) / 2);
    }

    // draw headers
    wmove(task_win, 0, 0);
    wattron(task_win, A_BOLD);
    wprintw(task_win, "Tasks (ID | Type | Prio | VRuntime | Progress)");
    wattroff(task_win, A_BOLD);

    // 刷新窗口
    wrefresh(task_win);
    wrefresh(tree_win);

    // main window instructions
    mvprintw(LINES - 3, 0, "CFS Scheduler Simulator - Press 'q' to quit, '+' to add task");
    refresh();
}

void NCursesUI::draw_task_list(const std::vector<Task *> &tasks)
{
    int line = 2;
    for (const auto &task : tasks)
    {
        if (line >= getmaxy(task_win) - 1)
            break;

        // calculate progress
        float progress = (task->get_duration() > 0)
                             ? static_cast<float>(task->get_duration() - task->get_duration()) / task->get_duration()
                             : 0.0f;

        int bar_width = 20;
        int pos = bar_width * progress;

        // draw task info
        wmove(task_win, line, 0);
        wattron(task_win, COLOR_PAIR(task->is_completed() ? 4 : 1));
        wprintw(task_win, "%4d | %4s | %4d | %8.2f | [",
                task->get_id(),
                task->get_type_str().c_str(),
                task->get_priority(),
                task->get_vruntime());

        // draw progress bar
        for (int i = 0; i < bar_width; ++i)
        {
            if (i < pos)
                waddch(task_win, ACS_CKBOARD);
            else
                waddch(task_win, ' ');
        }
        wprintw(task_win, "] %s", task->is_completed() ? "DONE" : "");
        wattroff(task_win, COLOR_PAIR(task->is_completed() ? 4 : 1));

        line++;
    }
}

void NCursesUI::draw_rbtree(const RBTreeNode *node, int depth, int offset)
{
    if (!node)
        return;

    // draw right subtree
    draw_rbtree(node->right, depth + 2, offset + 10);

    // draw current node
    wmove(tree_win, depth, offset);
    if (node->is_red)
    {
        wattron(tree_win, COLOR_PAIR(4)); // red node
    }
    else
    {
        wattron(tree_win, COLOR_PAIR(2)); // black node
    }

    wprintw(tree_win, "%.2f", node->key);
    wattroff(tree_win, COLOR_PAIR(node->is_red ? 4 : 2));

    // draw edges to children
    if (node->left)
    {
        mvwaddch(tree_win, depth + 1, offset - 1, ACS_ULCORNER);
        mvwhline(tree_win, depth + 1, offset - 1, ACS_HLINE, 2);
    }

    if (node->right)
    {
        mvwaddch(tree_win, depth + 1, offset + 5, ACS_URCORNER);
        mvwhline(tree_win, depth + 1, offset + 5, ACS_HLINE, 2);
    }

    // draw left subtree
    draw_rbtree(node->left, depth + 2, offset - 10);
}

void NCursesUI::handle_input(Scheduler &scheduler)
{
    int ch = getch();
    if (ch == 'q')
    {
        endwin();
        exit(0);
    }
    else if (ch == '+')
    {
        // add a new random task
        static int task_id = 100;
        TaskType type = (rand() % 2 == 0) ? TaskType::CPU_BOUND : TaskType::IO_BOUND;
        int priority = -10 + rand() % 20;   // random priority between -10 and 9
        int duration = 500 + rand() % 1500; // random duration between 500ms and 2000ms

        Task *new_task = new Task(task_id++, type, priority, duration);
        scheduler.add_task(new_task);
    }
}