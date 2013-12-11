// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// strings.c
// 
//

/**
*	Knihovna pro praci s retezci
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strings.h"


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

char * reAllocString(char *s,int length){
	char *temp = NULL;
	temp = (char *) realloc(s, length+1);
	if ((temp == NULL) && (s)){
		free(s);
		print_error(E_INTERN,"chyba pri realokaci stirngu");
	}
	s = temp;
	return s;
}

char * konkatenace(char*prvni,char*druhy){
	int delka = 0;
	delka = strlen(prvni)+strlen(druhy);
	char *vysledek;
	if((vysledek = (char *)malloc(sizeof(char) * delka+1)) == NULL){
		print_error(E_INTERN,"chyba pri alokaci konkatenace");
	}
	strcpy(vysledek,prvni);
	strcat(vysledek,druhy);
	return vysledek;
}

// nalezeni podretezce v retezci
char * get_substring(char* retezec, int pozice, int delka) 
{
 char* substring;
 char* ret=NULL;

 if ((substring=(char*)malloc(sizeof(char)*(delka+1)))!=NULL) 
 {
     int i=0;
     while (i<(pozice-1)) 
     {
         (retezec)++; // navysim adresu na pozadovanou
         i++;
     }

     int k=0;
     while (k<delka) 
     {
        *(substring+k) = *(retezec);
         (retezec)++;
     }

   // jeste ukoncovaci znak
   *(substring+k) = '\0';
   ret = substring;
 }
 else {
   ret = NULL;
 }
 
return ret;
}



