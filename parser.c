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
void add_const_hashtbl(tHashTbl *hashTbl, int type,tokenValue value, char *id)
{
	TblInsert (hashTbl, id, value, type);	
}

char* gen_id(char *counter)
{
	int i=2;
	do
	{
		counter[i++]++;
	} while ((counter[i]!='\0') &&(counter[i]==CHAR_MAX));	
	return(counter);	
}

// deklarace nebo prirazeni promene
void defVar(tokenValue value){
	char * var_name = value.varString;
	if(getToken(ptrs->source,ptrs->token) != PRIRAZENI){
		print_error(E_SYN,"chyba v syntaxi ocekavano = pri prirazeni promene");
	}else{
		ExEx(0,var_name);
	}
}

// podminka if
void defIf(){
	char* TmpExp=gen_id(ptrs->counter);
	char* TmpJmp=gen_id(ptrs->counter);
	ExEx(IF,TmpExp); // vyhodnoceni vyrazu	
	InsertInstLast (ptrs->act_list_inst,TmpExp,NULL,TmpJmp,I_JZ);
	TLItem *tmpItem;
	// vytvoreni 3AC
	// vytvoreni 3AC podmineneho skoku1
	if(getToken(ptrs->source,ptrs->token) != ZAV_SLOZ_L){
		print_error(E_SYN,"chyby leva slozena zavorka u if {");
	}else{
		// { ulozym na zasobnik
		classify();
		InsertInstLast (ptrs->act_list_inst,NULL,NULL,NULL,I_LAB);
		add_const_hashtbl(ptrs->main_symobol_tbl, IDENTIFIKATOR, (tokenValue)(void*)ptrs->act_list_inst->Last, TmpJmp);
		//ulozeni nazvu a odkazu navesti do globalni tabulky
		if(getToken(ptrs->source,ptrs->token) == ELSE){
			if(getToken(ptrs->source,ptrs->token) != ZAV_SLOZ_L){
				print_error(E_SYN,"chyby leva slozena zavorka u else {");
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
	if(getToken(ptrs->source,ptrs->token) != ZAV_JEDN_L){
		print_error(E_SYN,"chyba leva zavorka u while");
	}else{
		// vyhodnoceni vyrazu
		// vytvoreni 3AC
		if(getToken(ptrs->source,ptrs->token) != ZAV_SLOZ_L){
			print_error(E_SYN,"chyby slozena leva zavorka u while");
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
	if(getToken(ptrs->source,ptrs->token) != VARIABLE){
		print_error(E_SYN,"pri returnu neni zadna returnova hodnota");
	}else{
		if(getToken(ptrs->source,ptrs->token) != STREDNIK){
			print_error(E_SYN,"za returnem neni strednik");
		}else{
			// vygeneruje 3AC
			return;
		}
	}
}

// deklarace funkce
void defFunction(tokenValue value){
	if(getToken(ptrs->source,ptrs->token) != IDENTIFIKATOR){
		print_error(E_SYN,"pri deklarace funkce chyby jeji nazev");
	}else{
		if(getToken(ptrs->source,ptrs->token) != ZAV_JEDN_L){
			print_error(E_SYN,"pri deklaraci funkce chyby ( ");
		}else{
			// vyhodnoceni vyrazu
			// vytvoreni 3AC
			if(getToken(ptrs->source,ptrs->token) != ZAV_SLOZ_L){
				print_error(E_SYN,"pri deklaraci funkce chyby {");
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
	if(getToken(ptrs->source,ptrs->token) != ZAV_JEDN_L){
		print_error(E_SYN,"pri volani funkce chyby (");
	}else{
		while(getToken(ptrs->source,ptrs->token) != ZAV_JEDN_P){
			if(ptrs->token->id != VARIABLE){
				print_error(E_SYN,"pri volani funkce jsou parametry jine symboly");
			}else{
				// generujeme 3AC
			}
			if(getToken(ptrs->source,ptrs->token) != CARKA){
				print_error(E_SYN,"za parametrem neni carka");
			}
		}
	}
}

// vyber spravnej postup pro token
void classify(Ttoken token){
	while(getToken(ptrs->source,ptrs->token)){
		if(token.id == KONEC){
			return; // kdyz je token konec analyzy
		}
		// token je bud promena nebo volani fce
		if(token.id == VARIABLE){
			defVar(ptrs->token->value);	
		}else if(token.id == IDENTIFIKATOR){
			callFunction(token.value);
		}else if(token.id == FUNCTION){
			defFunction(token.value);
		}else if(token.id == IF){
			defIf();
		}else if(token.id == WHILE){
			defWhile();
			break;
		}else if(token.id == RETURN){
			defRetrun();
		}else if(token.id == ZAV_SLOZ_P){
			// zkontroluje jestli je na zasobniku }
			break;
		}
	}
}

void parser(tPointers *ptrs){
	getToken(ptrs->source,ptrs->token);
	if (ptrs->token->id == START){
			classify(*(ptrs->token));
		}
	else{
		print_error(E_SYN,"nazacatku neni <php");
		return;
	}
	
}
