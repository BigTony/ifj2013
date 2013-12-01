#include "stack.h"
#include "scaner.h"
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

/* FUNKCE PRO PRACI SE ZASOBNIKEM */

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
TExpType STop(TStack *stack){
	if(stack->top != NULL){
		return stack->top->item;		
	}
}
// bool prazdny zasobnik
int SEmpty(TStack *stack){
	if(stack->top == NULL)
		return 1;
	else
		return 0;
}

void SPopAll(TStack *stack){
	while(!SEmpty(stack)){
		SPop(stack);
	}        
}

/* HLAVNI FUNKCE PRO PSA */

/* prevede token na vyraz */
TExpType TokenToExpresion(Ttoken token){
	switch(token.id){
		case ZAV_JEDN_L:
			return BRACE_L;
		case ZAV_JEDN_P:
			return BRACE_R;
		case CARKA:
			return COM;
		case KRAT:
			return MULTI;
		case DELENO:
			return DIV;
		case PLUS:
			return PLUS;
		case MINUS:
			return MINUS;
		case TECKA:
			return KONK;
		case MENSI:
			return LTH;
		case VETSI:
			return GTH;
		case MENSI_ROVNO:
			return LTHE;
		case VETSI_ROVNO:
			return GTHE;
		case ROVNO:
			return EQ;
		case NEROVNO:
			return NEQ;
		case VARIABLE:
		case VARINT:
		case VARDOUBLE:
			return VALUE;
		case FUNCTION_CALL:
			return FUNC;
		default:
			return ERROR;
	}
}

void algorythmSA(TStack *stack){
	SPush(stack,END);
}

void reductionByRule(){

}

void printstack(TStack *stack){
	TExpType temp = STop(stack);
	while(temp != END){
		switch(temp){
			case NONTERM:
				printf("E\n");
				break;
			case BRACE_L:
				printf("(\n");
				break;
			case L:
				printf("<\n");
				break;
			case MULTI:
				printf("*\n");
				break;
			case BRACE_R:
				printf(")\n");
				break;
		}
		SPop(stack);
		temp = STop(stack);
	}
	printf("$\n");
}

void ExEqual(TStack *stack,TExpType input){
	SPush(stack,input);
    printstack(stack);	
}

void ExLess(TStack *stack,TExpType input){
	TStack *cur_ptr = stack;
	TSItemPtr prev_ptr;	

	while(STop(cur_ptr) != END){ 
		if(STop(cur_ptr) > END){	
			prev_ptr=cur_ptr->top;	 
            cur_ptr->top=cur_ptr->top->ptrNext;
		}else{
			TSItemPtr temp;
			TExpType item = L;
			// alokace nove polozky
    		temp = malloc(sizeof(struct TSItem));
    		temp->item = item;
  
    		prev_ptr->ptrNext = temp;
          	temp->ptrNext = cur_ptr->top;
			cur_ptr->top = prev_ptr;
			SPush(stack,input);
          	printstack(stack);
			return;
		}
	}
	printf("ERROR CHYBY ZAVORKA\n");
}

// $ < ( E
void ExGreater(TStack *stack){

	TStack *cur_ptr = stack;
	TStack temp;
	SInit(&temp);
	TExpType put = END;
	SPush(&temp,put);
	
	while(STop(cur_ptr) != END){
		if(STop(cur_ptr) != L){
			SPush(&temp,STop(cur_ptr));
			cur_ptr->top=cur_ptr->top->ptrNext;
		}else{
			cur_ptr->top=cur_ptr->top->ptrNext;
			switch(STop(&temp)){
				// E->VALUE
				case VALUE:
					printf("VALUE\n");
					SPop(&temp);
					SPop(&temp);//test vypisy
					// zasobnik musi byt prazdny => nic za E->VALUE
					if(!SEmpty(&temp))
						printf("ERROR!\n");
					SPush(cur_ptr,NONTERM);
					break;
				// E->(VALUE)
				case BRACE_L:
					printf("BRACE_L\n");
					SPop(&temp);
					if(STop(&temp) != NONTERM)
						printf("ERROR!\n");
					SPop(&temp);
					if(STop(&temp) != BRACE_R)
						printf("ERROR!\n");
					SPop(&temp);
					SPop(&temp);//test vypisy
					// zasobnik musi byt prazdny => nic za E->(VALUE)
					if(!SEmpty(&temp))
						printf("ERROR!\n");
					SPush(cur_ptr,NONTERM);
					break;
				case NONTERM:
					printf("NONTERM\n");
					SPop(&temp);
					if(STop(&temp) > LTH){
						printf("ERROR!\n");
					}else{
						printf("CAJK\n");
					}
					SPop(&temp);
					if(STop(&temp) != NONTERM)
						printf("ERROR!\n");
					SPop(&temp);
					SPop(&temp);//test vypisy
					if(!SEmpty(&temp))
						printf("ERROR!\n");
					SPush(cur_ptr,NONTERM);
					break;
				default:
					printf("ERROR\n");
					break;
			}
			// printstack(&temp);
			printstack(cur_ptr);
			return;
		}
	}
	printf("ERROR OPERATOR NEJAKEJ POJEBANEJ\n");
}

int main(int argc,char** argv){
	TStack stack;
	TExpType input = MULTI;
	SInit(&stack);
	SPush(&stack,END);
	SPush(&stack,L);
	SPush(&stack,BRACE_L);
	SPush(&stack,L);
	SPush(&stack,NONTERM);
	SPush(&stack,MULTI);
	SPush(&stack,NONTERM);
	// ExLess(&stack,input);
	// ExEqual(&stack,input);
	ExGreater(&stack);

	// int arr[6] = {ZAV_JEDN_L,VARIABLE,KRAT,VARIABLE,ZAV_JEDN_P};
	// algorythmSA(&stack);
}

// git ls-files | xargs wc -l
