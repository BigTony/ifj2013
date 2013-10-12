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

/**
* Hlavickovy soubor pro lexikalni analyzator
*/

/**
* Tabulka tokenu
*/
#define ID 0
#define VARINT 1
#define VARDOUBLE 2
// Znaky
#define ZAV_JEDN_L  10 // (
#define ZAV_JEDN_P  11 // )
#define ZAV_SLOZ_L  12 // {
#define ZAV_SLOZ_P  13 // }
#define STREDNIK	14 // ;
// Operatory
#define KRAT		22 // *
#define DELENO		23 // /
#define PLUS		20 // +
#define MINUS		21 // - 
#define TECKA		22 // .
#define MENSI		23 // <
#define VETSI		24 // >
#define MENSI_ROVNO	25 // <=
#define VETSI_ROVNO 26 // >=
#define ROVNO 		27 // ===
#define NEROVNO		28 // !==
// Klicova slova
#define IF			30 
#define ELSE		31
#define WHILE		32
#define RETURN		33
#define FUNCTION 	34
// Konstanty
#define NIL			40 // NULL

// Funkce
int getToken();

/**
* Struktury
*/
union Unie{
    int varInt;
    double varDouble;
    char *varString;
}tokenValue;

typedef struct{
	int id;
	tokenValue value;
}token;

