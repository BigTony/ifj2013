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



#ifndef ADT_H
#define ADT_H
#include "ilist.h"
#include "ial.h"
#include "errors.h"

// struktury pro zasobnik
// ktery bude pouzit pro tabulky symbolu
//
typedef struct tSHashItem{
	tHashTbl hashTbl;
	TLItem *NavrInstrukce;
	struct tSHashItem *ptrNext;
} *tStackItemPtr;

typedef struct {
	tStackItemPtr top;
} tHashTblStack;

void initStack(tHashTblStack *stack);
void pushStack(tHashTblStack *stack,tHashTbl hashTbl,TLItem *NavrInstrukce);
void popStack(tHashTblStack *stack);
tStackItemPtr topStack(tHashTblStack *stack);
int emptyStack(tHashTblStack *stack);
void freeStack(tHashTblStack *stack);

// struktury pro zasobnik
// ktery bude pouzivat if else
//
typedef struct tSItemIE{
	int zavorka;
	struct tSItemIE *ptrNext;
}*tStackItemIEPtr;

typedef struct {
	tStackItemIEPtr top;
} tIfElseStack;

void initStackIE(tIfElseStack *stack);
void pushStackIE(tIfElseStack *stack,int symbol);
void popStackIE(tIfElseStack *stack);
tStackItemIEPtr topStackIE(tIfElseStack *stack);
int emptyStackIE(tIfElseStack *stack);
void freeStackIE(tIfElseStack *stack);
#endif


