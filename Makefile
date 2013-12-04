RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -O3 -pedantic
BIN=kokotihlava

SOURCE= errors.o ilist.o adt.o strings.o vestavenefunkce.o vyrazy.o ial.o interpret.o parser.o scaner.o main.o

ALL: $(SOURCE)
	$(CC) $(CFLAGS) -o $(BIN) $(SOURCE)

clean:
	$(RM)   *.o
