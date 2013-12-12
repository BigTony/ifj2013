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
					i++;
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
  			// free(item->data);
  			// podle me to nemusime freeovat
  			// bo union zabira v pameti kolik jeho maximalni polozka takze by to melo bejt cajk
  			if(blank == 0)
  				item->data.varInt = 0;
  			else
  				item->data.varInt = cislo;
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
			item->data.varDouble = (int)item->data.varInt;
			item->type = VARDOUBLE;
			break;
		case STRING:{
			int i = 0;
			int cislo = 0;
			double desetiny_cislo = 0;
			int carka = 0;
			int blank = 0;
			int zbytek = 1;
			char *zpracuj = item->data.varString;
			while ((zpracuj[i]!='\0')){
				if((isspace(zpracuj[i]))&&(cislo==0)&&(desetiny_cislo==0)) {
					i++;
					continue;
				}
  				if (isdigit(zpracuj[i]) && (cislo<=INT_MAX)){
  					if(carka == 1){
  						desetiny_cislo=desetiny_cislo*10+(zpracuj[i]-'0');
  						zbytek = zbytek * 10;
  					}else{
  						cislo=cislo*10+(zpracuj[i]-'0');
  					}
  					blank = 1;
  					i++;
  				}else if(zpracuj[i] == '.'){
  					blank = 1;
  					i++;
  					if(carka == 0){
  						carka = 1;
  					}else{
  						break;
  					}
  				}else{
  					break;
  				}
  			}
  			desetiny_cislo = desetiny_cislo / zbytek;
  			if(blank == 0)
  				item->data.varDouble = 0.0;
  			else
  				item->data.varDouble = cislo+desetiny_cislo;
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
  	l++; 
  	value/=10;
  }
  return l;
}

