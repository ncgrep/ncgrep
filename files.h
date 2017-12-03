#ifndef FILES_H
#define FILES_H

#include <string>

#define GET_DIRS_MODE_FILE 0
#define GET_DIRS_MODE_RECURSIVE 1

#define LIST_DIR_DEFAULT_LEVEL 0

using namespace std;

struct match_dirs
{
    int mode;
    string dirname;
};

vector<string> listdir(string dirname, int lvl, int mode);
vector<match_dirs> getdirs(string dirname, int lvl, int dirname_lvl);
#endif
