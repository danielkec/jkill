//
// Created by daniel on 30.9.23.
//



#include <vector>
#include "Process.h"

class Menu {
    vector<Process> *processList;
    static void print_process(WINDOW *menu_win, int y, Process *p);
    static void print_header(WINDOW *menu_win, int y);
    void print_menu(WINDOW *menu_win, unsigned long highlight);


public:
    explicit Menu(vector<Process> *processList);
    Process* show();
};

