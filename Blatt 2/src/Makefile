CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall  
CC=gcc
.PHONY: all clean doc

all: myshell wait doc

myshell: myshell.c plist.c plist.h shellutils.c shellutils.h 
	$(CC) $(CFLAGS) -o $@ $^

wait: wait.c 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm *.o myshell wait 
	
doc:
	doxygen
