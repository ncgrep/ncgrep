# Ncgrep
NCGREP, which is based on ncurses library to provide user interface, is a grep tool for searching text on target directory.

[![Join the chat at https://gitter.im/ncgrep/Lobby](https://badges.gitter.im/ncgrep/ncgrep.svg)](https://gitter.im/ncgrep/Lobby)

# Demo
![ncgrep demo](https://github.com/GenialX/assets/blob/master/github.com/ncgrep/ncgrep_demo_5.gif)

# Features
 - The matching results are presented in a graphical interface and provide interactivity.
 - Currently it supports files ending in .php, .h, .c, .cpp suffixes.

# Usages

```shell
ncgrep "grep" . 3
```

 - `.` character stands for the current dirname
 - `3` It represents the level of search directory grouping, the three level directory as a group for text search

After entering the interactive interface, the commands are illustrated as followed:
 - `KEY_UP`    up
 - `KEY_DOWN`  down
 - `KEY_ENTER` open file in vim
 - `q`         quit
 - `k`         up
 - `j`         down
 - `CTRL-u`    half page up
 - `CTRL-d`    half page down
 - `o`         open file in vim
 - `CTRL-e`    return to the group interface

# Install

Firstly, you should install the dependency library:
```shell
# MAC
brew install ncurses
# CentOS
sudo yum install ncurses ncurses-devel
# Ubutun
sudo apt-get install libncurses5 libncurses5-dev
```

Then, compile & link:
```shell
# Just tested on Mac, CentOS and Ubutun
make
```

# Contact

E-Mail to admin@ihuxu.com
