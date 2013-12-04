// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// adt.h
//
//



// struktury pro zasobnik
// ktery bude pouzit pro tabulky symbolu
//
typedef struct tSHashItem{
	tHashTbl hashTbl;
	TLItem NavrInstrukce;
	struct tSHashItem *ptrNext;
} *tStackItemPtr;

typedef struct {
	tStackItemPtr top;
} tHashTblStack;

// struktury pro zasobnik
// ktery bude pouzivat if else
//
typedef struct tSItemIE{
	struct tSItemIE *ptrNext;
}*tStackItemIE;

typedef struct {

} tIfElseStack;