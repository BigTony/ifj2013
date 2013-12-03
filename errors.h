#include "ial.h"
#include <stdio.h>


// Error kody
#define E_OK 0
#define E_LEX 1 // Chyba v programu v ramci lexikalni analyzy
#define E_SYN 2 // Chyba v programu v ramci syntaktice analyzy
#define E_SEM_FCE 3 // Nedefinovana funkce, pokus o redefinici funkce
#define E_SEM_PARAM 4 // Chybejici parametr pri volani funkce
#define E_SEM_VAR 5 // Nedeklarovana promena
#define E_SEM_DIV_ZERO 10 // Deleni nulou
#define E_SEM_DOUBLEVAL 11 // Chyba pri pretypovani na cislo
#define E_SEM_TYPE 12 // Chyba typove kompatibility v aritmetickych a relacnich vyrazech
#define E_SEM_OTHER 13 // Ostatni semanticke chyby 
#define E_INTERN 99 // Interni chyba interpretu 
// meine chyby
 

// Funkce pro obsluhu error kodu
void print_error(int err_code);
// Funkce pro alokaci, inicializaci a dealokaci globalnich promenych
void init_global(tPointers *ptrs);
void dealloc_global(tPointers *ptrs);
// Pole pro alokacni funkce
typedef void (*ptrDealoc) (tPointers *ptrs);
// Pole pro dealokacni fu 





// struktura pro dealokaci a errory
// vsechno co se naalokuje a vyuziva se mezi jednotlivimi moduly se ulozi sem
typedef struct {
	Ttoken *token;	//predavany token
	tHashTbl *main_symobol_tbl;	//hlavni tabulka symbolu
	tHashTblStack *function_stack;	//zasobnik tabulek symbolu pro funkce 
	tListIns *list_instr_list;	//List listu instrukci
	FILE *source;	//ukazatel na v vstupni soubor
}tPointers;


extern tPointers *ptrs;