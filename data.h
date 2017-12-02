#ifndef DATA_H
#define DATA_H
#endif
#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "files.h"
#include "grep.h"
#include "tui.h"

using namespace std;

vector<match_files> get_data(const char *dirname, const char *parttern);
