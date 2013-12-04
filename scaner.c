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
#include "errors.h"
#include "scaner.h"
#include <stdlib.h>
#include <string.h>




//////

int reallocString(char** w)
{
    char* wPom;
    if((wPom = malloc(sizeof(char) * BUFF))==NULL)   //Alokace pro retezec
    {
        print_error(E_INTERN,"LexAllocError");
    }
    wPom = realloc(*w,strlen(*w)+BUFF);
    if(wPom==NULL)
    {
        print_error(E_INTERN,"LexAllocError");
    }
    else
    {
        *w=wPom;
    }

    return E_OK;
}

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
        print_error(E_INTERN,"LexAllocError");
    }
    //Ttoken token = malloc(sizeof(token));  //Alokace pro token (Neni lepsi obdrzet od zadatele?)


    skipSpace(&c,fp);
    while(1)
    {
        /**
        *Komentare
        */
        if(c=='/')
        {
            c=fgetc(fp);
            if(c=='/')
            {
                while((c!='\n') && (c!=(char)-1))
                {
                    c=fgetc(fp);
                }
            }
            else if(c=='*')
            {
                c=fgetc(fp);
                char cPom[2];
                cPom[0]=c;
                cPom[1]=fgetc(fp);
                while(!(cPom[0]=='*'&&cPom[1]=='/')&&cPom[1]!=(char)-1)
                {
                    cPom[0]=cPom[1];
                    cPom[1]=fgetc(fp);
                }
                //skipSpace(&c,fp);
            }
            else
            {
                print_error(E_LEX,"LexERROR");
                return E_LEX;
            }
            skipSpace(&c,fp);
            continue;
        }
        ////////////
        //////////////////
        switch( c )
        {
            case (char)-1: //KONEC
                token->id = KONEC;
                w[0]=c;
                token->value.varString=w;
                return token->id;
            case '{' :
                token->id = ZAV_SLOZ_L;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case '}' :
                token->id = ZAV_SLOZ_P;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                free(w);
                return token->id;
            case ';' :
                token->id = STREDNIK;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case '(' :
                token->id = ZAV_JEDN_L;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case ')' :
                token->id = ZAV_JEDN_P;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case '+' :
                token->id = PLUS;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case '-' :
                token->id = MINUS;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case '/' :
                token->id = DELENO;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case '.' :
                token->id = TECKA;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;
            case ',' :
                token->id = CARKA;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;

            case '*' :
                token->id = KRAT;
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                return token->id;

            default:
                break;

        }

        //////////////
        //>=    //////
        //////////////
        if(c=='>')
        {
            w[len]=c;
            len++;
            c=fgetc(fp);
            if(c== '=')
            {
                w[len]=c; w[len+1]='\0';
                token->value.varString = w;
                token->id=VETSI_ROVNO;
                return token->id;
            }
            else
            {
                w[len]='\0';
                ungetc(c,fp);
                token->value.varString = w;
                token->id=VETSI;
                return token->id;
            }

        }
    ///^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^///

        ///////////////////
        //=== / =       //
        //////////////////
        if(c=='=')
        {
            w[len]=c;
            len++;
            c=fgetc(fp);
            if(c=='=')
            {
                w[len]=c;
                len++;
                c=fgetc(fp);
                if(c=='=')
                {
                    w[len]=c;
                    len++; w[len]='\0';
                    token->value.varString = w;
                    token->id=ROVNO;
                    return token->id;
                }
                else
                {
                    free(w);
                    print_error(E_LEX,"LexERROR"); return E_LEX;
                }
            }
            else
            {
                w[len]='\0';
                token->value.varString = w;
                token->id=PRIRAZENI;
                return token->id;
            }
        }
    ///^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^///

        ///////////////////
        // !==   NEROVNO//
        //////////////////
        if(c=='!')
        {
            w[len]=c;
            len++;
            c=fgetc(fp);
            if(c=='=')
            {
                w[len]=c;
                len++;
                c=fgetc(fp);
                if(c=='=')
                {
                    w[len]=c;
                    len++; w[len]='\0';
                    token->value.varString = w;
                    token->id=NEROVNO;
                    return token->id;
                }
                else
                {
                    free(w);
                    print_error(E_LEX,"LexERROR"); return E_LEX;
                }
            }
            else
            {
                free(w);
                print_error(E_LEX,"LexERROR"); return E_LEX;
            }
        }
    ///^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^///



        //////////////////////
        //START // <?php    //
        //////////////////////
        if(c=='<')
        {
            w[len]=c;
            len++;
            //printf("%c",c);
            c=fgetc(fp);
            if(c== '=')
            {
                w[len]=c;
                len++; w[len]='\0';
                token->value.varString = w;
                token->id=MENSI_ROVNO;
                return token->id;
            }
            else if(c=='?') //START?
            {
                w[len]=c;
                len++;
                c=fgetc(fp);
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
                               w[len]='\0';
                               token->value.varString = w;
                               token->id=START;
                               return token->id;
                            }
                            else{
                                free(w);
                                print_error(E_LEX,"LexERROR"); return E_LEX;
                            }
                       }
                       else{
                        free(w);
                        print_error(E_LEX,"LexERROR"); return E_LEX;
                        }
                    }
                    else{
                       free(w);
                       print_error(E_LEX,"LexERROR"); return E_LEX;
                    }
                }
                else{
                    free(w);
                    print_error(E_LEX,"LexERROR"); return E_LEX;
                }
            }
            else
            {
                w[len]='\0';
                ungetc(c,fp);
                len--;
                token->value.varString = w;
                token->id =MENSI;
                return token->id;
            }

        }
        ////////////////
        //end START  //
        ///////////////


        /////////////////
        //VARIABLE//
        /////////////////
        if(c=='$')
        {
            w[len]=c;
            len++;
            c=fgetc(fp);
            if( (c=='_') || isalpha(c)!=0)   // IDENTIFIKATOR ?
            {
                w[len]=c;
                do
                {
                    if(len%10==9)
                    {
                      if(reallocString(&w)!=E_OK)
                      {
                          print_error(E_LEX,"LexERROR"); return E_LEX;
                      }
                    }
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
                token->id=VARIABLE;
                token->value.varString = w;
               // free(w);
                return token->id;

            }
            else
            {
                free(w);
                print_error(E_LEX,"LexERROR"); return E_LEX;
            }
        }
        ///^^^^^^/////////////
        ///End VARIABLE/



        /////////////////
        //IDENTIFIKATOR//
        /////////////////
        if( (c=='_') || isalpha(c)!=0)   // IDENTIFIKATOR ?
        {
            w[len]=c;
            do
            {
                if(len%10==9)
                {
                  if(reallocString(&w)!=E_OK){print_error(E_LEX,"LexERROR"); return E_LEX;}
                }
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
            if((strcmp(w,"if")==0)||(strcmp(w,"else")==0)||(strcmp(w,"while")==0)||(strcmp(w,"return")==0
                )||(strcmp(w,"function")==0)||(strcmp(w,"null")==0)||(strcmp(w,"true")==0)||(strcmp(w,"false")==0))
            {
                free(w);
                print_error(E_LEX,"LexERROR"); return E_LEX;
            }
            else
            {
                token->id=IDENTIFIKATOR;
                token->value.varString = w;
               // free(w);
                return token->id;
            }

        }                ///
        ///^^^^^^/////////////
        ///End IDENTIFIKATOR/
        ///^^^^^^////////////
        ///    /
        ///   /
        ///---/
        ///
       ///|
        ////////////////
        //NUMBERS   ///
        ///////////////

        if( (isdigit(c)!=0) )
        {
            w[len]=c;
            do
            {
                if(len%10==9)
                {
                  if(reallocString(&w)!=E_OK){print_error(E_LEX,"LexERROR"); return E_LEX;}
                }
                c=fgetc(fp);
                len++;
                w[len]=c;

            }while(isdigit(c)!=0);
            if(len==1)
            {
                if(c=='.')
                {
                    do
                    {
                        if(len%10==9)
                        {
                          if(reallocString(&w)!=E_OK){print_error(E_LEX,"LexERROR"); return E_LEX;}
                        }
                        c=fgetc(fp);
                        len++;
                        w[len]=c;

                    }while(isdigit(c)!=0);
                    if((c=='e'|| c=='E'))
                    {
                        c=fgetc(fp);
                        if((c=='+') || (c=='-'))
                        {
                            len++;
                            w[len]=c;
                            do
                            {
                                if(len%10==9)
                                {
                                    if(reallocString(&w)!=E_OK){print_error(E_LEX,"LexERROR"); return E_LEX;}
                                }
                                c=fgetc(fp);
                                len++;
                                w[len]=c;
                            }while(isdigit(c)!=0);

                            w[len]='\0';
                            ungetc(c,fp);
                            token->id=VARDOUBLE;
                            token->value.varDouble = strtod(w,NULL);
                            free(w);
                            return token->id;
                        }
                        else
                        {
                            free(w);
                            print_error(E_LEX,"LexERROR"); return E_LEX;
                        }
                    }
                    else
                    {
                        w[len]='\0';
                        ungetc(c,fp);
                        token->id=VARDOUBLE;
                        token->value.varDouble = strtod(w,NULL);
                        free(w);
                        return token->id;
                    }
                }
                else
                {
                    ungetc(c,fp);
                    w[len]='\0';
                    token->id=VARINT;
                    token->value.varInt=atoi(w);
                    free(w);
                    return token->id;
                }
            }
            else if(c=='.')
            {
                do{
                    if(len%10==9)
                    {
                      if(reallocString(&w)!=E_OK){print_error(E_LEX,"LexERROR"); return E_LEX;}
                    }
                    c=fgetc(fp);
                    len++;
                    w[len]=c;
                }while(isdigit(c)!=0);
                w[len]='\0';
                ungetc(c,fp);
                token->id=VARDOUBLE;
                token->value.varDouble = strtod(w,NULL);
                free(w);
                return token->id;

            }
            else
            {
                ungetc(c,fp);
                w[len]='\0';
                token->id=VARINT;
                token->value.varInt=atoi(w);
                free(w);
                return token->id;
            }

        }
        else{
            free(w);
            print_error(E_LEX,"LexERROR"); return E_LEX;
        }

        //////////////////
        //END NUMBERS   //
        //////////////////

        /**
        *VARSTRING
        **/
        if(c=='\"')
        {
            c=fgetc(fp);
            //len++;
            while(c!='\"');
            {
                if(len%10==9)
                {
                    if(reallocString(&w)!=E_OK)
                    {
                        print_error(E_LEX,"LexERROR");
                        return E_LEX;
                    }
                }
                if(c>31)
                {
                    if(c=='$')
                    {
                        if(w[len-1]!='\\')
                        {
                            print_error(E_LEX,"$ neni predchazen \\");
                        }
                        else
                        {
                        }
                    }

                    w[len]=c;
                    len++;
                    c=fgetc(fp);
                }

            }

            w[len]='\0';
            token->id=STRING;
            token->value.varString=w;

        }
        //////////////////
        //END VARSTRING //
        //////////////////

        /**
        *KLICOVE SLOVA
        **/
        //////////////////////
        //END KLICOVE SLOVA //
        //////////////////////



}
    //k tomuhle by nemelo vubec dojit, jen kvuli warningu
    free(w);
    print_error(E_LEX,"LexERROR"); return E_LEX;
    return E_LEX;
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
   // token.value.varString = malloc(sizeof(char)*BUFF);
  //  getToken(fp,&token);
  do{
      token.id = 0;
      token.value.varString = NULL;
      token.value.varInt = -1;
      token.value.varDouble = -1.0;
/////////////////
//getToken()
////////////////

        getToken(fp,&token);

//////////////
////////////////


      if(((token.id>=4) || (token.id==1)) && (token.id<61))
      {
        printf("%d\n         ",token.id);
        printf("%s\n",token.value.varString);
      }

      if(token.id==2 && token.id<61)
      {
        printf("%d\n         ",token.id);
        printf("%d\n",token.value.varInt);
      }

      if(token.id ==3 && token.id<61)
      {
        printf("%d\n         ",token.id);
        printf("%f\n",token.value.varDouble);
      }


    if(token.value.varString!=NULL)
    {
        free(token.value.varString);
    }
  }while(token.id != KONEC);
  /*if(token.value.varString!=NULL)
    {
        free(token.value.varString);
    }*/


    fclose(fp);
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
  return E_OK;
}

