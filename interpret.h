// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin - xseidl01
// Slunsky Tomas - xsluns01
// Skacel David - xskace12
// Skyva Petr - xskyva02
////////////////////////////////
// interpret.c
//
//
#ifndef INTEPRET_H
#define INTEPRET_H
#include "strings.h"
#include "ial.h"
#include "ilist.h"
#include "errors.h"
#include "scaner.h"
#include "adt.h"
#include "memory.h"

void tostring(item *item);
void toVARBOOL(item *item);
void todouble(item *item);
void tovarint(item *item);
void interpret (tHashTbl *global_htable, TList *L);

#endif

