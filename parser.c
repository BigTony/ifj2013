// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// parser.c
// 
//
#include <string.h>
#include <limits.h>
#include "parser.h"


/**Funkce pro vlozeni constanty do hashTbl
 * @param hashTbl: tabulka kam vkladas
 * @param type: typ vlozene hodnoty
 * @param tokenValue: hodnota vlozene hodnoty
 * @param counter: ukazatel na pocitadlo constant string  
 */     
void add_const_hashtbl(tHashTbl *hashTbl, int type, tokenValue value, char *id)
{
	TblInsert (hashTbl, id, value, type);	
}

char* gen_id(char *counter)
{
	int i=2;
	do
	{
		counter[i++]++;
	} while ((counter[i]!='\0') &&(counter[i]==MAX_CHAR));	
	return(counter);	
}

// deklarace nebo prirazeni promene
void defVar(tokenValue value){
	var_name = value;
	if(gettoken(*token) != PRIRAZENI){
		print_error(E_SYN);
	}else{
		ExEx(0,var_name);
	}
}

// podminka if
void defIf(){
	char* TmpExp=gen_id(ptrs->counter);
	char* TmpJmp=gen_id(ptrs->counter);
	ExEx(IF,tmpExp); // vyhodnoceni vyrazu	
	InsertInstLast (ptrs->act_list_inst,TmpExp,NULL,TmpJmp,I_JZ);
	TLItem *tmpItem;
	// vytvoreni 3AC
	// vytvoreni 3AC podmineneho skoku1
	if(gettoken(*token) != ZAV_SLOZ_L){
		print_error(E_SYN);
	}else{
		// { ulozym na zasobnik
		classify();
		InsertInstLast (ptrs->act_list_inst,NULL,NULL,NULL,I_LAB);
		add_const_hashtbl(ptrs->main_symobol_tbl, IDENTIFIKATOR, ptrs->act_list_inst->Last, TmpJmp);
		//ulozeni nazvu a odkazu navesti do globalni tabulky
		if(gettoken(*token)) == ELSE){
			if(gettoken(*token) != ZAV_SLOZ_L){
				print_error(E_SYN);
			}else{
				// { ulozym na zasobnik
				classify();
				// vytvoreni 3AC podmineneho obsahu skoku1
			}
		}
		return;
	}
}

// cyklus while
void defWhile(){
	if(gettoken(*token) != ZAV_JEDN_L){
		print_error(E_SYN);
	}else{
		// vyhodnoceni vyrazu
		// vytvoreni 3AC
		if(gettoken(*token) != ZAV_SLOZ_L){
			print_error(E_SYN);
		}else{
			// { ulozym na zasobnik
			classify();
			// vytvoreni 3AC podmineneho obsahu skoku1
			return;
		}
	}
}

// retuuurn
void defReturn(){
	if(gettoken(*token) != VARIABLE){
		print_error(E_SYN);
	}else{
		if(gettoken(*token) != STREDNIK){
			print_error(E_SYN);
		}else{
			// vygeneruje 3AC
			return;
		}
	}
}

// volani funkce
void defFunction(tokenValue value){
	if(gettoken(*token) != FUNCTION_CALL){
		print_error(E_SYN);
	}else{
		if(gettoken(*token) != ZAV_JEDN_L){
			print_error(E_SYN);
		}else{
			// vyhodnoceni vyrazu
			// vytvoreni 3AC
			if(gettoken(*token) != ZAV_SLOZ_L){
				print_error(E_SYN);
			}else{
				// { ulozym na zasobnik
				classify();
				// vytvoreni 3AC podmineneho obsahu skoku1
				return;
			}
		}
	}
}

// volani funkce
void callFunction(tokenValue value){
	// vygenerujeme 3AC pro nazev funkce
	if(gettoken(*token) != ZAV_JEDN_L){
		print_error(E_SYN);
	}else{
		while(gettoken(*token) != ZAV_JEDN_P){
			if(*token.id != VARIABLE){
				print_error(E_SYN);
			}else{
				// generujeme 3AC
			}
			if(gettoken(*token) != CARKA){
				print_error(E_SYN);
			}
		}
	}
}

// vyber spravnej postup pro token
void classify(){
	while(gettoken(*token)){
		if(*token.id == KONEC){
			return; // kdyz je token konec analyzy
		}
		// token je bud promena nebo volani fce
		if(*token.id == VARIABLE){
			defVar(*temp_token.value);	
		}else if(*token.id == FUNCTION_CALL){
			callFunction(*temp_token.value);
		}else if(*token.id == FUNCTION){
			defFunction();
		}else if(*token.id == IF){
			defIf();
		}else if(*token.id == WHILE){
			defWhile();
			break;
		}else if(*token.id == RETURN){
			defRetrun();
		}else if(*token.id == ZAV_SLOZ_P){
			// zkontroluje jestli je na zasobniku }
			break;
		}
	}
}

void parser(tPointers *ptrs){
	gettoken(ptrs->token);
	if (*token.id == START){
			classify();
		}
	else{
		print_error(E_SYN);
		return;
	}
	
}
