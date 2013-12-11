#include <stdio.h>
#include <stdlib.h>
#include "vyrazy.h"

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
    if (temp == NULL){
    	SPopAll(stack);
		print_error(E_INTERN,"chyba pri alokaci SPush");
	}
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
		// if(stack->top->var.varString != 0){
		// 	free(stack->top->var.varString);
		// }
		stack->top = stack->top->ptrNext;
		free(temp);
	}
}
// nacteni z vrcholu
TExpType STop(TStack *stack){
	if(stack->top != NULL){
		return stack->top->item;		
	}
	return ERROR;
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
		case STRING:
		case NIL:
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
			default:
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

// =
void ExEqual(TStack *stack,TExpType input){
	tokenValue value = g_ptrs->token->value;
	SPush(stack,input,value);
}

// <
void ExLess(TStack *stack,TExpType input){
	TStack *cur_ptr = stack;
	TSItemPtr prev_ptr = NULL;	
	tokenValue value;
	switch(g_ptrs->token->id){
		case VARINT:
		case VARBOOL:
		case VARDOUBLE:
			value.varString = gen_id(g_ptrs->counter);
printf ("%f\n",g_ptrs->token->value);
			add_const_hashtbl(g_ptrs->main_symobol_tbl,g_ptrs->token->id,g_ptrs->token->value,(char *)value.varString);
			break;
		case VARIABLE:
			value = g_ptrs->token->value;
			break;
		case STRING:
			value.varString = gen_id(g_ptrs->counter);
			add_const_hashtbl(g_ptrs->main_symobol_tbl,g_ptrs->token->id,g_ptrs->token->value,(char *)value.varString);
			break;
		default:
			break;
	}
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
    		if (temp == NULL){
    			SPopAll(stack);
				print_error(E_INTERN,"chyba pri alokaci ExLess");
			}
    		temp->item = item;
  
    		prev_ptr->ptrNext = temp;
          	temp->ptrNext = cur_ptr->top;
			cur_ptr->top = prev_ptr;
			SPush(stack,input,value);
			return;
		}
	}
    SPopAll(stack);
	print_error(E_SYN,"chyby zavorka pri ExLess");
}


// >
void ExGreater(TStack *stack){
	TStack *cur_ptr = stack;
	TStack temp;
	SInit(&temp);
	
	while(STop(cur_ptr) != ENDSTACK){
		if(STop(cur_ptr) != L){
			SPush(&temp,STop(cur_ptr),cur_ptr->top->var);
			cur_ptr->top=cur_ptr->top->ptrNext;
		}else{
			cur_ptr->top=cur_ptr->top->ptrNext;
			switch(STop(&temp)){
				// E->VALUE
				case VALUE:{
					
					tokenValue value_i = temp.top->var;
					SPop(&temp);
					// zasobnik musi byt prazdny => nic za E->VALUE
					if(!SEmpty(&temp)){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->VALUE neco za value");
					}

					SPush(cur_ptr,NONTERM,value_i);
					break;
				}
				// E->(E)
				case BRACE_L:
					if(STop(&temp) != BRACE_L){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->(E) chyby");
					}
					SPop(&temp);
					if(STop(&temp) != NONTERM){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->(E) chyby NONTERM");
					}
					tokenValue value = temp.top->var;
					SPop(&temp);
					if(STop(&temp) != BRACE_R){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->(E) chyby");
					}
					SPop(&temp);
					// zasobnik musi byt prazdny => nic za E->(VALUE)
					if(!SEmpty(&temp)){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->(E) zasobnik neni prazdny");
					}

					SPush(cur_ptr,NONTERM,value);
					break;
				// E-> E op E
				case NONTERM:
					// operand 1
					if(STop(&temp) != NONTERM){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->E op E , chyby operator 1");
					}
					tokenValue value1 = temp.top->var;
					SPop(&temp);
					// operator
					if(STop(&temp) > COM){
						printf("%i\n",STop(&temp));
						if(STop(&temp) != KONK){
							SPopAll(&temp);
							SPopAll(stack);
							print_error(E_SYN,"chyba pri E->E op E , spatny operator2");
						}
					}
					TIType op = I_LAB;
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
						case KONK:
							op = I_CON;
							break;
						default:
							print_error(E_SYN,"chyba spatny operand E op E");
							break;
					}

					SPop(&temp);
					// operand 2
					if(STop(&temp) != NONTERM){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->E op E , chyby operand 2");
					}
					tokenValue value2 = temp.top->var;
					SPop(&temp);

					if(!SEmpty(&temp)){
						SPopAll(&temp);
						SPopAll(stack);
						print_error(E_SYN,"chyba pri E->E op E , stack neni prazdny )");
					}
					// value musi bejt kam se uklada instrukce aww yeaaah
					value=CreateExInstruction(value1,value2,op);
					SPush(cur_ptr,NONTERM,value);
					break;
				default:
					SPopAll(&temp);
					SPopAll(stack);
					print_error(E_SYN,"chyba pri E->E op E , stack neni prazdny )");
			}
			// printstack(&temp);
			return;
		}
	}
	SPopAll(&temp);
	SPopAll(stack);
	print_error(E_SYN,"chyba pri ExGreater");
}

