# Ncgrep
NCGREP, which is based on ncurses library to provide user interface, is a grep tool for searching text on target directory.

# Demo
![ncgrep demo](https://github.com/GenialX/assets/blob/master/github.com/ncgrep/ncgrep_demo_5.gif)

# Usages

```shell
ncgrep "grep" . 3
```

 - `.` character stands for the current dirname
 - `3` It represents the level of search directory grouping, the three level directory as a group for text search

After entering the interactive interface, the commands are illustrated as followed:
 - `q` quit
 - `KEY_DOWN` down
 - `j` down
 - `k` up
 - `KEY_UP` up
 - `o` open file in vim
 - `KEY_ENTER` open file in vim
 - `CTRL-e` return to the group interface

# Install

Firstly, you should install the dependency library:
```shell
# MAC
brew install ncurses
# CentOS
yum install ncurses
```

Then, compile & link:
```shell
g++ -c files.cpp -o files.o
g++ -c grep.cpp -o grep.o
g++ -c tui.cpp -o tui.o
g++ -c data.cpp -o data.o
g++ -g main.cpp -o ncgrep files.o grep.o tui.o data.o -lncurses
```

TO BE CONTINUED...
