#ifndef GREP_H
#define GREP_H

#include <string>

using namespace std;

struct match_files
{
    int line;
    string filename;
    string content;
};

vector<match_files> match_pattern(string file, string pattern) ;

#endif
