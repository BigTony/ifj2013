#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "vestavenefunkce.h"

// pretypovani
void tovarint(item *item){
	switch(item->type){
		case VARBOOL:
			item->type = VARINT;
			break;
		case VARDOUBLE:
			tokenValue.varInt kokot;
			item->data = kokot;
			item->type = VARINT;
			break;
		case STRING:
			int i = 0;
			int cislo = 0;
			while ((item->data[i]!='\0')){
				if((isspace(item->data[i]))&&(blank)&&(cislo==0)) {
					continue;
				}
  				if (isdigit(item->data[i]) && (cislo<=INT_MAX)){
  					cislo=cislo*10+(item->data[i]-'0');
  					i++;
  				}else{
  					break;
  				}
  			}
  			free(item->data);
  			if(blank == 0)
  				item->data = cislo;
  			else
  				item->data = 0;
			item->type = VARINT;
			break;
		case VARINT:
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
			break;
	}
}

void toVARBOOL(item *item){
	switch(item->type){
		case VARINT:
			if(item->data != 0){
				item->data = 1;
			}
			item->type = VARBOOL;
			break;
		case VARDOUBLE:
			if(item->data == 0.0){
				item->data = 0;
			}else{
				item->data = 1;
			}
			item->type = VARBOOL;
			break;
		case STRING:
			if(strcmp (item->data,"") == 0){
				free(item->data);
				item->data = 0;
			}else{
				free(item->data);
				item->data = 1;
			}
			item->type = VARBOOL;
			break;
		case VARBOOL:
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
			break;
	}
}

void todouble(item *item){
	switch(item->type){
		case VARINT:
			item->type = VARDOUBLE;
			break;
		case STRING:
			int i = 0;
			int c = 0;
			int cislo = 0;
			int desetiny_cislo = 0;
			int carka = 0;
			while ((item->data[i]!='\0')){
				if((isspace(item->data[i]))&&(blank)&&(cislo==0)&&(desetiny_cislo==0)) {
					continue;
				}
  				if (isdigit(item->data[i]) && (cislo<=INT_MAX)&&(desetiny_cislo==0)){
  					if(carka == 1){
  						desetiny_cislo=desetiny_cislo+(item->data[i]-'0')/10;	
  					}else{
  						cislo=cislo*10+(item->data[i]-'0');
  					}
  					i++;
  				}else if((item->data[i] == '.')){
  					if(carka == 0){
  						carka = 1;
  					}else{
  						break;
  					}
  					
  				}else{
  					break;
  				}
  			}
  			free(item->data);
  			if(blank == 0)
  				item->data = cislo+desetiny_cislo;
  			else
  				item->data = 0.0;
			item->type = VARDOUBLE;
			break;
		case VARBOOL:
			if(item->data == 0){
				item->data = 0.0;
			}else{
				item->data = 1.0;
			}
			item->type = VARDOUBLE;
			break;
		case VARDOUBLE:
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
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
		case VARINT:
			int delka = get_int_len(item->data);
			char *vysledek[delka+1];
			itoa(item->data,vysledek,10);
			item->data = vysledek;
			item->type = VARINT;
			break;
		case VARDOUBLE:
			// delka double cisla?
			char *vysledek[100];
			sprintf(vysledek, "%g", item->data);
			item->data = vysledek;
			item->type = VARDOUBLE;
			break;
		case VARBOOL:
			if(item->data == 0){
				item->data = "";
			}else{
				item->data = "1";
			}
			item->type = STRING;	
			break;
		case STRING:
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
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
		case VARVARBOOL:
			break;
		case NIL:
			break;
		default:
			print_error(E_SYN,"token nelze prevest");
			break;
	}
}


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	double a=2.132;
	char arr[23];
	sprintf(arr, "%g", a);
	printf("%s\n",arr);
	return 0;
}
