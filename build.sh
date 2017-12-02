#!/bin/bash

g++ -c files.cpp -o files.o
g++ -c grep.cpp -o grep.o
g++ -c tui.cpp -o tui.o
g++ -c data.cpp -o data.o
g++ -g main.cpp -o grepx files.o grep.o tui.o data.o -lncurses
./grepx s .
