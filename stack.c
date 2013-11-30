#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// PA tabulka
int TabulkaVyrazu [MAX_INDEX][MAX_INDEX] = {	

				/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|FUNC|BRACER_L|BRACE_R|KONK|END*/
	/* VALUE*/		{B,G,G,G,G,G,G,G,G,G,G,G,B,B,G,G,G},
	/* PLUS*/		{L,G,G,L,L,G,G,G,G,G,G,G,L,L,G,G,G},
	/* MINUS*/		{L,G,G,L,L,G,G,G,G,G,G,G,L,L,G,G,G},
	/* MULTI*/		{L,G,G,G,G,G,G,G,G,G,G,G,L,L,G,G,G},
	/* DIV */		{L,G,G,G,G,G,G,G,G,G,G,G,L,L,G,G,G},
	/* EQ */		{L,L,L,L,L,G,G,L,L,L,L,G,L,L,G,L,G},
	/* NEQ */		{L,L,L,L,L,G,G,L,L,L,L,G,L,L,G,L,G},
	/* GTHE */		{L,L,L,L,L,G,G,G,G,G,G,G,L,L,G,L,G},
	/* LTHE */		{L,L,L,L,L,G,G,G,G,G,G,G,L,L,G,L,G},
	/* GTH */		{L,L,L,L,L,G,G,G,G,G,G,G,L,L,G,L,G},
	/* LTH */		{L,L,L,L,L,G,G,G,G,G,G,G,L,L,G,L,G},
	/* COM */		{L,L,L,L,L,L,L,L,L,L,L,E,L,L,E,L,B},
	/* FUNC */		{B,B,B,B,B,B,B,B,B,B,B,B,B,E,B,B,B},
	/* BRACE_L */	{L,L,L,L,L,L,L,L,L,L,L,E,L,L,E,L,B},
	/* BRACE_R */	{B,G,G,G,G,G,G,G,G,G,G,G,B,B,G,G,G},
	/* KONK */		{L,G,G,L,L,G,G,G,G,G,G,G,L,L,G,G,G},
	/* END */   	{L,L,L,L,L,L,L,L,L,L,L,B,L,L,B,L,B}
};

// init zasobniku
void SInit(TStack *stack){
	stack->top = NULL;
}
// vloz prvek na zasobnik
void SPush(TStack *stack,TExpType item){
	TSItemPtr temp;
	// alokace nove polozky
    temp = malloc(sizeof(struct TSItem));
    if (temp == NULL)
    	return;
    temp->item = item;
    temp->ptrNext = stack->top;
    stack->top = temp;
}
// odstran prvek z vrcholu zasobniku
void SPop(TStack *stack){
	if(stack->top != NULL){
		stack->top = stack->top->ptrNext;
	}
}
// nacteni z vrcholu
void STop(TStack *stack, TExpType *item){
	if(stack->top != NULL){
		*item = stack->top->item;		
	}
}
// nacteni z vrcholu a odstraneni prvku
void STopPop(TStack *stack, TExpType *item){
	if(stack->top != NULL){
		*item = stack->top->item;
		SPop(stack);
	}
}
// bool prazdny zasobnik
int SEmpty(TStack *stack){
	if(stack->top == NULL)
		return 1;
	else
		return 0;
}

int main(int argc,char** argv){
	TStack stack;
	TExpType put = PLUS;
	TExpType put1 = MINUS;
	TExpType get;
	SInit(&stack);
	SPush(&stack,put);
	SPush(&stack,put1);
	STop(&stack,&get);
	if(get == PLUS){
		printf("YES PLUS\n");
	}
	STop(&stack,&get);
	if(get == MINUS){
		printf("YES MINUS\n");
	}
	return 0;
}
