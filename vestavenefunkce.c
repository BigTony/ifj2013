#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "vestavenefunkce.h"
/*
typedef union {
    int varInt;
    double varDouble;
    char *varString;
}tokenValue
*/

// pretypovani
void tovarint(item *item){
	switch(item->type){
		case VARBOOL:{
			item->type = VARINT;
			break;
		}
		case VARDOUBLE:{
			tokenValue pretyp;
			pretyp.varInt = (int) item->data.varDouble;
			item->data = pretyp;
			item->type = VARINT;
			break;
		}
		case STRING:{
			int i = 0;
			int cislo = 0;
			char *zpracuj = item->data.varString;
			int blank= 0;
			while ((zpracuj[i]!='\0')){
				if((isspace(zpracuj[i]))&&(cislo==0)) {
					continue;
				}
  				if (isdigit(zpracuj[i]) && (cislo<=INT_MAX)){
  					blank = 1;
  					cislo=cislo*10+(zpracuj[i]-'0');
  					i++;
  				}else{
  					break;
  				}
  			}
  			free(zpracuj);
  			// free(item->data);
  			// podle me to nemusime freeovat
  			// bo union zabira v pameti kolik jeho maximalni polozka takze by to melo bejt cajk
  			if(blank == 0)
  				item->data.varInt = cislo;
  			else
  				item->data.varInt = 0;
			item->type = VARINT;
			break;
		}
		case VARINT:
			break;
		default:
			print_error(E_INTERN,"chyba pri pretypovani");
			break;
	}
}

void toVARBOOL(item *item){
	switch(item->type){
		case VARINT:
			if(item->data.varInt != 0){
				item->data.varInt = 1;
			}
			item->type = VARBOOL;
			break;
		case VARDOUBLE:
			if(item->data.varDouble == 0.0){
				item->data.varInt = 0;
			}else{
				item->data.varInt = 1;
			}
			item->type = VARBOOL;
			break;
		case STRING:
			if(strcmp (item->data.varString,"") == 0){
				// free(item->data);
				item->data.varInt = 0;
			}else{
				// free(item->data);
				item->data.varInt = 1;
			}
			item->type = VARBOOL;
			break;
		case VARBOOL:
			break;
		default:
			print_error(E_INTERN,"chyba pri pretypovani");
			break;
	}
}

void todouble(item *item){
	switch(item->type){
		case VARINT:
			item->type = VARDOUBLE;
			break;
		case STRING:{
			int i = 0;
			int cislo = 0;
			int desetiny_cislo = 0;
			int carka = 0;
			int blank = 0;
			char *zpracuj = item->data.varString;
			while ((zpracuj[i]!='\0')){
				if((isspace(zpracuj[i]))&&(cislo==0)&&(desetiny_cislo==0)) {
					continue;
				}
  				if (isdigit(zpracuj[i]) && (cislo<=INT_MAX)&&(desetiny_cislo==0)){
  					if(carka == 1){
  						desetiny_cislo=desetiny_cislo+(zpracuj[i]-'0')/10;	
  					}else{
  						cislo=cislo*10+(zpracuj[i]-'0');
  					}
  					i++;
  				}else if((zpracuj[i] == '.')){
  					if(carka == 0){
  						carka = 1;
  					}else{
  						break;
  					}
  					
  				}else{
  					break;
  				}
  			}
  			// free(item->data);
  			free(zpracuj);
  			if(blank == 0)
  				item->data.varDouble = cislo+desetiny_cislo;
  			else
  				item->data.varDouble = 0.0;
			item->type = VARDOUBLE;
			break;
		}
		case VARBOOL:
			if(item->data.varInt == 0){
				item->data.varDouble = 0.0;
			}else{
				item->data.varDouble = 1.0;
			}
			item->type = VARDOUBLE;
			break;
		case VARDOUBLE:
			break;
		default:
			print_error(E_INTERN,"chyba pri pretypovani");
			break;
	}
}

int get_int_len (int value){
  int l=1;
  while(value>9){ 
  	l++; value/=10; 
  }
  return l;
}

void tostring(item *item){
	switch(item->type){
		case VARINT:{
			int delka = get_int_len(item->data.varInt);
			char vysledek[delka+1];
			sprintf(vysledek, "%d", item->data.varInt);
			item->data.varString = vysledek;
			item->type = STRING;         //puvodne: item->type = VARINT;        
			break;
		}
		case VARDOUBLE:{
			// delka double cisla?
			char vysledek[100];
			sprintf(vysledek, "%g", item->data.varDouble);
			item->data.varString = vysledek;
			item->type = STRING;        // puvodne: item->type = VARDOUBLE;
			break;
		}
		case VARBOOL:
			if(item->data.varInt == 0){
				item->data.varString = "";
			}else{
				item->data.varString = "1";
			}
			item->type = STRING;	
			break;
		case STRING:
			break;
		default:
			print_error(E_INTERN,"chyba pri pretypovani");
			break;
	}
}

void pretypovani(item *item,int to){
	switch(to){
		case VARINT:
			tovarint(item);
			break;
		case VARBOOL:
			toVARBOOL(item);
			break;
		case VARDOUBLE:
			todouble(item);
			break;
		case STRING:
			tostring(item);
			break;
		default:
			break;
	}
}




// vestavene funkce 
void VARBOOLval(item *item){
	switch(item->type){
		case VARINT:

			break;
		case VARDOUBLE:
			break;
		case STRING:
			break;
		case VARBOOL:
			break;
		case NIL:
			break;
		default:
			print_error(E_SYN,"token nelze prevest");
			break;
	}
}


