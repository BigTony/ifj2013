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
#include "scaner.h"
#include "errors.h"



int main(int argc,char** argv){
  
  ProgramState main;
  main.err_code = 0;
  main.table = NULL;
  main.source = NULL;
  // main.instruction = TODO

  // Testovani parametru
  if (argc != 2){
  	print_error(E_WRONG_PARAM);
  	return E_WRONG_PARAM;
  }
  // Testovani otevreni souboru
  if ((main.source = fopen(argv[1], "r")) == NULL){
    print_error(E_FILE);
    return E_FILE;
  } 

  // Provedeni syntakticke analyzy
  tableInit(&main);
  parser(&main);

  // Pri chybe vypsani chyby, a return error kodu
  if(main.err_code != E_OK){
  	print_error(main.err_code);
  	return main.err_code;
  }

  fclose(main.source);
  //freeAll(); TODO, dealakovat veskerej bullshit
  return E_OK;
}