


#ifndef VYRAZY_H
#define VYRAZY_H
#include "scaner.h"
#include "ilist.h"
#include "errors.h"
#include "memory.h"
#include "parser.h"
// hodnoty ktere jsou mozne ulozit na zasobnik
typedef enum{
	VALUE, // promena 0
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
	BRACE_L, // ( 12
	BRACE_R, // ) 13		
	KONK, // konkatenace . 14
	END, // $ 15

	// nonterminal
	NONTERM, // pravidlo r 16
	G, // 17
	L, // 18
	E, // 19
	B, // 20
	ENDSTACK, // 21
	ERROR // 22
} TExpType;

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

int ExEx(int ifYes,char * result);
#endif



