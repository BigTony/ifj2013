RM=rm
CC=gcc
CFLAGS= -Wall -std=c99 -O3 -pedantic
BIN=xifj13


#SOURCE= errors.o ilist.o strings.o scaner.o ial.o adt.o memory.o vyrazy.o vestavenefunkce.o parser.o main.o
SOURCE= errors.o ilist.o adt.o strings.o scaner.o ial.o memory.o vyrazy.o vestavenefunkce.o interpret.o parser.o main.o
ALL: $(SOURCE)
	$(CC) $(CFLAGS) -o $(BIN) $(SOURCE)

clean:
	$(RM)   *.o
