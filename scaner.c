// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// scaner.c
//
//

#include "scaner.h"
#include <stdlib.h>
#include "errors.h"



//////
void skipSpace(char *c,FILE *fp)
{
    char _c=*c;
    do{
        _c=(char)fgetc(fp);
    }
     while((_c!=EOF) && (isspace(_c)!=0));
     *c=_c;
}

/*void horner(char *w,Ttoken *token,short which)
{
    if (which == VARDOUBLE)
    {
        token->value.varDouble=atof(w);
    }
    else if(which == VARINT)
    {
        token->value.varInt=atoi(w);
    }
    return;
}*/

/////////////////
//FUNKCE getToken(FILE *f)
//FILE *f je zdrojovy kod jazyka IFJ13 predavany ridicim programem
//
/////////////
int getToken(FILE *fp,Ttoken *token){
    char* w;
    char c;
    int len=0;
    if((w = malloc(sizeof(char) * BUFF))==NULL)   //Alokace pro retezec
    {
        return ALLOC_ERR;
    }
    //Ttoken token = malloc(sizeof(token));  //Alokace pro token (Neni lepsi obdrzet od zadatele?)


    skipSpace(&c,fp);


    switch( c )
    {
        case '\0':
            token->id = KONEC;
            w[0]='\0';
            token->value.varString=w;
        case '{' :
            token->id = ZAV_SLOZ_L;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case '}' :
            token->id = ZAV_SLOZ_P;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case ';' :
            token->id = STREDNIK;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case '(' :
            token->id = ZAV_JEDN_L;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case ')' :
            token->id = ZAV_JEDN_P;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case '+' :
            token->id = PLUS;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case '-' :
            token->id = MINUS;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case '/' :
            token->id = DELENO;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
        	return 0;
        case '.' :
            token->id = TECKA;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
            return 0;
        case ',' :
            token->id = CARKA;
            w[len]=c; w[len+1]='\0';
            break;

        case '*' :
            token->id = KRAT;
            w[len]=c; w[len+1]='\0';
            token->value.varString = w;
            free(w);
            return E_OK;
            break;
        default:
            break;

    }
    if(c=='<')
    {
        w[len]=c;
        len++;
        c=fgetc(fp);
        if(c== '=')
        {
            w[len]=c;
            len++; w[len+1]='\0';
            token->value.varString = w;
            token->id=MENSI_ROVNO;
            free(w);
        }
        else if(c=='?') //START?
        {
            w[len]=c;
            len++;
            if(c=='p'){
                w[len]=c;
                len++;
                c=fgetc(fp);
                if(c=='h'){
                   w[len]=c;
                   len++;
                   c=fgetc(fp);
                   if(c=='p'){
                       w[len]=c;
                       len++;
                       c=fgetc(fp);
                       if(isspace(c)!=0){
                           w[len+1]='\0';
                           token->value.varString = w;
                           token->id=START;
                           free(w);
                           return E_OK;
                        }
                        else{
                            free(w);
                            return E_LEX;
                        }
                   }
                   else{
                    free(w);
                    return E_LEX;
                    }
                }
                else{
                   free(w);
                   return E_LEX;
                }
            }
            else{
                free(w);
                return E_LEX;
            }
        }
        else
        {
            ungetc(c,fp);
            w[len]='\0';
            len--;
            token->value.varString = w;
            token->id = MENSI;
            free(w);
            return E_OK;
        }

    }
    if( (c=='_') || isalpha(c)!=0)   // IDENTIFIKATOR ?
    {
        w[len]=c;
        do
        {
            c=fgetc(fp);
            len++;
            w[len]=c;
        }
        while(isalnum(c)!=0 || c=='_');
        //navraceni nasledujiciho znaku ZA identifikatorem (kvuli dowhile)
        //ulozeni a odeslani tokenu
        ungetc(c,fp);
        w[len]='\0';
        len--;
        token->id=FUNCTION_CALL;
        token->value.varString = w;
        free(w);
        return E_OK;

    }
    if( (isdigit(c)!=0) )
    {
        w[len]=c;
        do
        {
            c=fgetc(fp);
            len++;
            w[len]=c;

        }while(isdigit(c)!=0);
        if(c=='.')
        {
            do
            {
                c=fgetc(fp);
                len++;
                w[len]=c;

            }while(isdigit(c)!=0);
            w[len]='\0';
        }

        else if(c=='+'|| c=='-')
        {
            //len++;
            //w[len]=c;
            /*
            TADY PREVEST NA CISLO PRVNI CAST
            */
            c=fgetc(fp);
            if(c=='e')
            {
                len=0;
                w=NULL;
                //    len++;
                //    w[len]=c;
                do
                {
                    c=fgetc(fp);
                    len++;
                    w[len]=c;
                }while(isdigit(c)!=0);
                w[len]='\0';
            }
            else
            {
                free(w);
                return 1; //RETURN E_OK
            }

        }
        free(w);
        return E_LEX;
    }
    else{
        free(w);
        return E_LEX;
    }



}





int main(int argc,char** argv){

  /*ProgramState main;
  main.err_code = 0;
  main.table = NULL;
  main.source = NULL;
  // main.instruction = TODO
*/
  // Testovani parametru
  /*if (argc != 2){
  	print_error(E_WRONG_PARAM);
  	return E_WRONG_PARAM;
  }*/
  // Testovani otevreni souboru
  FILE *fp;
  if ((fp = fopen("test.txt", "r")) == NULL){
    printf("error fopen");
    return 0;
  }

 // Ttoken token = malloc(sizeof(token));  //Alokace pro token (Neni lepsi obdrzet od zadatele?)
    Ttoken token;
    token.value.varString = malloc(sizeof(char)*BUFF);
  //  getToken(fp,&token);
  do{
      token.id = 0;
      token.value.varString = NULL;
      token.value.varInt = 0;
      token.value.varDouble = -1.0;

        getToken(fp,&token);

      if(token.value.varString!=NULL)
      {
        printf("%\n         ",token.id);
        printf("%s\n",token.value.varString);
      }

      if(token.value.varInt>0)
      {
        printf("%\n         ",token.id);
        printf("%d\n",token.value.varInt);
      }

      if(token.value.varDouble>=0.0)
      {
        printf("%\n         ",token.id);
        printf("%f\n",token.value.varDouble);
      }



  }while(token.id != KONEC);

    free(token.value.varString);

  // Provedeni syntakticke analyzy
 // tableInit(&main);
  //parser(&main);
    //scanner
  // Pri chybe vypsani chyby, a return error kodu
  /*if(main.err_code != E_OK){
  	print_error(main.err_code);
  	return main.err_code;
  }

  fclose(main.source);
  //freeAll(); TODO, dealakovat veskerej bullshit*/
  return 1;
}

