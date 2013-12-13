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





#ifndef IAL_H
#define IAL_H

#include "errors.h"
#include "scaner.h"
#define ALLOC 17


/*  // NEBUDU!! DoPiCI!! FURT!! LIST!!! DO SCANER.H!! dopcici!
typedef union {
    int varInt;
    double varDouble;
    char *varString;
    void *pointer;
}tokenValue;
*/

//typedef char* itemKey;
typedef struct item {
	int type;
	char* key;
	tokenValue data;
	struct item *nextItem;
}item;

typedef struct tHashTbl{
	item **tableItems; // Odkaz na tabulku itemu
	int size; // pocet itemu v tabulce
}tHashTbl;

//Knuth-Morris-Pratt
//void createTableKmp(char* patt, int pattLen, int** fail);
int getSubstringKmp(char* text,char* patt);
//Merge sort
void full_merge_sort(char*array);
int hashCode ( char* key );
void tableInit(tHashTbl **hash_table);
item* TblSearch (tHashTbl *tab, char* key);
void TblInsert (tHashTbl *tab, char* key,tokenValue data, int type);
tokenValue* TblReadData (tHashTbl *tab, char* key);
int TblReadType (tHashTbl *tab, char* key);
void TblDelete (tHashTbl *tab);
void TblPrint( tHashTbl* tab );
#endif






