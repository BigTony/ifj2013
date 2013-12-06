
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

tPointers* g_ptrs;

/**Inicializace globalni tabulky pointru
 * @param tPointers: ukazatel na globalni tabuku pointru   
 */  
void init_global()
{
	if ((g_ptrs=(tPointers*)malloc(sizeof(tPointers))) == NULL)
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global");
	}
	g_ptrs->counter=allocString();
	//nastavit na $$\0
	if (NULL == (g_ptrs->token=(Ttoken*)malloc(sizeof(Ttoken))))
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global->token");
	}
	g_ptrs->main_symobol_tbl=NULL;
	g_ptrs->function_stack=NULL;
	g_ptrs->list_instr=NULL;
	g_ptrs->act_list_inst=NULL;
	g_ptrs->source=NULL;
	g_ptrs->kokot=NULL;
	printf("%d memory.c %d\n",&(g_ptrs->source),&g_ptrs );		
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