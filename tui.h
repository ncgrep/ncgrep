#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "grep.h"
#include "files.h"

#ifndef NCGREP_TUI_WIN_SHOW_NUM
#define NCGREP_TUI_WIN_SHOW_NUM 1
#endif

using namespace std;

void refresh_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_dirs> dirs, long cur_dir_index, vector<match_files> mfv, unsigned long refresh_line);
void refresh_dirs_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_dirs> dirs, long cur_dir_index, unsigned long refresh_line);
void refresh_space_win(WINDOW *win, unsigned long yWin, unsigned long xWin, unsigned long start_line, unsigned long length);
