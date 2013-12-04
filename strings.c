// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// interpret.c
// 
//

/**
*	Knihovna pro praci s retezci
*/


#include "strings.h"
#define ALLOC_SIZE 8

/**
 * Naalokuje jeden string, a vrati ukazatel na nej
 *
 */

char * allocString(){
	char *temp = malloc(ALLOC_SIZE*sizeof(char));
	if(temp == NULL){
		print_error(E_INTERN,"chyba pri alokaci stringu");
	}
	strcpy(temp,"\0");
	return temp;
}

/**
* Uvolni string
*/
void freeString(char *s){
	if(s != NULL){
		free(s);
	}
}

char * reallocString(char *s,int length){
	char *temp = NULL;
	temp = (char *) realloc((void *) s, length+1);
	if ((temp == NULL) && (s)){
		free(s);
		print_error(E_INTERN,"chyba pri realokaci stirngu");
	}
	s = temp;
	return s;
}

/**
* Vyprazdni retezec
*/


/**
* Zkontroluje zda je retezec prazdny
*/


/**
*	strcmp // porovnani
*	strcpy // kopirovani
*/