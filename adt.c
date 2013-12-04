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

void initStack(tHashTblStack *stack){
	stack->top = NULL;
}

void pushStack(tHashTblStack *stack,tHashTbl hashTbl,TLItem NavrInstrukce){
	tStackItem temp;
	if(temp = malloc(sizeof(struct tStackItem)) == NULL){
		print_error(E_INTERN,"chyba pri alokaci stacku pro tabulku symbolu");
	}
	temp->hashTbl = hashTbl;
	temp->NavrInstrukce = NavrInstrukce;

	temp->ptrNext = stack->top;
    stack->top = temp;
}

void popStack(tHashTblStack *stack){
	tStackItem temp;
	if (stack->top != NULL){
		temp = stack->top;
		stack->top = stack->top->ptrNext;
		free(temp);
	}
}

tStackItem topStack(tHashTblStack *stack){
	if(stack->top != NULL){
		return stack->top;
	}
}

int emptyStack(tHashTblStack *stack){
	if(stack->top == NULL)
		return 1;
	else
		return 0;
}

void freeStack(tHashTblStack *stack){
	while(!SEmpty(stack)){
		popStack(stack);
	}        
	free(stack);
}
