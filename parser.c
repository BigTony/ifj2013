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



#include "errors.h"

// deklarace nebo prirazeni promene
void defVar(tokenValue value){

}

// podminka if
void defIf(){

}

// cyklus while
void defWhile(){

}

// retuuurn
void defReturn(){

}

// definice funkce jsem nalitej
void defFunction(){
	
}


// volani funkce
void callFunction(tokenValue value){

}


// vyber spravnej postup pro token
void classify(){
	while(gettoken(Ttoken *token) == 0){
		if(*token.id == KONEC){
			return; // kdyz je token konec analyzy
		}
		// token je bud promena nebo volani fce
		if(*token.id == ID){
			*temp_token = *token;
			if(gettoken(Ttoken *token) == ZAV_JEDN_L){
				callFunction(*temp_token.value);
			}else{
				defVar(*temp_token.value);	
			}
		}else if(*token.id == IF){
			defIf();
		}else if(*token.id == WHILE){
			defWhile();
		}else if(*token.id == RETURN){
			defRetrun();
		}else if(*token.id == FUNCTION){
			defFunction();
		}
	}
}

void parser(ProgramState *main){
	// init vseho
	Ttoken *temp_token;
	if (gettoken(Ttoken *token) == 0){
		if (*token.id == START){
			
		}else{
			print_error(E_SYN);
			return;
		}
	}else{
		print_error(E_LEX);
		return;
	}	
}
