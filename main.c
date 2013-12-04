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


int main(int argc,char** argv){
  // inicializace globalni pametove tabulky
  init_global(ptrs);

  // Testovani parametru
  if (argc != 2){
  	print_error(E_WRONG_PARAM);
  }
  // Testovani otevreni souboru
  if ((ptrs->source = fopen(argv[1], "r")) == NULL){
    print_error(E_FILE);
  } 

  // Provedeni syntakticke analyzy
  parser(ptrs);

	//all ok
  return E_OK;
}