#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

// globanilni promena
tPointers *ptrs;

/**Inicializace globalni tabulky pointru
 * @param tPointers: ukazatel na globalni tabuku pointru   
 */  
void init_global(tPointers *ptrs)
{
	if (NULL == (ptrs=malloc(sizeof(tPointers))))
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global");
	}
	ptrs->counter=allocString();
	//nastavit na $$\0
	ptrs->token=NULL;
	ptrs->main_symobol_tbl=NULL;
	ptrs->function_stack=NULL;
	ptrs->list_instr=NULL;
	ptrs->source=NULL;
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