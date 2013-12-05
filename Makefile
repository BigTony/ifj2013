RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -O3 -pedantic
BIN=kokotihlava

SOURCE= errors.o ilist.o adt.o strings.o scaner.o ial.o parser.o vestavenefunkce.o interpret.o memory.o vyrazy.o main.o

ALL: $(SOURCE)
	$(CC) $(CFLAGS) -o $(BIN) $(SOURCE)

clean:
	$(RM)   *.o
