#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "files.h"
#include "grep.h"

using namespace std;

void refresh_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_files> mfv, unsigned long cur_line);

int main(int argc, char ** argv)
{
    if (argc != 3) {
        cerr<<"Incorrect usage! grepx match_pattern file_path"<<endl;
        return -1;
    }

    // Get data
    vector<string> files = listdir(argv[2], 0);
    vector<match_files> mfv, tmp;
    for (unsigned long i = 0; i < files.size(); ++i) {
        //cout<<files[i]<<endl;
        try {
            tmp = match_pattern(files[i], argv[1]);
            mfv.insert(mfv.end(), tmp.begin(), tmp.end());
        } catch (runtime_error &e) {
            cerr<<e.what()<<endl;
            return 1;
        }
    }
    /*
    for (unsigned long j = 0; j < mfv.size(); ++j) {
        cout<<"filename:"<<mfv[j].filename<<endl;
        cout<<"line:"<<mfv[j].line<<endl;
        cout<<"content:"<<mfv[j].content<<endl<<endl;
    }
    return 0;
    */

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
        }
        switch (c) {
            case KEY_UP:
                --cur_line;
                refresh_win(win, yWin, xWin, mfv, cur_line);
                break;
            case KEY_DOWN:
                ++cur_line;
                refresh_win(win, yWin, xWin, mfv, cur_line);
                break;
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
        }
    }

    endwin();
    return 0;
}

void refresh_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_files> mfv, unsigned long cur_line) {
    unsigned long max_line = (yWin - 2) < mfv.size() ? yWin - 2 : mfv.size();
    unsigned long start_line = cur_line < (max_line / 2) ? 0 : cur_line - max_line / 2;
    for (unsigned long i = start_line; i < max_line + start_line; ++i) {
        if (cur_line == i) {
            wattron(win, A_REVERSE);
        }
        // @TODO repalce the tab to 4 space chars
        string text(mfv[i].filename + "+" + to_string(mfv[i].line) + " " + mfv[i].content.substr(0, xWin));
        string space;
        if (text.length() > xWin - 2) {
            text = text.substr(0, xWin - 2);
        } else if(text.length() < xWin - 2) {
            space = string(xWin - text.length() - 2, ' ');
        }
        mvwprintw(win, i - start_line + 1, 1, (text + space).c_str());
        if (cur_line == i) {
            wattroff(win, A_REVERSE);
        }
    }
    wrefresh(win);
    return;
}
