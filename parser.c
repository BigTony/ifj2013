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
	while ((counter[i]!='\0'))
	{
		if ((counter[i]<'9'))
		{
		counter[i]++;
		break;
		}
		i++;
	}
	char* new=allocString();
	strcpy(new,counter);	
	return(new);	
}
char* gen_param(char *counter)
{
	int i=0;
	while ((counter[i]!='\0'))
	{
		if ((counter[i]<'9'))
		{
		counter[i]++;
		break;
		}
		i++;
	}
	char* new=allocString();
	strcpy(new,counter);	
	return(new);	
}

tokenValue CreateExInstruction(tokenValue value1,tokenValue value2,TIType op)
{
tokenValue new_id;
new_id.varString = gen_id(g_ptrs->counter);
InsertInstLast (g_ptrs->act_list_inst,value1.varString,value2.varString,(char*)new_id.varString,op);   
return (new_id);
}

// deklarace nebo prirazeni promene
void defVar(tokenValue value){
	char * var_name = value.varString;
	if(getToken_test(g_ptrs->source,g_ptrs->token) != PRIRAZENI){
		print_error(E_SYN,"chyba v syntaxi ocekavano = pri prirazeni promene nebo funkce");
	}else{
		if(ExEx(0,var_name)){
			callFunction(var_name);
			if(getToken_test(g_ptrs->source,g_ptrs->token) != STREDNIK){
				print_error(E_SYN,"chyba v syntaxi ocekavano ; po prirazeni promene nebo funkce");
			}
		}
	}
}


// podminka if
void defIf(){
	char* TmpExp=gen_id(g_ptrs->counter);
	char* TmpJmp=gen_id(g_ptrs->counter);
	char* TmpJmp1=gen_id(g_ptrs->counter);
	ExEx(IF,TmpExp); // vyhodnoceni vyrazu	

	InsertInstLast (g_ptrs->act_list_inst,TmpExp,NULL,TmpJmp,I_JZ);
	// TLItem *tmpItem = g_ptrs->list_instr->Last;
	// vytvoreni 3AC
	// vytvoreni 3AC podmineneho skoku1
	classify();
	InsertInstLast (g_ptrs->act_list_inst,NULL,NULL,TmpJmp1,I_JMP);
	InsertInstLast (g_ptrs->act_list_inst,NULL,NULL,NULL,I_LAB);
	tokenValue pretyp;
	pretyp.varString = (char *) g_ptrs->act_list_inst->Last;
	add_const_hashtbl(g_ptrs->main_symobol_tbl, IDENTIFIKATOR,pretyp, TmpJmp);
	//ulozeni nazvu a odkazu navesti do globalni tabulky
	if(getToken_test(g_ptrs->source,g_ptrs->token) != ELSE){
	print_error(E_SYN,"chyby else u if");
	}
	else{
		if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_SLOZ_L){
			print_error(E_SYN,"chyby leva slozena zavorka u else {");
		}
		else{
			classify();
			InsertInstLast (g_ptrs->act_list_inst,NULL,NULL,NULL,I_LAB);
			tokenValue pretyp2;
			pretyp2.varString = (char *) g_ptrs->act_list_inst->Last;
			add_const_hashtbl(g_ptrs->main_symobol_tbl, IDENTIFIKATOR,pretyp2, TmpJmp1);
		}
	}		

	return;
}


// cyklus while
void defWhile(){
		char* TmpExp=gen_id(g_ptrs->counter);
		char* TmpJmp=gen_id(g_ptrs->counter);
		char* TmpJmp1=gen_id(g_ptrs->counter);
		tokenValue pretyp;
		pretyp.varString = (char *) g_ptrs->act_list_inst->Last;
		add_const_hashtbl(g_ptrs->main_symobol_tbl, IDENTIFIKATOR, pretyp, TmpJmp);
		ExEx(IF,TmpExp); // vyhodnoceni vyrazu	
		InsertInstLast (g_ptrs->act_list_inst,TmpExp,NULL,TmpJmp1,I_JZ);
		classify();
		InsertInstLast (g_ptrs->act_list_inst,NULL,NULL,TmpJmp,I_JMP);
		InsertInstLast (g_ptrs->act_list_inst,NULL,NULL,NULL,I_LAB);
		tokenValue pretyp2;
		pretyp2.varString = (char *) g_ptrs->act_list_inst->Last;
		add_const_hashtbl(g_ptrs->main_symobol_tbl, IDENTIFIKATOR, pretyp2, TmpJmp1);			
		return;
}

// retuuurn
void defReturn(){
	   char* TmpExp=gen_id(g_ptrs->counter);
	   ExEx(0,TmpExp); // vyhodnoceni vyrazu
	   InsertInstLast (g_ptrs->act_list_inst,TmpExp,NULL,NULL,I_RETURN);
	   return;
}

