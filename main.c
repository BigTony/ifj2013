// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// main.c
// Main program using other functions
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "errors.h"
#include "memory.h"

int main(int argc,char** argv){
  // inicializace globalni pametove tabulky
  // printf("%i main.c1 %i\n",&(g_ptrs->source),&(g_ptrs));
  init_global();
  // Testovani parametru
  if (argc != 2){
  	print_error(E_INTERN,"spatny parametr pri spousteni");
  }
  // Testovani otevreni souboru
  if ((g_ptrs->source = fopen(argv[1], "r")) == NULL){
    print_error(E_INTERN,"nelze otevrit soubor");
  } 
  printf("lezu do parseru\n");
  // Provedeni syntakticke analyzy
  parser(g_ptrs);

	//all ok SO FINISH HIM
  destr_global();
  return E_OK;
}