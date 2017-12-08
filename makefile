CC=g++
CFLAGS=-c -std=c++0x -Wall
LDFLAGS=-g -pthread -lncurses

SOURCES=main.cpp files.cpp grep.cpp tui.cpp data.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ncgrep

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
