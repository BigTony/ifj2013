#include "vyrazy.h"
#include <stdio.h>
#include <stdlib.h>

// PA tabulka
int TabulkaVyrazu [MAX_INDEX][MAX_INDEX] = {	

				/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|FUNC|BRACER_L|BRACE_R|KONK|END*/
	/* VALUE*/		{B,G,G,G,G,G,G,G,G,G,G,G,B,G,G,G},
	/* PLUS*/		{L,G,G,L,L,G,G,G,G,G,G,G,L,G,G,G},
	/* MINUS*/		{L,G,G,L,L,G,G,G,G,G,G,G,L,G,G,G},
	/* MULTI*/		{L,G,G,G,G,G,G,G,G,G,G,G,L,G,G,G},
	/* DIV */		{L,G,G,G,G,G,G,G,G,G,G,G,L,G,G,G},
	/* EQ */		{L,L,L,L,L,G,G,L,L,L,L,G,L,G,L,G},
	/* NEQ */		{L,L,L,L,L,G,G,L,L,L,L,G,L,G,L,G},
	/* GTHE */		{L,L,L,L,L,G,G,G,G,G,G,G,L,G,L,G},
	/* LTHE */		{L,L,L,L,L,G,G,G,G,G,G,G,L,G,L,G},
	/* GTH */		{L,L,L,L,L,G,G,G,G,G,G,G,L,G,L,G},
	/* LTH */		{L,L,L,L,L,G,G,G,G,G,G,G,L,G,L,G},
	/* COM */		{L,L,L,L,L,L,L,L,L,L,L,E,L,E,L,B},
	// /* FUNC */		{B,B,B,B,B,B,B,B,B,B,B,B,B,E,B,B,B},
	/* BRACE_L */	{L,L,L,L,L,L,L,L,L,L,L,E,L,E,L,B},
	/* BRACE_R */	{B,G,G,G,G,G,G,G,G,G,G,G,B,G,G,G},
	/* KONK */		{L,G,G,L,L,G,G,G,G,G,G,G,L,G,G,G},
	/* END */   	{L,L,L,L,L,L,L,L,L,L,L,B,L,B,L,B}
};

/* FUNKCE PRO PRACI SE ZASOBNIKEM */

// init zasobniku
void SInit(TStack *stack){
	stack->top = NULL;
}
// vloz prvek na zasobnik
void SPush(TStack *stack,TExpType item,tokenValue value){
	TSItemPtr temp;
	// alokace nove polozky
    temp = malloc(sizeof(struct TSItem));
    if (temp == NULL)
    	return;
    temp->item = item;
    temp->var = value;
    temp->ptrNext = stack->top;
    stack->top = temp;
}
// odstran prvek z vrcholu zasobniku
void SPop(TStack *stack){
	TSItemPtr temp;
	if(stack->top != NULL){
		temp = stack->top;
		stack->top = stack->top->ptrNext;
		free(temp);
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
	free(stack);
}

/* HLAVNI FUNKCE PRO PSA */

/* prevede token na vyraz */
// TExpType TokenToExpresion(Ttoken token){
TExpType TokenToExpresion(int token){
	switch(token){
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
			return PLUS_;
		case MINUS:
			return MINUS_;
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
		// case FUNCTION_CALL:
		// 	return FUNC;
		default:
			return ERROR;
	}
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
			case VALUE:
				printf("i\n");
				break;
			// case FUNC:
			// 	printf("FUNC\n");
			// 	break;
		}
		SPop(stack);
		temp = STop(stack);
	}
	printf("$\n");
}

void ExEqual(TStack *stack,TExpType input){
	tokenValue value;
	SPush(stack,input,value);
}

void ExLess(TStack *stack,TExpType input){
	TStack *cur_ptr = stack;
	TSItemPtr prev_ptr;	
	tokenValue value;
	// kdyz je hned nahore
	if(STop(cur_ptr) <= END){
		SPush(stack,L,value);
		SPush(stack,input,value);		
		return;
	}
	// kdyz neni hned nahore
	while(STop(cur_ptr) != ENDSTACK){ 
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
			SPush(stack,input,value);
			return;
		}
	}
	printf("ERROR CHYBY ZAVORKA\n");
}

