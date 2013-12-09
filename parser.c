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
 * @param id: ukazatel na string  
 */     
void add_const_hashtbl(tHashTbl *hashTbl, int type,tokenValue value, char *id)
{
	TblInsert (hashTbl, id, value, type);	
}
/**Generovani pomocnych idecek
 * @param counter: string ze ktereho se generuje
 * @return: Vraci ukazatel na novy string 
 */ 
char* gen_id(char *counter)
{
	int i=2;
	do
	{
		counter[i]++;
		i++;
	} while ((counter[i]!='\0') &&(counter[i]==CHAR_MAX));
	char* new=allocString();
	strcpy(new,counter);	
	return(new);	
}

tokenValue CreateExInstruction(tokenValue value1,tokenValue value2,TIType op)
{
tokenValue new_id;
new_id.varString = gen_id(g_ptrs->counter);
InsertInstLast (g_ptrs->list_instr,value1.varString,value2.varString,(char*)new_id.varString,op);   
return (new_id);
}

// deklarace nebo prirazeni promene
void defVar(tokenValue value){
	char * var_name = value.varString;
	if(getToken_test(g_ptrs->source,g_ptrs->token) != PRIRAZENI){
		print_error(E_SYN,"chyba v syntaxi ocekavano = pri prirazeni promene");
	}else{
		printf("lezu do vyrazu\n");
		ExEx(0,var_name);
		printf("jsem zpet z vyrazu\n");
	}
}


// podminka if
void defIf(){
	char* TmpExp=gen_id(g_ptrs->counter);
	char* TmpJmp=gen_id(g_ptrs->counter);
	ExEx(IF,TmpExp); // vyhodnoceni vyrazu	
	InsertInstLast (g_ptrs->act_list_inst,TmpExp,NULL,TmpJmp,I_JZ);
	TLItem *tmpItem = g_ptrs->list_instr->Last;
	// vytvoreni 3AC
	// vytvoreni 3AC podmineneho skoku1
	if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_SLOZ_L){
		print_error(E_SYN,"chyby leva slozena zavorka u if {");
	}else{
		pushStackIE(g_ptrs->IEStack,g_ptrs->token->id); // { ulozym na zasobnik
		classify();
		InsertInstLast (g_ptrs->act_list_inst,NULL,NULL,NULL,I_LAB);
		add_const_hashtbl(g_ptrs->main_symobol_tbl, IDENTIFIKATOR, (tokenValue)(void*)g_ptrs->act_list_inst->Last, TmpJmp);
		//ulozeni nazvu a odkazu navesti do globalni tabulky
		return;
	}
}

void defElse(){
return;
}
// cyklus while
void defWhile(){
	if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_L){
		print_error(E_SYN,"chyba leva zavorka u while");
	}else{
		// vyhodnoceni vyrazu
		// vytvoreni 3AC
		if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_SLOZ_L){
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
	if(getToken_test(g_ptrs->source,g_ptrs->token) != VARIABLE){
		print_error(E_SYN,"pri returnu neni zadna returnova hodnota");
	}else{
		if(getToken_test(g_ptrs->source,g_ptrs->token) != STREDNIK){
			print_error(E_SYN,"za returnem neni strednik");
		}else{
			// vygeneruje 3AC
			return;
		}
	}
}

// deklarace funkce
void defFunction(tokenValue value){
	if(getToken_test(g_ptrs->source,g_ptrs->token) != IDENTIFIKATOR){
		print_error(E_SYN,"pri deklarace funkce chyby jeji nazev");
	}else{
		if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_L){
			print_error(E_SYN,"pri deklaraci funkce chyby ( ");
		}else{
			// vyhodnoceni vyrazu
			// vytvoreni 3AC
			if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_SLOZ_L){
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
	if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_L){
		print_error(E_SYN,"pri volani funkce chyby (");
	}else{
		while(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_P){
			if(g_ptrs->token->id != VARIABLE){
				print_error(E_SYN,"pri volani funkce jsou parametry jine symboly");
			}else{
				// generujeme 3AC
			}
			if(getToken_test(g_ptrs->source,g_ptrs->token) != CARKA){
				print_error(E_SYN,"za parametrem neni carka");
			}
		}
	}
}

// vyber spravnej postup pro token
void classify(){
	printf("sem v clasify\n");
	while (getToken_test(g_ptrs->source,g_ptrs->token)!=KONEC)
	{
		printf("sem ve whilu clasify\n");		
		// token je bud promena nebo volani fce
		if(g_ptrs->token->id == VARIABLE){
			printf("VARIABLE\n");
			defVar(g_ptrs->token->value);	
		}else if(g_ptrs->token->id == IDENTIFIKATOR){
			callFunction(g_ptrs->token->value);
		}else if(g_ptrs->token->id == FUNCTION){
			defFunction(g_ptrs->token->value);
		}else if(g_ptrs->token->id == IF){
			defIf();
		}else if(g_ptrs->token->id == WHILE){
			defWhile();
			break;
		}else if(g_ptrs->token->id == RETURN){
			defReturn();
		}else if(g_ptrs->token->id == ELSE){
			defElse();
		}else if(g_ptrs->token->id == ZAV_SLOZ_P){
			// zkontroluje jestli je na zasobniku }
		}
	}
	//zkontroluj jesti je zasobnik prazdny
}

void parser(tPointers *ptrs){
	tableInit(g_ptrs->main_symobol_tbl); // init globalni tabulky symbolu
	printf("init tabulkyOK\n");
	initStackIE(g_ptrs->IEStack); // init if else stacku
	printf("init stackuOK\n");
	InitList((g_ptrs->list_instr=CreateList())); // init listu instrukci
	printf("init listuOK\n");
	printf("ziskavam token\n");
	getToken_test(g_ptrs->source,g_ptrs->token);
	printf("mam token\n");
	if (g_ptrs->token->id == START){
			printf("clasify?\n");
			classify();
			printf("clasify end?\n");
		}
	else{
		print_error(E_SYN,"nazacatku neni <php");
		return;
	}
	
}
