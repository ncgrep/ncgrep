#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>

#include "files.h"
#include "grep.h"

using namespace std;

int main(int argc, char ** argv)
{
    if (argc != 3) {
        cerr<<"Incorrect usage! grepx match_pattern file_path"<<endl;
        return -1;
    }
    vector<string> files = listdir(argv[2], 0);
    vector<match_files> mfv, tmp;
    for (unsigned long i = 0; i < files.size(); ++i) {
        //cout<<files[i]<<endl;
        tmp = match_pattern(files[i], argv[1]);
        mfv.insert(mfv.end(), tmp.begin(), tmp.end());
    }
    /*
    for (int j = 0; j < mfv.size(); ++j) {
        cout<<"filename:"<<mfv[j].filename<<endl;
        cout<<"line:"<<mfv[j].line<<endl;
        cout<<"content:"<<mfv[j].content<<endl<<endl;
    }
    return 0;
    */

    setlocale(LC_ALL,"");
    initscr();
    cbreak();
    noecho();
    curs_set(0); // hiden the cursor

    int yMax, xMax, yWin, xWin;
    int curLine = 1;
    yWin = 20;
    xWin = 100;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * win = newwin(yWin, xWin, (yMax - yWin) /2, (xMax - xWin) / 2);
    refresh();
    box(win, 0, 0);

    // print line to win
    //wattron(win, A_REVERSE);
    for (int i = 0; i < yWin - 2; ++i) {
        int line = i + 1;
        if (curLine == line) {
            wattron(win, A_REVERSE);
        }
        string text(mfv[i].filename + " " + to_string(mfv[i].line) + " " + mfv[i].content.substr(0, 60));
        string space((xWin - text.length() - 2), ' ');
        mvwprintw(win, line, 1, (text + space).c_str());
        if (curLine == line) {
            wattroff(win, A_REVERSE);
        }
    }
    //wattroff(win, A_REVERSE);
    wrefresh(win);

    int c = getch();

    endwin();
}
