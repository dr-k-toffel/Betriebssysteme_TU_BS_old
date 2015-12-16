# compiler
CC=gcc
# flags
CFLAGS=-std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall

# ruft hello auf
all: hello prompt parse

# ruft den compiler mit den flags auf und kompeliert hello.c zu hello.exe durch den aurruf -o
hello: hello.c
	$(CC) $(CFLAGS) -o hello.exe hello.c

# loescht alle .o und .exe daten
clean: 
	rm -f *.o *.exe

# ruft den compiler mit den flags auf und kompeliert prompt.c und shellutils zu prompt.exe durch den aurruf -o
prompt: prompt.c shellutils.h shellutils.c
	$(CC) $(CFLAGS) -o prompt.exe shellutils.c prompt.c

# ruft den compiler mit den flags auf und kompeliert parse.c und shellutils zu parse.exe durch den aurruf -o
parse: parse.c shellutils.h shellutils.c
	$(CC) $(CFLAGS) -o parse.exe shellutils.c parse.c
