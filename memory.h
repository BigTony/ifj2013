// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin - xseidl01
// Slunsky Tomas - xsluns01
// Skacel David - xskace12
// Skyva Petr - xskyva02
////////////////////////////////
// memory.h
//
//
#ifndef MEMORY_H
#define MEMORY_H
#include "ilist.h"
#include "ial.h"
#include "scaner.h"
#include "adt.h"
#include "strings.h"
#include <stdio.h>
// struktura pro dealokaci a errory
// vsechno co se naalokuje a vyuziva se mezi jednotlivimi moduly se ulozi sem
typedef struct tPointers{
	Ttoken *token;	//predavany token
	struct tHashTbl *main_symobol_tbl;	//hlavni tabulka symbolu
	struct tHashTbl *function_table; // tabulka funkci
	struct tHashTblStack *function_stack;	//zasobnik tabulek symbolu pro funkce
	struct TList *list_instr;	//Ukazatel List main instrukci 
	struct TList *act_list_inst; //Ukazatel na aktulani instruction list
	struct tIfElseStack *IEStack; //Ukazatel na If else stack 
	FILE *source;	//ukazatel na v vstupni soubor
	char * counter;
	char * params;
}tPointers;

extern tPointers* g_ptrs;

// Funkce pro alokaci, inicializaci a dealokaci globalnich promenych
void init_global();
void destr_global();
// Pole pro alokacni funkce
typedef void (*ptrDealoc) (tPointers *ptrs);
// Pole pro dealokacni funkce 
#endif