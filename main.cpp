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
        cerr<<"Incorrect usage! bin_name match_pattern file_path"<<endl;
        return -1;
    }
    vector<string> files = listdir(argv[2], 0);
    vector<match_files> mfv;
    for (int i = 0; i < files.size(); ++i) {
        mfv = match_pattern(files[i], argv[1]);
        for (int j = 0; j < mfv.size(); ++j) {
            cout<<"filename:"<<mfv[j].filename<<endl;
            cout<<"line:"<<mfv[j].line<<endl;
            cout<<"content:"<<mfv[j].content<<endl<<endl;
        }
    }
    return 0;

    initscr();
    cbreak();
    noecho();

    int yMax, xMax, yWin, xWin;
    yWin = 20;
    xWin = 100;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * win = newwin(yWin, xWin, (yMax - yWin) /2, (xMax - xWin) / 2);
    refresh();
    box(win, 0, 0);

    wattron(win, A_REVERSE);
    string text("Hello World");
    string space((xWin - text.length() - 3), ' ');
    mvwprintw(win, 1, 1, (text + space).c_str());
    wattroff(win, A_REVERSE);
    wrefresh(win);

    int c = getch();

    endwin();
}
