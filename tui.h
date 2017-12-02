#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "grep.h"

using namespace std;

void refresh_win(WINDOW *win, unsigned long yWin, unsigned long xWin, vector<match_files> mfv, unsigned long refresh_line);
