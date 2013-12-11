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
#include "interpret.h"
/**
* Hlavickovy soubor pro syntakticky analyzator
*/
//hlavni funkce parseru
void parser(tPointers *ptrs);
//funkce na pridani do hashTbl
void add_hash_tbl(tHashTbl *hashTbl, int type, tokenValue value);
//hlavni funkce parseru
void classify();
// vyber spravneho postupu pro token
void main_classify();

void defFunction();
void defReturn();
void defWhile();
void defIf();
char* gen_id(char *counter);
void add_const_hashtbl(tHashTbl *hashTbl, int type,tokenValue value, char *id);
//funkce na vlozeni instrukce do ilist v ExEx
tokenValue CreateExInstruction(tokenValue value1,tokenValue value2,TIType op);
void callFunction(char* dest);
char* gen_param(char *counter);
#endif
