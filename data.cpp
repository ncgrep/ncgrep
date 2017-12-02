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
    // Get data
    vector<string> files = listdir(dirname, 0);
    vector<match_files> mfv, tmp;
    for (unsigned long i = 0; i < files.size(); ++i) {
        //cout<<files[i]<<endl;
        tmp = match_pattern(files[i], parttern);
        mfv.insert(mfv.end(), tmp.begin(), tmp.end());
    }
    /*
       for (unsigned long j = 0; j < mfv.size(); ++j) {
       cout<<"filename:"<<mfv[j].filename<<endl;
       cout<<"line:"<<mfv[j].line<<endl;
       cout<<"content:"<<mfv[j].content<<endl<<endl;
       }
       return 0;
       */
    return mfv;
}
