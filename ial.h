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

#pragma once
#include "errors.h"

#define ALLOC 67
typedef char* itemKey;



typedef struct item{
	int type;
	itemKey key;
	tokenValue data;
	struct item *nextItem;
}item;

typedef struct tHashTbl{
	item **tableItems; // Odkaz na tabulku itemu
	int size; // pocet itemu v tabulce
}tHashTbl;

int hashCode ( itemKey key );
void tableInit(tHashTbl *hash_table);
item* TblSearch (tHashTbl *tab, itemKey key);
void TblInsert (tHashTbl *tab, itemKey key,tokenValue data, int type);
tokenValue* TblReadData (tHashTbl *tab, itemKey key);
int TblReadType (tHashTbl *tab, itemKey key);
void TblDelete (tHashTbl *tab);
 
 





