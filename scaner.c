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

#include "scaner.h"

#include <stdio.h>
#include <ctype.h>
#include <stdio.h>




/////////////////
//FUNKCE getToken(FILE *f)
//FILE *f je zdrojovy kod jazyka IFJ13 predavany ridicim programem
//
/////////////
char skipSpace(char *c,FILE *fp)
{
    while(c!=EOF && isspace(c)!=0)
    {
        c=fgetc(fp);
    }
    return c;
}

/////////////////
//FUNKCE getToken(FILE *f)
//FILE *f je zdrojovy kod jazyka IFJ13 predavany ridicim programem
//
/////////////
token getToken(FILE *fp){
    char* w;
    if((w = malloc(sizeof((char)*BUFF)))   //Alokace pro retezec
    {
        return ERROR;
    }
    
    token n;
    n = malloc(sizeof(token));  //Alokace pro token (Neni lepsi obdrzet od zadatele?)
    
    char c;
    char c_dalsi;
        
    skipSpace(&c);
    
    switch( c )
    {
        case '{' :
            n.id = ZAV_SLOZ_L;
            n.tokenValue = c;
        	return n;
        case '}' :
            n.id = ZAV_SLOZ_P;
            n.tokenValue = c;
        	return n;
        case ';' :
            n.id = STREDNIK;
            n.tokenValue = c;
        	return n;
        case '(' :
            n.id = ZAV_JEDN_L;
            n.tokenValue = c;
        	return n;
        case ')' :
            n.id = ZAV_JEDN_P;
            n.tokenValue = c;
        	return n;
        case '+' :
            n.id = PLUS;
            n.tokenValue = c;
        	return n;
        case '-' :
            n.id = MINUS;
            n.tokenValue = c;
        	return n;
        case '/' :
            n.id = DELENO;
            n.tokenValue = c;
        	return n;
        case '.' :
            n.id = TECKA;
            n.tokenValue = c;
            return n;
        case '*' :
            n.id = KRAT;
            n.tokenValue = c;
            return n;
        case '\'' :
            n.id = APOST;
            n.tokenValue = c;
            return n;
        case '\"' :
            n.id = UVOZ;
            n.tokenValue = c;
            return n;
        case '<' :
            n.id = ZAV_JEDN_P;
            n.tokenValue = c;
            return n;
        default:
            break;
        
    }
    if( (c=='_') || isalpha(c)!=0)   // IDENTIFIKATOR ?
    {
    
        do
        {
            c=fgetc(fp);
            w=w+c;
        }
        while(isalnum(c)!=0 || c=='_');
        //navraceni nasledujiciho znaku ZA identifikatorem (kvuli dowhile)
        //ulozeni a odeslani tokenu
        ungetc(c,fp);
        w=w-c;
        n.id=ID;
        n.tokenValue=w;
        return n;
                
    }

}