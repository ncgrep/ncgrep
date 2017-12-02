#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "files.h"
#include "grep.h"
#include "tui.h"
#include "data.h"

using namespace std;

int main(int argc, char ** argv)
{
    if (argc != 3) {
        cerr<<"Incorrect usage! grepx match_pattern file_path"<<endl;
        return -1;
    }

    // Get data
    vector<match_files> mfv;
    try {
        mfv = get_data(argv[2], argv[1]);
    } catch (runtime_error &e) {
        cerr<<e.what()<<endl;
        return 1;
    }

    // Ncurses initialization
    setlocale(LC_ALL,"");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0); // hiden the cursor

    // Print window
    unsigned long yMax, xMax, yWin, xWin;
    unsigned long cur_line = 0;
    getmaxyx(stdscr, yMax, xMax);
    yWin = long(yMax * 0.6);
    xWin = long(xMax * 0.8);
    WINDOW * win = newwin(yWin, xWin, (yMax - yWin) /2, (xMax - xWin) / 2);
    refresh();
    box(win, 0, 0);
    refresh_win(win, yWin, xWin, mfv, cur_line);

    // Keyboard input
    int c;
    bool do_continue = true;
    while (do_continue && (c = getch())) {
        switch (*keyname(c)) {
            case 'q':
                do_continue = false;
                break;
            case 'k':
                if (cur_line == 0) {
                    break;
                }
                refresh_win(win, yWin, xWin, mfv, --cur_line);
                break;
            case 'j':
                if (cur_line == mfv.size() - 1) {
                    break;
                }
                refresh_win(win, yWin, xWin, mfv, ++cur_line);
                break;
        }
    }

    endwin();
    return 0;
}
