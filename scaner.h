git// Project interpret IFJ2013
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

#define ERROR -1 
#define ID 0       
#define VARINT 1
#define VARDOUBLE 2
#define VARSTRING 3 //"" 
// Znaky
#define ZAV_JEDN_L  10 // (
#define ZAV_JEDN_P  11 // )
#define ZAV_SLOZ_L  12 // {
#define ZAV_SLOZ_P  13 // }
#define STREDNIK	14 // ;
#define UVOZ		15 // "
#define APOST		16 // ' 
// Operatory
#define DELENO		19 // /
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
#define true        41
#define false       42
//-------------------
#define EOF         45
//-------------------
//Unikove sekvence
/*#define ESC_TAB     45 // \t
#define ESC_N_L     46 // \n
#define ESC_DOLAR   47 // \$
#define ESC_ESC     48 // \\
#define ESC_UVOZ    49 // \"   */

//Alokace
#define BUFF        100 //Alok ci realok vstupniho stringu

// Funkce
int getToken(FILE *fp);
void skipSpace(char* *c, FILE *f);   //Nacte prvni znak za prazdnymi znaky.
/**
* Struktury
*/

typedef union {
    int varInt;
    double varDouble;
    char* *varString;
}tokenValue;

typedef struct{
	int id;
	tokenValue value;
}token;

