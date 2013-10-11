/**
*	Knihovna pro praci s retezci
*/


#include "strings.h"
#define ALLOC_SIZE 8

/**
 * Naalokuje jeden string
 *
 */
int allocString(String *s){
   return (String *)malloc(sizeof(String));
}

/**
* Uvolni string
*/
void freeString(String *s){
	free(s->str);
}

/**
* Vyprazdni retezec
*/
void clearString(String *s){
   s->str[0] = '\0';
   s->length = 0;
}

/**
* Zkontroluje zda je retezec prazdny
*/
bool isEmptyString(String *s){
	if(s->data == NULL || s->length == 0){
		return true;
	}
	return false;
}

/**
*	strcmp // porovnani
*	strcpy // kopirovani
*/