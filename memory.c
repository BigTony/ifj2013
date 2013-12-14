
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
	strcpy(g_ptrs->counter,"$$00000\0" );
	g_ptrs->params=allocString();
	strcpy(g_ptrs->params,"0000000\0" );
	//nastavit na $$aaaaa\0
	if (NULL == (g_ptrs->token=(Ttoken*)malloc(sizeof(Ttoken))))
	{
		print_error(E_INTERN,"Chyba alokace pameti: init_global->token");
	}
	g_ptrs->main_symobol_tbl=NULL;
	g_ptrs->function_table = NULL;
	g_ptrs->function_stack=NULL;
	g_ptrs->list_instr=NULL;
	g_ptrs->act_list_inst=NULL;
	g_ptrs->source=NULL;	
}
/**Dealokace globalni tabulky pointru
 *@param tPointers: ukazatel na globalni tabuku pointru   
 */   
void destr_global(){	
	if (g_ptrs->source!=NULL){
 		fclose(g_ptrs->source);
	}
	if (g_ptrs->counter!=NULL){
	 	free(g_ptrs->counter);
	}
	if (g_ptrs->function_table != NULL){
		TblDeleteFunction(g_ptrs->function_table);
	}
	// if (g_ptrs->main_symobol_tbl != NULL){
	// 	TblDeleteMain(g_ptrs->main_symobol_tbl);
	// }
	if (g_ptrs->function_stack != NULL){
		freeStack(g_ptrs->function_stack);
	}
	if ((g_ptrs->list_instr != NULL) && (g_ptrs->list_instr != g_ptrs->act_list_inst)){
		free(g_ptrs->list_instr);
	}
	if (g_ptrs->act_list_inst != NULL){
		free(g_ptrs->act_list_inst);
	}
	free(g_ptrs);
}