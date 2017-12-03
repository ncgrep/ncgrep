#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <chrono>

#include "files.h"
#include "grep.h"
#include "tui.h"
#include "data.h"

using namespace std;

void init_screen();
void listen_keyboard();
void dispose_data();
void print_status_line(string msg);
unsigned long yMax, xMax, yWin, xWin;
unsigned long cur_line = 0;
WINDOW * win;
vector<match_files> mfv;
char *dirname;
char *parttern;
int group_level;

int main(int argc, char ** argv)
{
    if (argc < 3) {
        cerr<<"Incorrect usage! ncgrep match_pattern file_path [search_group_level]"<<endl;
        return -1;
    }
    dirname = argv[2];
    parttern = argv[1];
    if (argc == 4) {
        group_level = atoi(argv[3]);
    } else {
        group_level = 1;
    }

    // Init screen
    init_screen();

    // Print window
    refresh_win(win, yWin, xWin, mfv, cur_line);

    // Dispose data
    thread sub_thread(dispose_data);

    // Keyboard input
    listen_keyboard();

    sub_thread.join();
    endwin();
    return 0;
}

void init_screen()
{
    // Ncurses initialization
    setlocale(LC_ALL,"");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0); // hiden the cursor
    getmaxyx(stdscr, yMax, xMax);
    yWin = long(yMax * 0.6);
    xWin = long(xMax * 0.8);
    win = newwin(yWin, xWin, (yMax - yWin) /2, (xMax - xWin) / 2);
    box(win, 0, 0);
    refresh();
}

void listen_keyboard() {
    int c;
    bool do_continue = true;
    while (do_continue && (c = getch())) {
        switch (c) {
            case 10:
                string cmd = "vim " + mfv[cur_line].filename + " +" + to_string(mfv[cur_line].line);
                system(cmd.c_str());
                endwin();
                init_screen();
                refresh_win(win, yWin, xWin, mfv, cur_line);
                break;
        }

        switch (*keyname(c)) {
            case 'q':
                do_continue = false;
                break;
            case 'k':
                if (cur_line == 0) {
                    break;
                }
                refresh_win(win, yWin, xWin, mfv, --cur_line);
                break;
            case 'j':
                if (cur_line == mfv.size() - 1) {
                    break;
                }
                refresh_win(win, yWin, xWin, mfv, ++cur_line);
                break;
            case 'o':
                string cmd = "vim " + mfv[cur_line].filename + " +" + to_string(mfv[cur_line].line);
                system(cmd.c_str());
                endwin();
                init_screen();
                refresh_win(win, yWin, xWin, mfv, cur_line);
                break;
        }
    }
}

void dispose_data() {
        print_status_line("loadding...");
        vector<match_dirs> dirs;
        vector<string> files_tmp;
        vector<match_files> mfv_tmp;
        dirs = getdirs(dirname, 0, group_level);
        unsigned long dirs_count = dirs.size();
        unsigned long files_count;
        // FOR GROUPs
        for (unsigned long i = 0; i < dirs_count; ++i) {
            print_status_line("loadding " + to_string(int(((i * 1.0 + 1) / dirs_count) * 100)) + "%%...");
            files_tmp = listdir(dirs[i].dirname, group_level, dirs[i].mode);
            // FOR FILEs
            files_count = files_tmp.size();
            for (unsigned long j = 0; j < files_count; ++j) {
                print_status_line("loadding " + to_string(int(((i * 1.0 + 1) / dirs_count) * 100)) + "%%... sub process "
                        + to_string(int(((j * 1.0 + 1) / files_count) * 100)) + "%%...");
                try {
                    mfv_tmp = match_pattern(files_tmp[j], parttern);
                    mfv.insert(mfv.end(), mfv_tmp.begin(), mfv_tmp.end());
                } catch (runtime_error &e) {
                    continue;
                }
            }
            // ONE GROUP RESULTS
            refresh_win(win, yWin, xWin, mfv, cur_line);
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

    return;
}

void print_status_line(string msg) {
    mvprintw(yMax - 1, 0, msg.c_str());
    refresh();
    return;
}
