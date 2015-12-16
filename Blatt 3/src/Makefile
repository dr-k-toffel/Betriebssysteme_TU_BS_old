CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall 
CC=gcc
.PHONY: all clean doc

all: mymem

mymem: mymem.c test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm mymem *.o

doc:
	doxygen
