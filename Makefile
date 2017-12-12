CC=g++
CFLAGS=-c -std=c++0x -Wall
LDFLAGS=-g -pthread -lncurses

SOURCES=main.cpp files.cpp grep.cpp tui.cpp data.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ncgrep

all: $(SOURCES) $(EXECUTABLE)


# -lncurses needs to be at end off compilation command
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lncurses

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
