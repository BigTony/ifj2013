// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin - xseidl01
// Slunsky Tomas - xsluns01
// Skacel David - xskace12
// Skyva Petr - xskyva02
////////////////////////////////
// interpret.c
//
//

#include "strings.c"
#include "ial.c"
#include "ilist.h"
#include "interpret.h"
#include "errors.h"
#include "scaner.h"
#include <math.h>


/*  INTERPRET - vykona intepretaci jazyka IFJ13
 *  @param1: globalni TS
 *  @param2: Instrukcni seznam MAINU
 *  @param3: zasobnik adres TS a ptr na navrat do instrukcniho seznamu
 */
void interpret (tHashTbl *global_htable, TList *L, tHashTblStack *stack) 
{

//------------------- INIT -------------------------------------------------------------

   // aktivuju seznam MAINU
   TList *ActiveList = L; 
     
   // data aktualni instrukce
   TInstr     *instr;           
   tokenValue *src1;
   tokenValue *src2;
   tokenValue *result;

   // data aktualni instrukce z HASH table          
   tokenValue *tHsrc1;
   tokenValue *tHsrc2;
   tokenValue *tHresult;

   // POMOCNE PROMENNE
   tokenValue *tmp;
   tokenValue *dataType,*dataType1,*dataType2;
   tokenValue *src1Data,*src2Data,*resultData;
   int TypeOF;

   // navratva dresa instrukcniho seznamu MAINU
   TLItem nil = NULL;

   // Lokalni TS
   tHashTbl *lokal_htable_main;

   // naalokovani a inicializaci LOKALNI TS
   tableInit(lokal_htable_main);

   // push adresy lokalni TS na stack
   pushStack(stack,lokal_htable_main,nil);

   // aktivace prvni instrukce z prave provadenyho instrukcniho listu
   ActiveFirstItem (ActiveList);

//------------------- EXECUTE -------------------------------------------------------------

  /// cekuju jestli je instrukce aktivni, pokud ano = while (1), jinak = while (0) = end
  while (IsActiveItem(ActiveList)) 
  {
      // nactu aktivni instrukci z aktualniho instrukcniho listu
      instr = (TInstr*) ReturnActiveInstr (ActiveList);

     /* -------------------------------------------------------------
      *	
      *  semantic controll 
      *
      * ------------------------------------------------------------ */


      /// item* TblSearch (tHashTbl *tab, itemKey key);
      /// void  TblInsert (tHashTbl *tab, itemKey key, tokenValue data, int type); 

      switch (instr->operation) 
      {

	 /*========================I_ASS=================================================== (EKV.: mov) */ 
	 case I_ASS:

         // nactu id src1 & result
         src1   = instr->src1;
         result = instr->result;
	
         // nactu typ dat src1
         dataType = (TblSearch (lokal_htable_main, src1))->type;

         // POKUD operand do ktereho prirazuju jiz exituje, tak jeho data prepisu
         if (TblSearch (lokal_htable_main, result)!=NULL) 
         {
            // prepisu data operandu result daty operandu src1
            (TblSearch (lokal_htable_main, result))->data = (TblSearch (lokal_htable_main, src1))->data;

            // prepisu datovy typ operandu result datovym typem operandu src1
            (TblSearch (lokal_htable_main, result))->type = (TblSearch (lokal_htable_main, src1))->type;
         } 
         else 
         {
            // v pripade ze operand result jeste neexsituje, vlozim novou polozku
            TblInsert (lokal_htable_main, result, src1, dataType); 
         }
	 break;


	 /*========================I_ADD===================================================*/
	 case I_ADD:

         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         // nactu data src1, src2 & result
         src1Data   = tHsrc1->data;
         src2Data   = tHsrc2->data;
         resultData = tHresult->data;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (src1Data==NULL || src2Data==NULL) 
           { 
             return E_SEM_OTHER;
           }

              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp = src1Data + src2Data; 
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;
                   todouble(tHsrc1);
                   todouble(tHsrc2);
                   tmp = (tHsrc1->data + tHsrc2->data);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
               }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 resultData = tmp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, tmp, TypeOF); 
	       }
 
	 break;


         /*========================I_SUB=========================================================*/
	 case I_SUB:

         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         //  nactu data src1, src2 & result
         src1Data   = tHsrc1->data;
         src2Data   = tHsrc2->data;
         resultData = tHresult->data;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (src1Data==NULL || src2Data==NULL) 
           { 
             return E_SEM_OTHER;
           }

              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp = src1Data - src2Data; 
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;
                   todouble(tHsrc1);
                   todouble(tHsrc2);
                   tmp = (tHsrc1->data - tHsrc2->data);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
               }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 resultData = tmp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, tmp, TypeOF); 
	       }
	 break;


	 /*========================I_MUL============================================================*/
	 case I_MUL:

         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         // nactu data src1 & src2
         src1Data   = tHsrc1->data;
         src2Data   = tHsrc2->data;
         resultData = tHresult->data;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (src1Data==NULL || src2Data==NULL) 
           { 
             return E_SEM_OTHER;
           }

              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp = src1Data * src2Data; 
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;
                   todouble(tHsrc1);
                   todouble(tHsrc2);
                   tmp = (tHsrc1->data * tHsrc2->data);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
               }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 resultData = tmp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, tmp, TypeOF); 
	       }
	 break;


	 /*========================I_DIV==============================================================*/
	 case I_DIV:

         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         //  nactu data src1, src2 & result
         src1Data   = tHsrc1->data;
         src2Data   = tHsrc2->data;
         resultData = tHresult->data;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (src1Data==NULL || src2Data==NULL) 
           { 
             return E_SEM_OTHER;
           }

              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp = src1Data / src2Data; 
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;
                   todouble(tHsrc1);
                   todouble(tHsrc2);
                   tmp = (tHsrc1->data / tHsrc2->data);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
               }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 resultData = tmp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, tmp, TypeOF); 
	       }
	 break;


	 /*========================I_CON===============================================================*/ 
	 case I_CON:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         // nactu data src1, src2 & result
         src1Data   = tHsrc1->data;
         src2Data   = tHsrc2->data;
         resultData = tHresult->data;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         ///char * konkatenace(char*prvni,char*druhy)
         if (dataType1==VARSTRING && dataType2==VARSTRING) 
         {
                TypeOF = VARSTRING;
		tmp = (tokenValue) konkatenace((char *)src1Data,(char *)src2Data);
         } else 
          {
              TypeOF = VARSTRING;
              tostring(tHsrc1);
              tostring(tHsrc2);
              tmp = (tokenValue) konkatenace((char *)tHsrc1->data,(char *)tHsrc2->data);
          }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 resultData = tmp; // uloim do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam 
                   TblInsert (lokal_htable_main, result, tmp, TypeOF); 
	      }

	 break;

     /*
       #define IDENTIFIKATOR 1
       #define VARINT 2
       #define VARDOUBLE 3
       #define VARIABLE 4
       #define STRING 5
     */

     /*
        typedef union {
           int varInt;
           double varDouble;
           char *varString;
        }tokenValue;
      */

      /*
        typedef struct item{
            	int type;
            	itemKey key;
            	tokenValue data;
            	struct item *nextItem;
        }item;
      */


	 /*========================I_G=================================================================*/ 
	 case I_G:

         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt > tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble > tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)>0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       }
	 break;

	 /*========================I_GE===================================================================*/
	 case I_GE:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt >= tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble >= tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)>=0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       }
	 break;

	 /*========================I_L=========================*/ 
	 case I_L:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt < tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble < tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)<0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       }
	 break;

	 /*========================I_LE==========================================================*/ 
	 case I_LE:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt <= tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble <= tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)<=0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       }
	 break;

	 /*========================I_E================================================================*/ 
	 case I_E:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt == tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble == tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)==0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       }
	 break;

	 /*========================I_NE=========================*/ 
	 case I_NE:

	 break         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt != tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble != tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)!=0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       };


	 /*========================I_ET=========================*/ 
	 case I_ET:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (lokal_htable_main, src1));
         tHsrc2   = (TblSearch (lokal_htable_main, src2));
         tHresult = (TblSearch (lokal_htable_main, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         int datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type) 
         {
              TypeOF = VARBOLEAN;

              if (tHsrc1->type==VARINT) 
              {
                 if (tHsrc1->data->varInt == tHsrc2->data->varInt)  datTyp = 1;
                 else datTyp = 0;
              }

	      else if (tHsrc1->type==VARDOUBLE) 
              {
                 if (tHsrc1->data->varDouble == tHsrc2->data->varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARSTRING) 
              {
                 if (strcmp(tHsrc1->data->varString,tHsrc2->data->varString)==0)  datTyp = 1;
                 else datTyp = 0;
              }

         } 
         else 
         {
             return E_SEM_TYPE; // nejsou stejny typy vole!
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL) 
	      {
                 tHresult->data->varInt = datTyp; // uloim soucet do te exitusjici
	      } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   TblInsert (lokal_htable_main, result, datTyp, TypeOF); 
	       }
	 break;


	 /*========================I_NET=========================*/
	 case I_NET:
	 break;

	 /*========================I_CALL=========================*/ 
	 case I_CALL:
	 break;

	 /*========================I_RETURN=========================*/ 
	 case I_RETURN:
	 break;

	 /*========================I_CON=========================*/ 
	 case I_TYPE:
	 break;

	 /*========================I_SORT=========================*/
	 case I_SORT:
	 break;

	 /*========================I_FIND=========================*/ 
	 case I_FIND:
	 break;

	 /*========================I_STRL=========================*/ 
	 case I_STRL:
	 break;


	 /*========================I_PUTS=========================*/ 
	 case I_PUTS:
	 break;


	 /*========================I_GETS=========================*/ 
	 case I_GETS:
	 break;


	 /*========================I_GETSU=========================*/ 
	 case I_GETSU:
	 break;


	 /*========================I_JMP=========================*/ 
	 case I_JMP:
	 break;


	 /*========================I_JZ=========================*/ 
	 case I_JZ:
	 break;


	 /*========================I_JNZ=========================*/ 
	 case I_JNZ:
	 break;


	 /*========================I_LAB=========================*/ 
	 case I_LAB:
	 break;


     }

  	/*posun se na dalsi instrukci*/
	ActiveNextItem (ActiveList);
  }

}

