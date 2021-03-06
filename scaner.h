// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// scaner.h
//
//
/**
* Hlavickovy soubor pro lexikalni analyzator
*/



#ifndef SCANER_H
#define SCANER_H
#include <stdio.h>
#include "errors.h"
/**
* Tabulka typu tokenu
*/

#define IDENTIFIKATOR 1
#define STRING 2
#define VARIABLE 3
#define VARINT 4
#define VARDOUBLE 5
#define VARBOOL 6
#define NIL 7   //null

// Znaky
#define ZAV_JEDN_L  10 // (
#define ZAV_JEDN_P  11 // )
#define ZAV_SLOZ_L  12 // {
#define ZAV_SLOZ_P  13 // }
#define STREDNIK	14 // ;
#define CARKA       15 // ,
// Operatory
#define PLUS		20 // +
#define MINUS		21 // -
#define TECKA		22 // .
#define MENSI		23 // <
#define VETSI		24 // >
#define MENSI_ROVNO	25 // <=
#define VETSI_ROVNO 26 // >=
#define ROVNO 		27 // ===
#define NEROVNO		28 // !==
#define PRIRAZENI   29 // =
#define KRAT        30 // *
#define DELENO		31 // /
//-------------------------
//Tabulka klicovych slov a konstant
//
// Klicova slova
#define IF			40
#define ELSE		41
#define WHILE		42
#define RETURN		43
#define FUNCTION 	44
// starty konce lol
#define START     60 // <?php
#define KONEC    61 // eof
//-------------------


//Alokace
#define BUFF        10 //Alok ci realok vstupniho stringu

/**
* Struktury
*/


typedef union {
    int varInt;
    double varDouble;
    char *varString;
    void *pointer;
}tokenValue;

typedef struct{
	int id;
	tokenValue value;
}Ttoken;



// Funkce
int getToken(FILE *fp,Ttoken *token);

int getToken_test(FILE *fp,Ttoken *token);

#endif


