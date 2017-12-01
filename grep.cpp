#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "grep.h"

using namespace std;

vector<match_files> match_pattern(string file, string parttern) 
{ 
    char buffer[1024];
    string tmp;
    ifstream in(file);
    int line = 0;
    vector<match_files> mfv;
    if (!in.is_open())
    {
        cout <<"Open file failed."<<endl;
    }
    while (!in.eof())
    {
        ++line;
        in.getline(buffer, 1024);
        tmp.assign(buffer);
        if (tmp.find(parttern) != tmp.npos) {
            match_files mf;
            mf.line = line;
            mf.filename = file;
            mf.content = tmp;
            mfv.push_back(mf);
        }
    }
    in.close();

    return mfv;
} 
