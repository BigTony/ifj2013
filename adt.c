// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// adt.c
//
//
#include <stdio.h>
#include <stdlib.h>
#include "adt.h"

// zasobnik pro tabulku symbolu
//
void initStack(tHashTblStack **stack){
	if((*stack = (tHashTblStack*)malloc(sizeof(tHashTblStack))) == NULL){
		print_error(E_INTERN,"chyba pri alokaci stacku tabulky symbolu");
	}
	(*stack)->top = NULL;
}

void pushStack(tHashTblStack *stack,tHashTbl *hashTbl,TLItem *NavrInstrukce){
	tStackItemPtr temp;
	if((temp = (tStackItemPtr )malloc(sizeof(struct tSHashItem))) == NULL){
		print_error(E_INTERN,"chyba pri alokaci itemu stacku pro tabulku symbolu");
	}
	temp->hashTbl = hashTbl;
	temp->NavrInstrukce = NavrInstrukce;

	temp->ptrNext = stack->top;
    stack->top = temp;
}

void popStack(tHashTblStack *stack){
	tStackItemPtr temp;
	if (stack->top != NULL){
		temp = stack->top;
		stack->top = stack->top->ptrNext;
		free(temp);
	}
}

tStackItemPtr topStack(tHashTblStack *stack){
	if(stack->top != NULL){
		return stack->top;
	}
	return NULL;
}

int emptyStack(tHashTblStack *stack){
	if(stack->top == NULL)
		return 1;
	else
		return 0;
}

void freeStack(tHashTblStack *stack){
	tStackItemPtr temp;
	while(!emptyStack(stack)){
		while(stack->top != NULL){
			TblDelete(stack->top->hashTbl);
			// uvolneni polozky stacku a prepnuti na dalsi
			temp = stack->top;
			stack->top = stack->top->ptrNext;
			free(temp);
		}
	}        
	free(stack);
}