// $ < ( E
void ExGreater(TStack *stack){
	tokenValue value;
	TStack *cur_ptr = stack;
	TStack temp;
	SInit(&temp);
	
	while(STop(cur_ptr) != ENDSTACK){
		if(STop(cur_ptr) != L){
			SPush(&temp,STop(cur_ptr),value);
			cur_ptr->top=cur_ptr->top->ptrNext;
		}else{
			cur_ptr->top=cur_ptr->top->ptrNext;
			switch(STop(&temp)){
				// E->VALUE
				case VALUE:
					if(STop(&temp) != VALUE)
						printf("ERROR_V1\n");
					tokenValue value_i = temp.top->var;
					SPop(&temp);
					// zasobnik musi byt prazdny => nic za E->VALUE
					if(!SEmpty(&temp))
						printf("ERROR_V2\n");
					SPush(cur_ptr,NONTERM,value_i);
					break;
				// E->(VALUE)
				case BRACE_L:
					if(STop(&temp) != BRACE_L)
						printf("ERROR_B1\n");
					SPop(&temp);
					if(STop(&temp) != NONTERM)
						printf("ERROR_B2\n");
					tokenValue value = temp.top->var;
					SPop(&temp);
					if(STop(&temp) != BRACE_R)
						printf("ERROR_B3\n");
					SPop(&temp);
					// zasobnik musi byt prazdny => nic za E->(VALUE)
					if(!SEmpty(&temp))
						printf("ERROR_B4\n");
					SPush(cur_ptr,NONTERM,value);
					break;
				// E-> E op E
				case NONTERM:
					// operand 1
					if(STop(&temp) != NONTERM)
						printf("ERROR_T1\n");
					tokenValue value1 = temp.top->var;
					SPop(&temp);
					// operator
					if(STop(&temp) > COM){
						if(STop(&temp) != KONK)
							printf("ERROR_T2\n");
					}
					TIType op;
					switch(STop(&temp)){
						case PLUS_:
							op = I_ADD;
							break;
						case MINUS_:
							op = I_SUB;
							break;
						case MULTI:
							op = I_MUL;
							break;
						case DIV:
							op = I_DIV;
							break;
						case EQ:
							op = I_ET;
							break;
						case NEQ:	
							op = I_NET;
							break;
						case GTHE:
							op = I_GE;
							break;
						case LTHE:
							op = I_LE;
							break;
						case GTH:
							op = I_G;
							break;
						case LTH:
							op = I_L;
							break;
					}
					SPop(&temp);
					// operand 2
					if(STop(&temp) != NONTERM)
						printf("ERROR_T3\n");
					tokenValue value2 = temp.top->var;
					SPop(&temp);

					callCreateINstruction(value1,value2,op);
					
					if(!SEmpty(&temp))
						printf("ERROR_T4\n");
					// value musi bejt kam se uklada instrukce aww yeaaah
					SPush(cur_ptr,NONTERM,value);
					break;
				default:
					printf("ERROR\n");
					break;
			}
			// printstack(&temp);
			return;
		}
	}
	printf("ERROR OPERATOR NEJAKEJ POJEBANEJ\n");
}

TExpType skipNonTerm(TStack *stack){
	TStack tempstack;
	SInit(&tempstack);
	tokenValue value;
	TExpType b;

	while(1){	
		b = STop(stack);
		if(b < NONTERM){
			while(!SEmpty(&tempstack)){
				SPush(stack,STop(&tempstack),value);
				SPop(&tempstack);
			}
			break;
		}else{
			SPush(&tempstack,b,value);
			SPop(stack);
		} 
	}	
	return b;
}

