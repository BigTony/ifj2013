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
typedef struct tSItem{
	tHashTbl hashTbl;
	TLItem NavrInstrukce;
	struct tSItem *ptrNext;
} *tStackItem;

typedef struct {
	tSItemPtr top;
} tHashTblStack;