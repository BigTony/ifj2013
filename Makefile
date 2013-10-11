RM=rm
CC=gcc
#CFLAGS=-c -Wall -std=c99 -O3 -pedantic
CFLAGS=

ALL: errors.o ial.o interpret.o parser.o scaner.o main.o
	$(CC) $(CFLAGS) -o $(BIN) errors.o ial.o interpret.o parser.o scaner.o main.o

clean:
	$(RM)   *.o
