# file: Makefile
# author: Giridhar Nair
# date: 04/21/2024
# purpose: CS3377
# description: This is the makefile for assignment 6

CC = g++
CFLAGS = -c -w
PROGRAM = TwoPipesTwoChildren
OBJS = TwoPipesTwoChildren.o

build : $(PROGRAM)

TwoPipesTwoChildren : TwoPipesTwoChildren.o
	$(CC) TwoPipesTwoChildren.o -o TwoPipesTwoChildren

TwoPipesTwoChildren.o : TwoPipesTwoChildren.cpp
	$(CC) $(CFLAGS) TwoPipesTwoChildren.cpp
run :
	./$(PROGRAM)
clean :
	rm -f $(OBJS) $(PROGRAM) 
