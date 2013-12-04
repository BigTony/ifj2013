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

void initStack(tHashTblStack *stack);
void pushStack(tHashTblStack *stack,tHashTbl hashTbl,TLItem NavrInstrukce);
void popStack(tHashTblStack *stack);
tStackItem topStack(tHashTblStack *stack);
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
	tStackItemIE top;
} tIfElseStack;

void initStack(tIfElseStack *stack);
void pushStack(tIfElseStack *stack,int symbol);
void popStack(tIfElseStack *stack);
tStackItem topStack(tIfElseStack *stack);
int emptyStack(tIfElseStack *stack);
void freeStack(tIfElseStack *stack);



