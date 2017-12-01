#!/bin/bash

g++ -c files.cpp -o files.o
g++ -c grep.cpp -o grep.o
g++ main.cpp -o main files.o grep.o