void ExEx(int ifYes,char *result){
	tokenValue value;
	if(ifYes == IF){
		if(gettoken(ptrs->token) != ZAV_JEDN_L){
			printf("ERROR BIATCH\n");
			return;
		}
	}else{
		gettoken(ptrs->token);
	}

	TStack stack;
	TStack tempstack;
	TExpType a;
	TExpType b;
	TExpType nonterm;
	SInit(&stack);
	SInit(&tempstack);
	SPush(&stack,ENDSTACK,value);
	SPush(&stack,END,value);
	int c = 0;
	int redukce = 0;

	do{
		printf("%i\n",c );
		a = TokenToExpresion(ptrs->token->id);
		b = skipNonTerm(&stack);		

		if(b > END){
			printf("ERROR NA ZASOBNIKU NENI HODNOTA Z TABULKY\n");
			return;
		}
	
		nonterm = TabulkaVyrazu[b][a];
			
		if(nonterm == E){
			printf("[E]\n");
			ExEqual(&stack,a);
		}else if(nonterm == L){
			printf("[L]\n");
			ExLess(&stack,a);
		}else if(nonterm == G){
			printf("[G]\n");
			ExGreater(&stack);
			redukce = 1;
		}else if(nonterm == B){
			printf("SYNTAX ERROR\n");
			return;
		}else{
			printf("ERROR\n");
			return;
		}

		if(redukce == 0){
			c = c+1;
		}else{
			redukce = 0;
		}
	}while(gettoken(ptrs->token));


	// po nalezeni ; dokonceni vyrazu 
	while(1){
		a = END;
		b = skipNonTerm(&stack);
		if(b == END)
			break;
		if(b > END){
			printf("ERROR NA ZASOBNIKU NENI HODNOTA Z TABULKY\n");
			return;
		}
		nonterm = TabulkaVyrazu[b][a];
		if(nonterm == G){
			printf("[G]\n");
			ExGreater(&stack);
		}else if(nonterm == B){
			printf("ERROR\n");
			return;
		}else{
			printf("ERROR\n");
			return;
		}
	}
	printstack(&stack);

}




// int main(int argc,char** argv){
// 	// int array[15] = {FUNCTION_CALL,ZAV_JEDN_L,VARIABLE,CARKA,VARIABLE,ZAV_JEDN_P,STREDNIK};
// 	// int array[15] = {ZAV_JEDN_L,VARIABLE,KRAT,VARIABLE,ZAV_JEDN_P,STREDNIK};
// 	int array[30] = {ZAV_JEDN_L,ZAV_JEDN_L,VARIABLE,KRAT,VARIABLE,ZAV_JEDN_P,PLUS,ZAV_JEDN_L,ZAV_JEDN_L,VARIABLE,DELENO,VARIABLE,ZAV_JEDN_P,MINUS,ZAV_JEDN_L,VARIABLE,TECKA,VARIABLE,ZAV_JEDN_P,ZAV_JEDN_P,ZAV_JEDN_P,STREDNIK};

// 	TStack stack;
// 	TStack tempstack;
// 	TExpType a;
// 	TExpType b;
// 	TExpType nonterm;
// 	SInit(&stack);
// 	SInit(&tempstack);
// 	SPush(&stack,ENDSTACK);
// 	SPush(&stack,END);
// 	int c = 0;
// 	int redukce = 0;

// 	while(array[c] != STREDNIK){
// 		printf("%i\n",c );
// 		a = TokenToExpresion(array[c]);
// 		b = skipNonTerm(&stack);		

// 		if(b > END){
// 			printf("ERROR NA ZASOBNIKU NENI HODNOTA Z TABULKY\n");
// 			return;
// 		}
	
// 		nonterm = TabulkaVyrazu[b][a];
			
// 		if(nonterm == E){
// 			printf("[E]\n");
// 			ExEqual(&stack,a);
// 		}else if(nonterm == L){
// 			printf("[L]\n");
// 			ExLess(&stack,a);
// 		}else if(nonterm == G){
// 			printf("[G]\n");
// 			ExGreater(&stack);
// 			redukce = 1;
// 		}else if(nonterm == B){
// 			printf("SYNTAX ERROR\n");
// 			return;
// 		}else{
// 			printf("ERROR\n");
// 			return;
// 		}

// 		if(redukce == 0){
// 			c = c+1;
// 		}else{
// 			redukce = 0;
// 		}
		
// 	}

// 	// po nalezeni ; dokonceni vyrazu 
// 	while(1){
// 		a = END;
// 		b = skipNonTerm(&stack);
// 		if(b == END)
// 			break;
// 		if(b > END){
// 			printf("ERROR NA ZASOBNIKU NENI HODNOTA Z TABULKY\n");
// 			return;
// 		}
// 		nonterm = TabulkaVyrazu[b][a];
// 		if(nonterm == G){
// 			printf("[G]\n");
// 			ExGreater(&stack);
// 		}else if(nonterm == B){
// 			printf("ERROR\n");
// 			return;
// 		}else{
// 			printf("ERROR\n");
// 			return;
// 		}
// 	}
// 	printstack(&stack);

// }

// // git ls-files | xargs wc -l
