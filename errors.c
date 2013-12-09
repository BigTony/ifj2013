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
		printf ("Aditional description:\n---------------------------------------\n%s\n",err_msg);
	}
	destr_global();
	exit(err_code);
}


