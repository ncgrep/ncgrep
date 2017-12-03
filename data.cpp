#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "files.h"
#include "grep.h"
#include "tui.h"

using namespace std;

vector<match_files> get_data(const char *dirname, const char *parttern)
{
    vector<string> files = listdir(dirname, LIST_DIR_DEFAULT_LEVEL, GET_DIRS_MODE_RECURSIVE);
    vector<match_files> mfv, tmp;
    for (unsigned long i = 0; i < files.size(); ++i) {
        tmp = match_pattern(files[i], parttern);
        mfv.insert(mfv.end(), tmp.begin(), tmp.end());
    }
    return mfv;
}
