RM=rm
CC=gcc
CFLAGS= -Wall -std=c99 -O3 -pedantic -g
BIN=kokotihlava

SOURCE= errors.o ilist.o adt.o strings.o scaner.o ial.o memory.o vyrazy.o vestavenefunkce.o parser.o main.o
#SOURCE= errors.o ilist.o adt.o strings.o scaner.o ial.o memory.o vyrazy.o vestavenefunkce.o interpret.o parser.o main.o

ALL: $(SOURCE)
	$(CC) $(CFLAGS) -o $(BIN) $(SOURCE)

clean:
	$(RM)   *.o
