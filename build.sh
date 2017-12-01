#!/bin/bash

g++ -c files.cpp -o files.o
g++ -c grep.cpp -o grep.o
g++ -g main.cpp -o grepx files.o grep.o -lncurses
./grepx s .
