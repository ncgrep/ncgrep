#include <stdexcept>
#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "grep.h"
#include "tui.h"

using namespace std;

unsigned long win_cur_line = 0;
unsigned long data_cur_line = 0;
long last_cur_dir_index = 0;
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
        vector<match_dirs> dirs,
        long cur_dir_index,
        vector<match_files> mfv, 
        unsigned long refresh_line,
        bool do_move_win_line)
{
    if (cur_dir_index == -1) {
        refresh_dirs_win(win, yWin, xWin, dirs, cur_dir_index, refresh_line, do_move_win_line);
        return;
    }
    if (dirs[cur_dir_index].length == 0) {
        refresh_space_win(win, yWin, xWin, (unsigned long)0, yWin - 2);
        return;
    }
    if (refresh_line > dirs[cur_dir_index].start + dirs[cur_dir_index].length - 1) {
        throw runtime_error("The refresh line param is more than the length of current group mfv.");
    }
    if (refresh_line < dirs[cur_dir_index].start) {
        throw runtime_error("The refresh line param is less than the start of current group mfv.");
    }
    unsigned long max_line = (yWin - 2) < dirs[cur_dir_index].length ? yWin - 3 : dirs[cur_dir_index].length - 1;
    if (last_cur_dir_index != cur_dir_index) {
        win_cur_line = 0;
        data_cur_line = refresh_line;
    }
    unsigned long tmp_win_cur_line = win_cur_line;
    if (do_move_win_line) {
        if (refresh_line < data_cur_line) {
            // UP
            if (win_cur_line == 0) {
                tmp_win_cur_line = 0;
            } else {
                tmp_win_cur_line = win_cur_line == 0 ?  0 : --win_cur_line;
            }
        } else if (refresh_line > data_cur_line) {
            // DOWN
            if (win_cur_line == max_line) {
                tmp_win_cur_line = win_cur_line;
            } else {
                tmp_win_cur_line = ++win_cur_line;
            }
        }
    }
    unsigned long start_line = refresh_line <= tmp_win_cur_line ? 0 : refresh_line - tmp_win_cur_line;

    for (unsigned long i = start_line; i < max_line + start_line + 1; ++i) {
        if (i - start_line == tmp_win_cur_line) {
            wattron(win, A_REVERSE);
        }
        string text(mfv[i].filename + " +" + to_string(mfv[i].line) + " " + mfv[i].content.substr(0, xWin));
        if (NCGREP_TUI_WIN_SHOW_NUM == 1) {
            text = to_string(i + 1) + " " + text;
        }
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
    if (max_line < yWin - 3) {
        refresh_space_win(win, yWin, xWin, max_line + 1, yWin - 3 - max_line);
    }
    wrefresh(win);
    data_cur_line = refresh_line;
    win_cur_line = tmp_win_cur_line;
    last_cur_dir_index = cur_dir_index;
    return;
}


void refresh_dirs_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_dirs> dirs, long cur_dir_index, unsigned long refresh_line, bool do_move_win_line)
{
    if (refresh_line > dirs.size() - 1) {
        throw runtime_error("The refresh line param is more than the length of dirs.");
    }
    unsigned long max_line = (yWin - 2) < dirs.size() ? yWin - 3 : dirs.size() - 1;
    if (last_cur_dir_index != cur_dir_index) {
        win_cur_line = 0;
        data_cur_line = 0;
    }
    unsigned long tmp_win_cur_line = win_cur_line;
    if (do_move_win_line) {
        if (refresh_line < data_cur_line) {
            // UP
            if (win_cur_line == 0) {
                tmp_win_cur_line = 0;
            } else {
                tmp_win_cur_line = win_cur_line == 0 ?  0 : --win_cur_line;
            }
        } else if (refresh_line > data_cur_line) {
            // DOWN
            if (win_cur_line == max_line) {
                tmp_win_cur_line = win_cur_line;
            } else {
                tmp_win_cur_line = ++win_cur_line;
            }
        }
    }
    unsigned long start_line = refresh_line <= tmp_win_cur_line ? 0 : refresh_line - tmp_win_cur_line;

    for (unsigned long i = start_line; i < max_line + start_line + 1; ++i) {
        if (i - start_line == tmp_win_cur_line) {
            wattron(win, A_REVERSE);
        }
        string text(dirs[i].dirname);
        if (NCGREP_TUI_WIN_SHOW_NUM == 1) {
            text = to_string(i + 1) + " " + text;
        }
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
    if (max_line < yWin - 3) {
        refresh_space_win(win, yWin, xWin, max_line + 1, yWin - 3 - max_line);
    }
    wrefresh(win);
    data_cur_line = refresh_line;
    win_cur_line = tmp_win_cur_line;
    last_cur_dir_index = cur_dir_index;
    return;
    return;
}

void refresh_space_win(WINDOW *win, unsigned long yWin, unsigned long xWin, unsigned long start_line, unsigned long length)
{
    for (unsigned long i = start_line; i < start_line + length; ++i) {
        string space;
        space = string(xWin - 2, ' ');
        mvwprintw(win, i + 1, 1, space.c_str());
    }
    wrefresh(win);
    return;
}

unsigned long get_last_win_cur_line() {
    return win_cur_line;
}

void set_last_win_cur_line(unsigned long new_win_cur_line) {
    win_cur_line = new_win_cur_line;
}
