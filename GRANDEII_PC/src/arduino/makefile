CC=gcc -c
CFLAGS= -Wall -Wextra -MMD
LD=gcc
LDFLAGS=
LIB=-lm
LDLIBS=
PROGRAM=arduino_test
BUILDDIR=output

include $(wildcard .d)


all: main

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

main: $(PROGRAM).o arduino_test.o communication.o arduino_utils.o
	$(LD) $(LDFLAGS) $^ $(LIB) -o $@