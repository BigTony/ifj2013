#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "errors.h"

/** Funkce pro vypis chyb a ukonceni programu
 *	@param err_code: Kod chyby
 *	@param err_msg: Doplnujici chybova hlaska 
 */
void print_error(int err_code, char* err_msg){
	switch(err_code) {
		case E_WRONG_PARAM:
			fprintf(stderr, "Chybne zadane parametry prikazove radky!\n");
			break;
		case E_FILE:
			fprintf(stderr, "Chyba se souborem!\n");
			break;
		case E_LEX:
			fprintf(stderr, "Chyba v programu v ramci lexikalni analyzy!\n");
			break;
		case E_SYN:
			fprintf(stderr, "Chyba v programu v ramci syntakticke analyzy!\n");
			break;
		case E_SEM_FCE:
		case E_SEM_PARAM:
		case E_SEM_VAR:
		case E_SEM_DIV_ZERO:
		case E_SEM_DOUBLEVAL:
		case E_SEM_TYPE:
		case E_SEM_OTHER:
			fprintf(stderr, "Semanticky nebo behova chyba v programu!\n");
			break;
		case E_INTERN:
			fprintf(stderr, "Interni chyba interpretu!\n");
			break;
		default:
			break;
	}
	if (err_msg!=NULL)
	{	
		printf ("Description: %s\n",err_msg);
	}
	exit(err_code);
}

/**Inicializace globalni tabulky pointru
 * @param tPointers: ukazatel na globalni tabuku pointru   
 */  
void init_global(tPointers *ptrs);
{
	if (NULL == (ptrs=malloc(sizeof(tPointers)))
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global");
	}
	ptrs->counter=allocString();
	//nastavit na $$\0
	ptrs->token=NULL;
	ptrs->main_symobol_tbl=NULL
	ptrs->function_stack=NULL;
	ptrs->list_instr_list=NULL;
	ptrs->FILE=NULL;
}
/**Dealokace globalni tabulky pointru
 *@param tPointers: ukazatel na globalni tabuku pointru   
 */   
void destr_global(tPointers *ptrs)
{
	
	destr_tToken(ptrs->token);
	destr_tHashTbl(ptrs->main_symobol_tbl);
	destr_tHashTblStack(ptrs->function_stack);
	destr_tListIns(ptrs->list_instr_list);
	if (FILE!=NULL)
	{
		fclose(source);
	}
	if (counter!=NULL)
	{
		free(counter);
	}
	free(ptrs);
}