TExpType skipNonTerm(TStack *stack){
	TStack tempstack;
	SInit(&tempstack);
	TExpType b;

	while(1){	
		b = STop(stack);
		if(b < NONTERM){
			while(!SEmpty(&tempstack)){
				SPush(stack,STop(&tempstack),tempstack.top->var);
				SPop(&tempstack);
			}
			break;
		}else{
			SPush(&tempstack,b,stack->top->var);
			SPop(stack);
		} 
	}	
	return b;
}

int ExEx(int ifYes,char * result){

	tokenValue value;
	if(ifYes == IF){
		if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_L){
			print_error(E_SYN,"chyby ( v ifu");
		}
	}else{
		int c = getToken_test(g_ptrs->source,g_ptrs->token);
		if((c == STREDNIK) || (c == ZAV_SLOZ_L)){
			print_error(E_SYN,"syntax error $a = ;");
		}
		if(c == IDENTIFIKATOR)
			return 1;

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
	int redukce = 0;

	while((g_ptrs->token->id != STREDNIK)&&(g_ptrs->token->id != ZAV_SLOZ_L)){
		
		a = TokenToExpresion(g_ptrs->token->id);
		
		b = skipNonTerm(&stack);		

		
		if(b > END){
			SPopAll(&stack);
			print_error(E_SYN,"chyba na zasobniku neni symbol z tabulky");
		}
	
		nonterm = TabulkaVyrazu[b][a];

		if(nonterm == E){
			ExEqual(&stack,a);
		}else if(nonterm == L){
			ExLess(&stack,a);
		}else if(nonterm == G){
			ExGreater(&stack);
			redukce = 1;
		}else if(nonterm == B){
			SPopAll(&stack);
			print_error(E_SYN,"chyba prazdne misto z tabulky");
		}else{
			SPopAll(&stack);
			print_error(E_SYN,"chyba tabulka vratila neexistujici hodnotu");
		}


		if(redukce == 0){
			getToken_test(g_ptrs->source,g_ptrs->token);
		}else{
			redukce = 0;
		}
	}


	// po nalezeni ; dokonceni vyrazu 
	while(1){
		a = END;
		b = skipNonTerm(&stack);
		if(b == END){
			break;
		}
		if(b > END){
			SPopAll(&stack);
			print_error(E_SYN,"chyba na zasobniku neni symbol z tabulky");
		}
		nonterm = TabulkaVyrazu[b][a];
		if(nonterm == G){
			ExGreater(&stack);
		}else if(nonterm == B){
			SPopAll(&stack);
			print_error(E_SYN,"chyba prazdne misto z tabulky");
		}else{
			SPopAll(&stack);
			print_error(E_SYN,"chyba tabulka vratila neexistujici hodnotu");
		}
	}

	InsertInstLast (g_ptrs->act_list_inst,(char *)stack.top->var.varString,NULL,result,I_ASS);
	SPopAll(&stack);
	printf("KONCIM VE VYRAZECH-------\n");
	return 0;

}

// // git ls-files | xargs wc -l
