# -*- Makefile -*-
SHELL=/bin/sh

CC=gcc

dirR = $(PWD)/..
dirI = $(dirR)/include
dirL = $(dirR)/src
dirS = $(dirR)/Obj-serial
dirT = $(dirR)/tests

CFLAGS = -Wall -Wextra -g -O3 -ffast-math -fomit-frame-pointer -I$(dirI) -I$(dirT)
LDLIBS = -lm -Wl,-rpath,$(dirL) -Wl,-rpath,$(dirS) -L$(dirS) -lLJMD-LJ 


# VPATH=$(dirL)

OBJ = test.o

default: test

clean:
	rm -f *.mod *.o *.x $(OBJ) *.xyz *.dat


test: $(OBJ)
	$(CC) -o $@.x $< $(LDLIBS)

run: test
	$(dirT)/test.x < argon_3.inp

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# pre-requisite

test.o: myunittest.h
