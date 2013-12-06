
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

tPointers* g_ptrs;

/**Inicializace globalni tabulky pointru
 * @param tPointers: ukazatel na globalni tabuku pointru   
 */  
void init_global(tPointers *ptrs)
{
	if ((ptrs=(tPointers*)malloc(sizeof(tPointers))) == NULL)
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global");
	}
	ptrs->counter=allocString();
	//nastavit na $$\0
	if (NULL == (ptrs->token=(Ttoken*)malloc(sizeof(Ttoken))))
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global->token");
	}
	ptrs->main_symobol_tbl=NULL;
	ptrs->function_stack=NULL;
	ptrs->list_instr=NULL;
	ptrs->act_list_inst=NULL;
	ptrs->source=NULL;
	printf("%i memory.c %i\n",&(g_ptrs->source),&g_ptrs );		
}
/**Dealokace globalni tabulky pointru
 *@param tPointers: ukazatel na globalni tabuku pointru   
 */   
// void destr_global(tPointers *ptrs)
// {
	
// 	destr_tToken(ptrs->token);
// 	destr_tHashTbl(ptrs->main_symobol_tbl);
// 	destr_tHashTblStack(ptrs->function_stack);
// 	destr_tListIns(ptrs->list_instr_list);
// 	if (FILE!=NULL)
// 	{
// 		fclose(source);
// 	}
// 	if (counter!=NULL)
// 	{
// 		free(counter);
// 	}
// 	free(ptrs);
// }