// deklarace funkce
void defFunction(){
	if(getToken_test(g_ptrs->source,g_ptrs->token) != IDENTIFIKATOR){
		print_error(E_SYN,"pri deklarace funkce chyby jeji nazev");
	}else{
	   InitList((g_ptrs->act_list_inst=CreateList())); // init listu instrukci nove
	   add_const_hashtbl(g_ptrs->main_symobol_tbl, IDENTIFIKATOR, (tokenValue)(void*)g_ptrs->act_list_inst, g_ptrs->token->value.varString);
		if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_L){
			print_error(E_SYN,"pri deklaraci funkce chyby ( ");
		}else{
			strcpy(g_ptrs->params,"0000000\0");
			while(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_P){
				if(g_ptrs->token->id != VARIABLE){
					print_error(E_SYN,"pri deklaraci funkce jsou parametry jine lexemy nez promene");
				}else{
					InsertInstLast (g_ptrs->act_list_inst,gen_param(g_ptrs->params),NULL,g_ptrs->token->value.varString,I_CHCKPAR);
				}
				if(getToken_test(g_ptrs->source,g_ptrs->token) != CARKA){
					break;
				}
			}
			if (g_ptrs->token->id != ZAV_JEDN_P){
				print_error(E_SYN,"pri deklaraci funkce je za poslednim parametetrem ocekavana ) ");	
			}
			if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_SLOZ_L){
				print_error(E_SYN,"pri deklaraci funkce chyby {");
			}else{
				classify();	
			}
			g_ptrs->act_list_inst = g_ptrs->list_instr;	
		}
	}
	return;
}

// volani funkce
void callFunction(char* dest){
	char * nazevFce = g_ptrs->token->value.varString;
	InsertInstLast (g_ptrs->act_list_inst,(char*)g_ptrs->token->value.varString,NULL,NULL,I_TSW);// vygenerujeme 3AC pro nazev funkce a pro prepnuti tabulky
	if(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_L){
		print_error(E_SYN,"pri volani funkce chyby (");
	}else{
		strcpy(g_ptrs->params,"0000000\0");
		while(getToken_test(g_ptrs->source,g_ptrs->token) != ZAV_JEDN_P){
			if(!((g_ptrs->token->id > IDENTIFIKATOR) && (g_ptrs->token->id <= NIL))){
				print_error(E_SYN,"pri volani funkce jsou parametry jine termy");
			}else{
				if (g_ptrs->token->id==VARIABLE)
				{
					InsertInstLast (g_ptrs->act_list_inst,(char*)g_ptrs->token->value.varString,NULL,gen_param(g_ptrs->params),I_PARAM);	
				}
				else
				{
					char*Tmp=gen_id(g_ptrs->counter);
					add_const_hashtbl(g_ptrs->main_symobol_tbl,g_ptrs->token->id, g_ptrs->token->value,Tmp);
					InsertInstLast (g_ptrs->act_list_inst,(char*)g_ptrs->token->value.varString,NULL,gen_param(g_ptrs->params),I_PARAM);
				}
			}
			if(getToken_test(g_ptrs->source,g_ptrs->token) != CARKA){
				break;
			}
		}
		if (g_ptrs->token->id != ZAV_JEDN_P){
			print_error(E_SYN,"pri volani funkce je za poslednim parametetrem ocekavana ) ");	
		}
		InsertInstLast (g_ptrs->act_list_inst,nazevFce,NULL,NULL,I_CALL);
		InsertInstLast (g_ptrs->act_list_inst,"$",NULL,dest,I_ASS);
	}
	
	
}

// vyber spravnej postup pro token
void main_classify(){
	printf("sem v main clasify\n");
	while (getToken_test(g_ptrs->source,g_ptrs->token)!=KONEC)
	{
		printf("sem ve whilu main clasify\n");	
		// token je bud promena nebo volani fce
		if(g_ptrs->token->id == VARIABLE){
			printf("VARIABLE\n");
			defVar(g_ptrs->token->value);	
		}else if(g_ptrs->token->id == FUNCTION){
			defFunction(g_ptrs->token->value);
		}else if(g_ptrs->token->id == IF){
			defIf();
		}else if(g_ptrs->token->id == WHILE){
			defWhile();
		}else if(g_ptrs->token->id == RETURN){
			printf("nasel sem return mainclasify\n");
			defReturn();
		}else{
		   print_error(E_SYN,"neocekavany token v main_clasify");
		}
	}
}



// vyber spravnej postup pro token
void classify(){
	printf("sem v clasify\n");
	while (getToken_test(g_ptrs->source,g_ptrs->token)!= ZAV_SLOZ_P)
	// zkontroluje jestli je token }, kdyz jo tak konci
	{
		printf("sem ve whilu clasify\n");		
		// token je bud promena nebo volani fce
		if(g_ptrs->token->id == VARIABLE){
			printf("VARIABLE\n");
			defVar(g_ptrs->token->value);	
		}else if(g_ptrs->token->id == IF){
			defIf();
		}else if(g_ptrs->token->id == WHILE){
			defWhile();
		}else if(g_ptrs->token->id == RETURN){
			printf("nasel sem return v clasify\n");
			defReturn();
		}
			
		else{
		   print_error(E_SYN,"neocekavany token v clasify");
		}
	}
}

void parser(tPointers *ptrs){
	tableInit(&(g_ptrs->main_symobol_tbl)); // init globalni tabulky symbolu
	InitList((g_ptrs->list_instr=CreateList())); // init listu instrukci
	g_ptrs->act_list_inst = g_ptrs->list_instr;	
	getToken_test(g_ptrs->source,g_ptrs->token);
	if (g_ptrs->token->id == START){
			printf("clasify?\n");
			main_classify();
			printf("clasify end?\n");
			printf("-----zaciname interpretovat------\n");
			interpret(g_ptrs->main_symobol_tbl, g_ptrs->list_instr);
		}
	else{
		print_error(E_SYN,"nazacatku neni <php");
		return;
	}
	
}
