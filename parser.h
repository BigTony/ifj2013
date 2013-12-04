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


#include "scaner.h"

/**
* Hlavickovy soubor pro syntakticky analyzator
*/
//hlavni funkce parseru
void parser(ProgramState *main);
//funkce na pridani do hashTbl
void add_hash_tbl(tHashTbl *hashTbl, int type, tokenValue value);