void tostring(item *item){
	switch(item->type){
		case VARINT:{
			int delka = 0;
			delka = get_int_len(item->data.varInt);
			char *vysledek;
			if((vysledek =(char *) malloc((delka+1)*sizeof(char))) == NULL){
				print_error(E_INTERN,"chyba pri alokace tostring");
			}
			sprintf(vysledek, "%d", item->data.varInt);
			item->data.varString = vysledek;
			item->type = STRING;         //puvodne: item->type = VARINT;
			break;
		}
		case VARDOUBLE:{
			// delka double cisla?
			char *vysledek;
			if((vysledek = (char*)malloc(sizeof(char)*50)) == NULL){
				print_error(E_INTERN,"chyba pri alokace tostring");
			}
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


// vestavene funkce z IFJ13

void vs_boolval(tHashTbl *tab,tHashTbl *NavrTab){
    item *tHsrc1 = (TblSearch (tab, "1000000\0"));
    if(tHsrc1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_boolval");
	toVARBOOL(tHsrc1);
	TblInsert(NavrTab,"$",tHsrc1->data,tHsrc1->type);
}

void vs_doubleval(tHashTbl *tab,tHashTbl *NavrTab){
	item * tHsrc1 = (TblSearch (tab, "1000000\0"));
	if(tHsrc1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_doubleval");
	todouble(tHsrc1);
	TblInsert(NavrTab,"$",tHsrc1->data,tHsrc1->type);
}

void vs_intval(tHashTbl *tab,tHashTbl *NavrTab){
	item *tHsrc1 = (TblSearch (tab, "1000000\0"));
	if(tHsrc1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_intval");
	tovarint(tHsrc1);
	TblInsert(NavrTab,"$",tHsrc1->data,tHsrc1->type);
}

void vs_strval(tHashTbl *tab,tHashTbl *NavrTab){
	item *tHsrc1 = (TblSearch (tab, "1000000\0"));
	if(tHsrc1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_strval");
	tostring(tHsrc1);
	TblInsert(NavrTab,"$",tHsrc1->data,tHsrc1->type);
}

void vs_get_string(tHashTbl *tab,tHashTbl *NavrTab){
	int c;
	int i = 0;
	int size = ALLOC_SIZE;
	char *temp = allocString();
  	do {
  		 
    	c=getchar();
    	if(i >= size){
    		reAllocString(temp,size+ALLOC_SIZE);
    		size = size + ALLOC_SIZE;
    	}
    	temp[i] = c;
    	i++;
  	}while (!((c == '\n') || (c == EOF)));
  	temp[i-1] = '\0';
  	tokenValue tempstring;
  	tempstring.varString = temp;
  	TblInsert(NavrTab,"$",tempstring,STRING);
}

void vs_put_string(tHashTbl *tab,tHashTbl *NavrTab){
	tokenValue i;
	i.varInt = 0;
	item *tempitem;
	strcpy(g_ptrs->params,"0000000\0");
	while((tempitem = TblSearch (tab, gen_param(g_ptrs->params)))!= NULL){
		tostring(tempitem);
		printf("%s",tempitem->data.varString);
		i.varInt++;
	}
	// printf("%s",konk);
	TblInsert(NavrTab,"$",i,VARINT);
}


void vs_strlen(tHashTbl *tab,tHashTbl *NavrTab){
	item *tHsrc1 = (TblSearch (tab, "1000000\0"));
	if(tHsrc1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_strlen");
	tostring(tHsrc1);
	int delka = strlen(tHsrc1->data.varString);
	tokenValue i;
	i.varInt = delka;
	TblInsert(NavrTab,"$",i,VARINT);
}

void vs_get_substring(tHashTbl *tab,tHashTbl *NavrTab){
	// prvni parametr
	item *param1 = (TblSearch(tab,"1000000\0"));
	if(param1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_get_substring");
    if(param1->type != STRING)
    	print_error(E_SEM_PARAM,"vs_get_substring prvni param neni string");

    // druhy parametr
	item *param2 = (TblSearch(tab,"2000000\0"));
	if(param2 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_get_substring");
    if(param2->type != VARINT)
    	print_error(E_SEM_PARAM,"vs_get_substring druhy param neni int");
    if((param2->data.varInt < 0) || (param2->data.varInt >= strlen(param1->data.varString)))
    	print_error(E_SEM_PARAM,"vs_get_substring druhy param < 0 nebo vetsi jak strlen string");

    // treti parametr
	item *param3 = (TblSearch(tab,"3000000\0"));
	if(param3 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_get_substring");
    if(param3->type != VARINT)
    	print_error(E_SEM_PARAM,"vs_get_substring treti param neni int");
    if((param3->data.varInt < 0) || (param3->data.varInt > strlen(param1->data.varString)) || (param2->data.varInt > param3->data.varInt))
    	print_error(E_SEM_PARAM,"vs_get_substring druhy param < 0 nebo vetsi jak strlen string");

    // beh programu
    int delka = param3->data.varInt - param2->data.varInt;
    char *vysledek;
	if((vysledek = malloc((char)sizeof(delka+10))) == NULL){
		print_error(E_INTERN,"chyba pri alokace vs_get_substring");
	}
	int start = param2->data.varInt;
	int end = param3->data.varInt;
	int i = 0;
	while(start <= end){
		vysledek[i] = param1->data.varString[start];
		i++;
		start++;
	}
	vysledek[i] = '\0';
	tokenValue result;
	result.varString = vysledek;
	TblInsert(NavrTab,"$",result,STRING);
}

void vs_find_string(tHashTbl *tab,tHashTbl *NavrTab){
	// prvni parametr
	item *param1 = (TblSearch(tab,"1000000\0"));
	if(param1 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_get_substring");
    if(param1->type != STRING)
    	print_error(E_SEM_PARAM,"vs_get_substring prvni param neni string");

    // druhy parametr
	item *param2 = (TblSearch(tab,"2000000\0"));
	if(param2 == NULL)
    	print_error(E_SEM_PARAM,"Chybny pocet parametru ..  vs_get_substring");
    if(param2->type != STRING)
    	print_error(E_SEM_PARAM,"vs_get_substring prvni param neni string");

    // beh programu
    if((strcmp(param2->data.varString,""))==0){
    	tokenValue result;
    	result.varInt = 0;
    	TblInsert(NavrTab,"$",result,VARINT);
    	return;
    }
	tokenValue result;
	result.varInt = find_string(param1->data.varString,param2->data.varString);
	TblInsert(NavrTab,"$",result,VARINT);
}

void vs_sort_string(tHashTbl *tab,tHashTbl *NavrTab){

}




// konec vestavencyh funkci IFJ13

int find_string(char *text,char* word)
{
    int pozice=getSubstringKmp(text,word);
    int textLen=strlen(text);

    if(pozice>textLen)
    {//NENASEL
        return -1;
    }
    else//NASEL
    {
        return pozice;
    }
}


