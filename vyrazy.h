#include "scaner.h"
#include "ilist.h"
// #include "errors.h"
// hodnoty ktere jsou mozne ulozit na zasobnik
typedef enum{
	VALUE, // promena 0F
	PLUS_, // + 1
	MINUS_, // - 2
	MULTI, // * 3
	DIV, // / 4
	EQ, // === 5
	NEQ, // !== 6
	GTHE, // >= 7 
	LTHE, // <= 8
	GTH, // > 9
	LTH, // < 10 
	COM, // , 11
	// FUNC, // Funkce 12 
	BRACE_L, // ( 13 
	BRACE_R, // ) 14		
	KONK, // konkatenace . 15
	END, // $ 16

	// nonterminal
	NONTERM, // pravidlo r 17
	G, // 18
	L, // 19
	E, // 20
	B, // 21
	ENDSTACK, // 22
	ERROR // 23
} TExpType; // 24

#define MAX_INDEX 17

// #define G 0 // >
// #define L 1 // <
// #define E 2 // =
// #define B 3 // blank, prazdna cast tabulky


// struktury pro zasobnik
typedef struct TSItem{
	tokenValue var;
	TExpType item;
	struct TSItem *ptrNext;
} *TSItemPtr;

typedef struct {
	TSItemPtr top;
} TStack;

// init zasobniku
void SInit(TStack *stack);
// vloz prvek na zasobnik
void SPush(TStack *stack,TExpType item,tokenValue value);
// odstran prvek z vrcholu zasobniku
void SPop(TStack *stack);
// nacteni z vrcholu
TExpType STop(TStack *stack);
// bool prazdny zasobnik
int SEmpty(TStack *stack);
// odstran vsechny prvky
void SPopAll(TStack *stack);




