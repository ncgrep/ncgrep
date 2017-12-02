#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "grep.h"

#ifndef GREPX_TUI_WIN_SHOW_NUM
#define GREPX_TUI_WIN_SHOW_NUM 1
#endif

using namespace std;

void refresh_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_files> mfv, unsigned long refresh_line);
