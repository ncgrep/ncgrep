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
string replace_all_distinct(string str, const string old_value, const string new_value);

#endif
