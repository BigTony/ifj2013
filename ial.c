// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// ial.c
// Iplementation sort and find functions
//


#include "errors.h"

void tableInit(ProgramState *main){
	tHashTbl *hash_table;
	if((hash_table = malloc(sizeof(tHashTbl)))==NULL){
		main->err_code = ALLOC_ERR;
		return;
	}
	if((hash_table->tableItems = malloc(ALLOC*sizeof(item)))==NULL){
		main->err_code = ALLOC_ERR;
		return;
	}
	hash_table->size = ALLOC;
	main->table = hash_table;
}

