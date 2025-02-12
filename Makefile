# Variables
# COMPILER
CC = g++
# -g for debugging, -Wall for all compiler warnings
CFLAGS = -g -Wall
TARGET = main
OBJECTS = main.cpp lexer.cpp token.cpp errorstack.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) 
	

