CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -Werror -O3
LDFLAGS=-L./lib -lBuddy
INCLUDES=-I./include
TARGET=bin/main
SOURCES=src/main.cpp src/constraints.cpp 

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
