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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scaner.h"
#include <stdio.h>

char* vestaveneFunkce[10]={
    "boolval",      //BOOLVAL
    "doubleval",    //DOUBLEVAL
    "intval",       //INTVAL
    "strval",       //STRVAL
    "get_string",   //GET_STRING
    "put_string",   //PUT_STRING
    "strlen",       //STRLEN
    "get_substring",//GET_SUBSTRING
    "find_string",  //FIND_STRING
    "sort_string",  //SORT_STRING
};
//test-vypis nacteneho tokenu
int getToken_test(FILE *fp,Ttoken *token)
{
int i= getToken(fp,token);
printf("Token ID Load:%d\n",i);
return i;
}//////

void freeW(char **w)
{
    if(*w!=NULL)
        free(*w);
}
int reallocString(char** w)
{
    if(*w!=NULL)
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
    else
    {
        print_error(E_INTERN,"LexAllocError");
        return E_LEX;
    }

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
                skipSpace(&c,fp);
                continue;
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
                skipSpace(&c,fp);
                continue;
            }
            else
            {
                ungetc(c,fp);
                ungetc('/',fp);
                //skipSpace(&c,fp);
                c=fgetc(fp);
            }
        }
        ////////////
        //////////////////
        switch( c )
        {
            case (char)-1: //KONEC
                token->id = KONEC;
                w[0]=c;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '{' :
                token->id = ZAV_SLOZ_L;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '}' :
                token->id = ZAV_SLOZ_P;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case ';' :
                token->id = STREDNIK;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '(' :
                token->id = ZAV_JEDN_L;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case ')' :
                token->id = ZAV_JEDN_P;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '+' :
                token->id = PLUS;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '-' :
                token->id = MINUS;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '/' :
                token->id = DELENO;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case '.' :
                token->id = TECKA;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;
            case ',' :
                token->id = CARKA;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
                return token->id;

            case '*' :
                token->id = KRAT;
                w[len]=c; w[len+1]='\0';
                freeW(&w); token->value.varString=NULL;
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
                freeW(&w); token->value.varString=NULL;
                token->id=VETSI_ROVNO;
                return token->id;
            }
            else
            {
                w[len]='\0';
                ungetc(c,fp);
                freeW(&w); token->value.varString=NULL;
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
                    freeW(&w); token->value.varString=NULL;
                    token->id=ROVNO;
                    return token->id;
                }
                else
                {
                    freeW(&w); token->value.varString=NULL;
                    print_error(E_LEX,"LexERROR"); return E_LEX;
                }
            }
            else
            {
                w[len]='\0';
                freeW(&w); token->value.varString=NULL;
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
                    freeW(&w); token->value.varString=NULL;
                    token->id=NEROVNO;
                    return token->id;
                }
                else
                {
                    freeW(&w); token->value.varString=NULL;
                    print_error(E_LEX,"LexERROR"); return E_LEX;
                }
            }
            else
            {
                freeW(&w); token->value.varString=NULL;
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
                freeW(&w); token->value.varString=NULL;
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
                                freeW(&w); token->value.varString=NULL;
                               token->id=START;
                               return token->id;
                            }
                            else{
                                freeW(&w); token->value.varString=NULL;
                                print_error(E_LEX,"LexERROR"); return E_LEX;
                            }
                       }
                       else{
                        freeW(&w); token->value.varString=NULL;
                        print_error(E_LEX,"LexERROR"); return E_LEX;
                        }
                    }
                    else{
                       freeW(&w); token->value.varString=NULL;
                       print_error(E_LEX,"LexERROR"); return E_LEX;
                    }
                }
                else{
                    freeW(&w); token->value.varString=NULL;
                    print_error(E_LEX,"LexERROR"); return E_LEX;
                }
            }
            else
            {
                w[len]='\0';
                ungetc(c,fp);
                len--;
                freeW(&w); token->value.varString=NULL;
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
                len++;
                c=fgetc(fp);
                while(isalnum(c)!=0 || c=='_')
                {
                    w[len]=c;
                    len++;
                    if(len%10==9)
                    {
                      if(reallocString(&w)!=E_OK)
                      {
                          print_error(E_LEX,"LexERROR"); return E_LEX;
                      }
                    }
                    c=fgetc(fp);
                }
                //navraceni nasledujiciho znaku ZA identifikatorem (kvuli dowhile)
                //ulozeni a odeslani tokenu
                ungetc(c,fp);
                w[len]='\0';
                len--;
                token->id=VARIABLE;
                token->value.varString = w;
               // freeW(&w); token->value.varString=NULL;
                return token->id;

            }
            else
            {
                freeW(&w); token->value.varString=NULL;
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
           /* if((strcmp(w,"if")==0)||(strcmp(w,"else")==0)||(strcmp(w,"while")==0)||(strcmp(w,"return")==0
                )||(strcmp(w,"function")==0)||(strcmp(w,"null")==0)||(strcmp(w,"true")==0)||(strcmp(w,"false")==0))
            {
                freeW(&w); token->value.varString=NULL;
                print_error(E_LEX,"LexERROR"); return E_LEX;
            }*/
            int i=0;
            while(i<10)
            {
                if(strcmp(w,vestaveneFunkce[i])==0){
                    break;}
                i++;
            }
            if(i<10)//Vestavena funkce
            {
                switch(i)
                {
                    case 0:
                        token->id=BOOLVAL;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 1:
                        token->id=DOUBLEVAL;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 2:
                        token->id=INTVAL;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 3:
                        token->id=STRVAL;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 4:
                        token->id=GET_STRING;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 5:
                        token->id=PUT_STRING;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 6:
                        token->id=STRLEN;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 7:
                        token->id=GET_SUBSTRING;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 8:
                        token->id=FIND_STRING;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    case 9:
                        token->id=SORT_STRING;
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    default:
                        freeW(&w); token->value.varString=NULL;
                        print_error(E_LEX,"LexERROR");
                        break;

                }
            }
            else if((strcmp(w,"if")==0))
            {
                token->id=IF;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
            else if((strcmp(w,"else")==0))
            {
                token->id=ELSE;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
            else if((strcmp(w,"while")==0))
            {
                token->id=WHILE;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
            else if((strcmp(w,"return")==0))
            {
                token->id=RETURN;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
            else if((strcmp(w,"function")==0))
            {
                token->id=FUNCTION;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
            else if((strcmp(w,"null")==0))
            {
                token->id=NIL;
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
            else if((strcmp(w,"true")==0))
            {
                token->id=VARBOOL;
                token->value.varInt=1;
                return token->id;
            }
            else if((strcmp(w,"false")==0))
            {
                token->id=VARBOOL;
                token->value.varInt=0;
                return token->id;
            }
            else
            {
                token->id=IDENTIFIKATOR;
                token->value.varString = w;
               // freeW(&w); token->value.varString=NULL;
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
                            freeW(&w); token->value.varString=NULL;
                            return token->id;
                        }
                        else
                        {
                            freeW(&w); token->value.varString=NULL;
                            print_error(E_LEX,"LexERROR"); return E_LEX;
                        }
                    }
                    else
                    {
                        w[len]='\0';
                        ungetc(c,fp);
                        token->id=VARDOUBLE;
                        token->value.varDouble = strtod(w,NULL);
                        freeW(&w); token->value.varString=NULL;
                        return token->id;
                    }
                }
                else
                {
                    ungetc(c,fp);
                    w[len]='\0';
                    token->id=VARINT;
                    token->value.varInt=atoi(w);
                    freeW(&w); token->value.varString=NULL;
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
                freeW(&w); token->value.varString=NULL;
                return token->id;

            }
            else
            {
                ungetc(c,fp);
                w[len]='\0';
                token->id=VARINT;
                token->value.varInt=atoi(w);
                freeW(&w); token->value.varString=NULL;
                return token->id;
            }
        }
        //////////////////
        //END NUMBERS   //
        //////////////////
        /**
        *VARSTRING
        **/
        if(c=='"')
        {
            c=fgetc(fp);
            int pom=0;// na rozliseni " nebo \"
            //len++;
            while(c!='"'||pom==0)
            {
                if(len%10==9)
                {
                    if(reallocString(&w)!=E_OK)
                    {
                        print_error(E_LEX,"LexERROR");
                        return E_LEX;
                    }
                }
                if(c>31 && c!='"' && c!='$' && c!='t' && c!='n' && c!='\\' && c!='x')
                {

                    w[len]=c;
                    len++;
                    c=fgetc(fp);

                }
                else if(c=='"')
                {
                    if(w[len-1]=='\\')
                    {
                        w[len-1]=c;
                        c=fgetc(fp);
                    }
                    else
                        pom=1;
                }
                else if(c=='n')
                {
                    if(w[len-1]=='\\')
                    {
                        w[len-1]='\n';
                        c=fgetc(fp);
                    }
                    else
                    {
                        w[len]=c;
                        len++;
                        c=fgetc(fp);
                    }
                }
                else if(c=='$')
                {
                    if(w[len-1]=='\\')
                    {
                        w[len-1]=c;
                        c=fgetc(fp);
                    }
                    else
                    {
                        print_error(E_LEX,"$ neni predchazen \\");
                    }
                }
                else if(c=='t')
                {
                    if(w[len-1]=='\\')
                    {
                        w[len-1]='\t';
                        c=fgetc(fp);
                    }
                    else{
                        w[len]=c;
                        len++;
                        c=fgetc(fp);
                    }
                }
                else if(c=='x')
                {
                    if(w[len-1]=='\\')
                    {//\xDD kde DD jsou hexadecimalni cisla od 00 do FF/Ff/fF/ff
                        char dd[2];
                        dd[0]=fgetc(fp);
                        dd[1]=fgetc(fp);
                        if(isxdigit(dd[0])&&isxdigit(dd[1]))
                        {
                            w[len-1]=(char)strtol(dd,NULL,16);
                        }
                        else
                        {
                            print_error(E_LEX,"Chybny format unikove skevence \xDD");
                        }
                    }
                    else
                    {
                        w[len]=c;
                        len++;
                        c=fgetc(fp);
                    }
                }
                else if(c=='\\')
                {
                    if(w[len-1]=='\\')
                    {
                        w[len-1]='\\';
                        c=fgetc(fp);
                        //len++;
                    }
                    else
                    {
                        w[len]=c;
                        len++;
                        c=fgetc(fp);
                    }
                }
                else{
                    print_error(E_LEX,"$ neni predchazen \\");
                }

            }

            w[len]='\0';
            token->id=STRING;
            token->value.varString=w;
            return token->id;
        }
        //////////////////
        //END VARSTRING //
        //////////////////
}
    //k tomuhle by nemelo vubec dojit, jen kvuli warningu
    freeW(&w); token->value.varString=NULL;
    print_error(E_LEX,"LexERROR");
    return E_LEX;
}