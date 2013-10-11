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
#include "scaner.h"
#include "errors.h"


int main(int argc,char** argv)
{
  FILE *source;
  // Testovani parametru
  if (argc == 1 || argc > 2){
  	print_error(E_WRONG_PARAM);
  	return E_WRONG_PARAM;
  }
  // Testovani otevreni souboru
  if ((source = fopen(argv[1], "r")) == NULL){
    print_error(E_FILE);
    return E_FILE;
  } 

  // Provedeni syntakticke analyzy
  int result;
  //result = parser(); TODO :D

  // Pri chybe vypsani chyby, a return error kodu
  if(result != E_OK){
  	print_error(result);
  	return result;
  }
  

  fclose(source);
  //freeAll(); TODO, dealakovat veskerej bullshit
  return E_OK;
}