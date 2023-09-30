//
// Created by daniel on 30.9.23.
//

#include <ncurses.h>
#include <cstring>
#include <vector>
#include <string>
#include <filesystem>
#include <sys/ioctl.h>
#include "StringUtils.h"
#include "Menu.h"


Menu::Menu(vector<Process> *processList) {
    this->processList = processList;
}

Process* Menu::show() {
    struct winsize w{};
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    int HEIGHT = 10;
    int WIDTH = (int) (w.ws_col);
    //height = (int)(w.ws_row);

    unsigned long highlight = 1;
    unsigned long choice = 0;
    int key;

    initscr();
    clear();
    noecho();
    cbreak();
    int start_x = 0;//(80 - WIDTH) / 2;
    int start_y = 1;//(24 - HEIGHT) / 2;

    WINDOW *menu_win = newwin(HEIGHT, WIDTH, start_y, start_x);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select Java process to kill.");
    refresh();
    print_menu(menu_win, highlight);
    while (true) {
        key = wgetch(menu_win);
        switch (key) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = processList->size();
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == processList->size())
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", key, key);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0)
            break;
    }
    Process *process = &processList->at(choice - 1);
    mvprintw(23, 0, "Kill %d? [Ctrl+C to abort]\n", process->pid);
    clrtoeol();
    getch();
    refresh();
    endwin();

    return process;
}

void Menu::print_menu(WINDOW *menu_win, unsigned long highlight) {
    int i, x, y;
    y = 2;
//    box(menu_win, 0, 0);
    print_header(menu_win, y - 1);
    for (i = 0; i < processList->size(); ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            print_process(menu_win, y, &processList->at(i));
            wattroff(menu_win, A_REVERSE);
        } else
            print_process(menu_win, y, &processList->at(i));
        ++y;
    }
    wrefresh(menu_win);
}

void Menu::print_process(WINDOW *menu_win, int y, Process *p) {
    mvwprintw(menu_win, y, 2, "%10d %s %s", p->pid, p->normalizedPort().c_str(), p->mainClass.c_str());
}

void Menu::print_header(WINDOW *menu_win, int y) {
    string ports = string(Process::largestPortSize - 5, ' ').append("PORTS");
    wattron(menu_win, A_UNDERLINE);
    wattron(menu_win, A_BOLD);
    mvwprintw(menu_win, y, 2, "%10s %s %s", "PID", ports.c_str(), "MAIN CLASS                                       ");
    wattroff(menu_win, A_BOLD);
    wattroff(menu_win, A_UNDERLINE);
}

