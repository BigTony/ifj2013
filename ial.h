// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// ial.c
// Iplementation sort and find functions
//
#include "errors.h"

#define ALLOC 67
typedef char* itemKey;

typedef union {
	int varInt;
	char *string;
	double varDouble;
	// int instruction;
}itemValue;

typedef struct item{
	int type;
	itemKey key;
	itemValue data;
	struct item *nextItem;
}item;

typedef struct {
	item **tableItems; // Odkaz na tabulku itemu
	int size; // pocet itemu v tabulce
}tHashTbl;



