# file: Makefile
# author: Giridhar Nair
# date: 04/21/2024
# purpose: CS3377
# description: This is the makefile for assignment 6

CC = g++
CFLAGS = -c -w
PROGRAM = TwoPipesTwoChildren TwoPipesThreeChildren
OBJS = TwoPipesTwoChildren.o TwoPipesThreeChildren.o

build : $(PROGRAM)

TwoPipesTwoChildren : TwoPipesTwoChildren.o
	$(CC) TwoPipesTwoChildren.o -o TwoPipesTwoChildren

TwoPipesTwoChildren.o : TwoPipesTwoChildren.cpp
	$(CC) $(CFLAGS) TwoPipesTwoChildren.cpp

TwoPipesThreeChildren : TwoPipesThreeChildren.o
	$(CC) TwoPipesThreeChildren.o -o TwoPipesThreeChildren

TwoPipesThreeChildren.o : TwoPipesThreeChildren.cpp
	$(CC) $(CFLAGS) TwoPipesThreeChildren.cpp

twopipes_twochildren :
	./TwoPipesTwoChildren

twopipes_threechildren : 
	./TwoPipesThreeChildren

clean :
	rm -f $(OBJS) $(PROGRAM) 
