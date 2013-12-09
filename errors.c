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
			break;
		case E_SEM_PARAM:
			break;
		case E_SEM_VAR:
			break;
		case E_SEM_DIV_ZERO:
			break;
		case E_SEM_DOUBLEVAL:
			break;
		case E_SEM_TYPE:
			break;
		case E_SEM_OTHER:
			fprintf(stderr, "Semanticka nebo behova chyba v programu!\n");
			break;
		case E_INTERN:
			fprintf(stderr, "Interni chyba interpretu!\n");
			break;
		default:
			break;
	}
	if (err_msg!=NULL)
	{	
		printf ("\nAditional description:\n---------------------------------------\n||     %s      ||\n---------------------------------------\n",err_msg);
	}
	destr_global();
	exit(err_code);
}


