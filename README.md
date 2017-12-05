# Ncgrep
NCGREP, which is based on ncurses library to provide user interface, is a grep tool for searching text on target directory.

# Demo
![ncgrep demo](https://github.com/GenialX/assets/blob/master/github.com/grepx/ncgrep_demo_4.gif)

# Usages

```shell
ncgrep "grep" . 3
```

 - `.` character stands for the current dirname
 - `3` It represents the level of search directory grouping, the three level directory as a group for text search

After entering the interactive interface, the commands are illustrated as followed:
 - `q` quit
 - `j` down
 - `k` up
 - `o` open file in vim
 - `KEY_ENTER` open file in vim
 - `CTRL-e` return to the group interface

TO BE CONTINUED...
