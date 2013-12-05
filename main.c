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

tPointers *g_ptrs=NULL;

int main(int argc,char** argv){
  // inicializace globalni pametove tabulky
  init_global(g_ptrs);

  // Testovani parametru
  if (argc != 2){
  	print_error(E_INTERN,"spatny parametr pri spousteni");
  }
  printf ("%d/n",g_ptrs.source);
  // Testovani otevreni souboru
  if ((g_ptrs->source = fopen(argv[1], "r")) == NULL){
    print_error(E_INTERN,"nelze otevrit soubor");
  } 

  // Provedeni syntakticke analyzy
  parser(g_ptrs);

	//all ok
  return E_OK;
}