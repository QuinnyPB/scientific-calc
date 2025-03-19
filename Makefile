# Variables
# compiler version and flags
# -g for debugging, -Wall for all compiler warnings
CC = g++
CFLAGS = -g -Wall -std=c++20
# sources and object files
SOURCES = main.cpp lexer.cpp token.cpp errorqueue.cpp
OBJECTS = $(SOURCES:.cpp=.o) # converts .cpp -> .o
# target executable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) 
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)