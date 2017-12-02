#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "grep.h"

using namespace std;

unsigned long win_cur_line = 0;
unsigned long data_cur_line = 0;
/**
 * @brief refresh the specific line data to the windown.
 * @param WEINDOW *win
 * @param unsigned long xWin
 * @param vector<match_files> mfv
 * @param unsigned long refresh_line
 */
void refresh_win(WINDOW *win, 
        unsigned long yWin, 
        unsigned long xWin, 
        vector<match_files> mfv, 
        unsigned long refresh_line)
{
    if (refresh_line > mfv.size() - 1) {
        throw runtime_error("The refresh line param is more than the size of mfv.");
    }
    unsigned long max_line = (yWin - 2) < mfv.size() ? yWin - 3 : mfv.size() - 1;
    unsigned long tmp_win_cur_line = 0;
    if (refresh_line < data_cur_line) {
        // UP
        if (win_cur_line == 0) {
            tmp_win_cur_line = 0;
        } else {
            tmp_win_cur_line = --win_cur_line;
        }
    } else if (refresh_line > data_cur_line) {
        // DOWN
        if (win_cur_line == max_line) {
            tmp_win_cur_line = win_cur_line;
        } else {
            tmp_win_cur_line = ++win_cur_line;
        }
    }
    unsigned long start_line = refresh_line - tmp_win_cur_line;

    for (unsigned long i = start_line; i < max_line + start_line + 1; ++i) {
        if (i - start_line == tmp_win_cur_line) {
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
        if (i - start_line == tmp_win_cur_line) {
            wattroff(win, A_REVERSE);
        }
    }
    wrefresh(win);
    data_cur_line = refresh_line;
    win_cur_line = tmp_win_cur_line;
    return;
}
