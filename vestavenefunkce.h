// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin - xseidl01
// Slunsky Tomas - xsluns01
// Skacel David - xskace12
// Skyva Petr - xskyva02
////////////////////////////////
// vestavenefunkce.h
//
//
#ifndef VEST_H
#define VEST_H
#include "ial.h"
#include "errors.h"
#include "scaner.h"
#include "strings.h"
#include "parser.h"
#endif

//vraci pozici v char[pozice]..cislo 0azDELKATEXTU
//vraci -1 nenajde-li
int find_string(char* text,char* word);

void vs_boolval(tHashTbl *tab,tHashTbl *NavrTab);
void vs_doubleval(tHashTbl *tab,tHashTbl *NavrTab);
void vs_intval(tHashTbl *tab,tHashTbl *NavrTab);
void vs_strval(tHashTbl *tab,tHashTbl *NavrTab);
void vs_get_string(tHashTbl *tab,tHashTbl *NavrTab);
void vs_put_string(tHashTbl *tab,tHashTbl *NavrTab);
void vs_strlen(tHashTbl *tab,tHashTbl *NavrTab);
void vs_get_substring(tHashTbl *tab,tHashTbl *NavrTab);
void vs_find_string(tHashTbl *tab,tHashTbl *NavrTab);
void vs_sort_string(tHashTbl *tab,tHashTbl *NavrTab);
void vs_define(tHashTbl *tab,tHashTbl *NavrTab);
