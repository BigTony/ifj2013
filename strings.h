// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// strings.h
// 
//
#ifndef STRINGS_H
#define STRINGS_H
#include "errors.h"
#define ALLOC_SIZE 8
char * allocString();
void freeString(char *s);
char * reAllocString(char *s,int length);
char * konkatenace(char*prvni,char*druhy);
#endif

