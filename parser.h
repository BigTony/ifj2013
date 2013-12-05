// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// interpret.c
// 
//


#ifndef PARSER_H
#define PARSER_H
#include "scaner.h"
#include "ilist.h"
#include "errors.h"
#include "memory.h"
#include "vyrazy.h"
/**
* Hlavickovy soubor pro syntakticky analyzator
*/
//hlavni funkce parseru
void parser(tPointers *ptrs);
//funkce na pridani do hashTbl
void add_hash_tbl(tHashTbl *hashTbl, int type, tokenValue value);
#endif
