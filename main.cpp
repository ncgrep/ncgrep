#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <thread>

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
long cur_dir_index = -1;
WINDOW * win;
vector<match_files> mfv;
vector<match_dirs> dirs, used_dirs;
char *dirname;
char *parttern;
int group_level;
bool do_moving = false;

int main(int argc, char ** argv)
{
    if (argc < 3) {
        cerr<<"Version: 0.0.3_beta\nIncorrect usage! ncgrep match_pattern file_path [search_group_level]"<<endl;
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
    refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);

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

const int CMD_NOOP           = 0;
const int CMD_OPEN           = 1;
const int CMD_GROUP          = 2;
const int CMD_UP             = 3;
const int CMD_DOWN           = 4;
const int CMD_QUIT           = 5;
const int CMD_PAGE_HALF_DOWN = 6;
const int CMD_PAGE_HALF_UP   = 7;

void listen_keyboard() {
    int c;
    int command = CMD_NOOP;

    bool do_continue = true;
    while (do_continue && (c = getch())) {
        // Get current key pressed.
        // Slightly complicate because of different handling of control keys and "standard" keys.
        switch (c) {
        case 4: // ctrl-d
            command = CMD_PAGE_HALF_DOWN;
            break;
        case 21: // ctrl-u
            command = CMD_PAGE_HALF_UP;
            break;
        case 5: // ctrl-e
            command = CMD_GROUP;
            break;
        case 10:
            command = CMD_OPEN;
            break;
        case KEY_UP:
            command = CMD_UP;
            break;
        case KEY_DOWN:
            command = CMD_DOWN;
            break;
        }
        switch (*keyname(c)) {
        case 'q':
            command = CMD_QUIT;
            break;
        case 'k':
            command = CMD_UP;
            break;
        case 'j':
            command = CMD_DOWN;
            break;
        case 'o':
            command = CMD_OPEN;
            break;
        }

        unsigned long min = 0;
        unsigned long max = used_dirs.size() == 0 ? 0 : used_dirs.size() - 1;
        unsigned long last_win_cur_line = get_last_win_cur_line();
        unsigned long win_cur_line;
        if (cur_dir_index != -1) {
            min = used_dirs[cur_dir_index].start;
            max = used_dirs[cur_dir_index].start + used_dirs[cur_dir_index].length == 0 ? 0 : used_dirs[cur_dir_index].start + used_dirs[cur_dir_index].length - 1;
        }
        switch (command) {
        case CMD_QUIT:
            do_continue = false;
            break;
        case CMD_UP:
            do_moving = true;
            if (cur_line == min) {
                do_moving = false;
                break;
            }
            refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, --cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);
            do_moving = false;
            break;
        case CMD_DOWN:
            do_moving = true;
            if (cur_line == max) {
                do_moving = false;
                break;
            }
            refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, ++cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);
            do_moving = false;
            break;
        case CMD_PAGE_HALF_UP:
            do_moving = true;
            if (cur_line == min) {
                do_moving = false;
                break;
            } else {
                win_cur_line = last_win_cur_line + 1 <= (yWin - 2) / 2 ? 0 : last_win_cur_line - (yWin - 2) / 2;
                set_last_win_cur_line(win_cur_line);
                cur_line = cur_line < (yWin - 2) / 2 + min ? min : cur_line - (yWin - 2) / 2;
            }
            refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_NOT_MOVE_WIN_LINE);
            do_moving = false;
            break;
        case CMD_PAGE_HALF_DOWN:
            do_moving = true;
            if (cur_line == max) {
                do_moving = false;
                break;
            } else {
                win_cur_line = last_win_cur_line + 1 + (yWin - 2) / 2 > yWin - 2 ? yWin - 3 : last_win_cur_line + (yWin - 2) / 2;
                set_last_win_cur_line(win_cur_line);
                cur_line = cur_line + (yWin - 2) / 2 > max ? max : cur_line + (yWin - 2) / 2;
            }
            refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_NOT_MOVE_WIN_LINE);
            do_moving = false;
            break;
        case CMD_OPEN:
            if (cur_dir_index == -1) {
                cur_dir_index = cur_line;
                cur_line = used_dirs[cur_dir_index].start;
                refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);
            } else {
                string cmd = "vim " + mfv[cur_line].filename + " +" + to_string(mfv[cur_line].line);
                system(cmd.c_str());
                endwin();
                init_screen();
                refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);
            }
            break;
        case CMD_GROUP:
            if (cur_dir_index == -1) {
                break;
            }
            cur_dir_index = -1;
            cur_line = 0;
            refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);
            break;
        }
        command = CMD_NOOP;
    }
}

void dispose_data() {
    print_status_line("loadding...");
    vector<string> files_tmp;
    vector<match_files> mfv_tmp;
    try {
        dirs = getdirs(dirname, 0, group_level);
    } catch (runtime_error &e) {
        cerr<<e.what()<<endl;
        return;
    }
    unsigned long dirs_count = dirs.size();
    unsigned long files_count;
    // FOR GROUPs
    for (unsigned long i = 0; i < dirs_count; ++i) {
        //print_status_line("loadding " + to_string(int(((i * 1.0 + 1) / dirs_count) * 100)) + "%%...");
        files_tmp = listdir(dirs[i].dirname, group_level, dirs[i].mode);
        // FOR FILEs
        files_count = files_tmp.size();
        dirs[i].start = mfv.size();
        for (unsigned long j = 0; j < files_count; ++j) {
            /*
            print_status_line("loadding " + to_string(int(((i * 1.0 + 1) / dirs_count) * 100)) + "%%... "
                    + "sub process " + to_string(int(((j * 1.0 + 1) / files_count) * 100)) + "%%... "
                    + dirs[i].dirname);
                    */
            try {
                mfv_tmp = match_pattern(files_tmp[j], parttern);
                mfv.insert(mfv.end(), mfv_tmp.begin(), mfv_tmp.end());
            } catch (runtime_error &e) {
                continue;
            }
        }
        dirs[i].length = mfv.size() - dirs[i].start;
        if (dirs[i].length > 0) {
            match_dirs md;
            md.dirname = dirs[i].dirname;
            md.start = dirs[i].start;
            md.length = dirs[i].length;
            used_dirs.push_back(md);
        }
        // ONE GROUP RESULTS
        if (cur_dir_index == -1) {
            while (do_moving == true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            refresh_win(win, yWin, xWin, used_dirs, cur_dir_index, mfv, cur_line, NCGREP_TUI_DO_MOVE_WIN_LINE);
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    print_status_line("loaded     ");

    return;
}

void print_status_line(string msg) {
    mvprintw(yMax - 1, 0, msg.c_str());
    refresh();
    return;
}
