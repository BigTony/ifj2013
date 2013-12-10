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

#include <string.h>
#include "interpret.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* INTERPRET - vykona intepretaci jazyka IFJ13
* @param1: globalni TS
* @param2: Instrukcni seznam MAINU
*/
void interpret (tHashTbl *global_htable, TList *L)
{
  // TblPrint(global_htable);

//------------------- INIT -------------------------------------------------------------

//____________________PROMENNE____________________________________________

   // data aktualni instrukce [operandy]
   TInstr *instr;
   char *src1;
   char *src2;
   char *result;

   // data aktualni instrukce z HASH table
   item *tHsrc1, *tHsrcGlob1;
   item *tHsrc2, *tHsrcGlob2;
   item *tHsrcLocF1,*tHsrcLocF2;
   item *tHresult = NULL;

   // POMOCNE PROMENNE
   int dataType,dataType1,dataType2;
   int TypeOF=0;
   int jump=0;
   int datTyp=0;

   // tmp
   tokenValue tmp;

//____________________LOKALNI TS_____________________________________________

   // Lokalni TS
   tHashTbl *local_htable_main;

   // naalokovani a inicializaci LOKALNI TS
   tableInit(&local_htable_main);

   // init & push adresy lokalni TS na stack
   initStack(&g_ptrs->function_stack);
   pushStack(g_ptrs->function_stack,local_htable_main,NULL);

//_____________________AKTIVACE TS a INSTRUKCE___________________________________

   // ukazuje na aktivni LOKALNI TS, coz bude po inicializace ta jiz naalokovana
   tHashTbl *active_htable = local_htable_main;

   // aktivuju seznam MAINU
   TList *ActiveList = L;

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
* semantic controll
*
* ------------------------------------------------------------ */

      /// item* TblSearch (tHashTbl *tab, itemKey key);
      /// void TblInsert (tHashTbl *tab, itemKey key, tokenValue data, int type);
/*
          printf("====PROVADENA INSTRUKCE=====\n");
          printf("%i\n",instr->operation);
          printf("%s\n",instr->src1);
          printf("%s\n",instr->src2);
          printf("%s\n",instr->result);
          printf("============================\n");
*/

// PrintList píčo.
PrintList (L);

      switch (instr->operation)
      {

         /*========================I_ASS=================================================== (EKV.: mov) */
         case I_ASS:


         // nactu id src1 & result
         src1 = instr->src1;
         result = instr->result;

          // nactu id src1,src2 & result z HASH tabulky
          tHsrcGlob1 = (TblSearch (global_htable, src1));//global
          tHsrc1 = (TblSearch(active_htable,src1));
          tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1
         dataType = (tHsrc1->type);

          if (!dataType) print_error(E_SEM_OTHER, "dat.typ v itemu neni nastaven [I_ASS]");

          if (tHsrc1==NULL) print_error(E_SEM_OTHER, "item v lokalni ani globalni TS neexistuje [I_ASS]");
          else
          {
                if (dataType==VARINT)
                {
                   // prepisu data operandu result daty operandu src1
                   tmp.varInt = (tHsrc1)->data.varInt;

                     // POKUD operand do ktereho prirazuju jiz exituje, tak jeho data prepisu
                     if (tHresult!=NULL)
                     {
                        tHresult->data.varInt = tmp.varInt;
                        tHresult->type = dataType;
                     }
                     else
                     {
                        
                        // v pripade ze operand result jeste neexsituje, vlozim novou polozku
                        TblInsert (active_htable, result, tmp, dataType);
                     }
                }
                else if (dataType==VARDOUBLE)
                {
                    
                    tmp.varDouble = (tHsrc1)->data.varDouble;
                     // POKUD operand do ktereho prirazuju jiz exituje, tak jeho data prepisu
                     if (tHresult!=NULL)
                     {
                        tHresult->data.varDouble = tmp.varDouble;
                        tHresult->type = dataType;
                     }
                     else
                     {
                        // v pripade ze operand result jeste neexsituje, vlozim novou polozku
                        TblInsert (active_htable, result, tmp, dataType);
                     }
                }
                else if (dataType==STRING)
                {
                     tmp.varString = (tHsrc1)->data.varString;

                     // POKUD operand do ktereho prirazuju jiz exituje, tak jeho data prepisu
                     if (tHresult!=NULL)
                     {
                        tHresult->data.varString = tmp.varString;
                        tHresult->type = dataType;
                     }
                     else
                     {
                        // v pripade ze operand result jeste neexsituje, vlozim novou polozku
                        TblInsert (active_htable, result, tmp, dataType);
                     }
                }
                else if (dataType==VARBOOL)
                {
                     tmp.varInt= (tHsrc1)->data.varInt;

                     // POKUD operand do ktereho prirazuju jiz exituje, tak jeho data prepisu
                     if (tHresult!=NULL)
                     {
                        tHresult->data.varInt = tmp.varInt;
                        tHresult->type = dataType;
                     }
                     else
                     {
                        // v pripade ze operand result jeste neexsituje, vlozim novou polozku
                        TblInsert (active_htable, result, tmp, dataType);
                     }
                }
         }

         break;

//---------------------------- ARITMETICKE OPERACE -----------------------------------------------------

         /*========================I_ADD===================================================*/
         case I_ADD:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         if (!dataType1 || !dataType2) print_error(E_SEM_OTHER, "dat.typ itemu v src1 nebo src2 neni nastaven [I_ADD] ");

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_ADD]");
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt + tHsrc2->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble + (double) (tHsrc2->data.varInt));
                   else if(dataType1==VARINT && dataType2==VARDOUBLE)
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) + tHsrc2->data.varDouble);
                   else if(dataType1==VARDOUBLE && dataType2==VARDOUBLE)
                       tmp.varDouble = (double)((tHsrc1->data.varDouble) + tHsrc2->data.varDouble);
               }
               else {
                  print_error(E_SEM_TYPE, "item v lokalni ani globalni TS neexistuje [I_ADD]");
               }

            ///-----------pokud result exituje, prepisu data
            if (tHresult!=NULL)
                {
                   if (dataType1==VARINT && dataType2==VARINT)
                   {
                     tHresult->data.varInt = tmp.varInt; // uloim soucet do te exitusjici
                     tHresult->type=TypeOF;
                    }
                   else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                   {
                       tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                       tHresult->type=TypeOF;
                   }
             }
              else
               {
                  if (dataType1==VARINT && dataType2==VARINT)
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
                  else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
               }
         break;


         /*========================I_SUB=========================================================*/
         case I_SUB:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));
         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_SUB]");
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt - tHsrc2->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble - (double) (tHsrc2->data.varInt));
                   else if(dataType1==VARINT && dataType2==VARDOUBLE)
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) - tHsrc2->data.varDouble);
                   else if(dataType1==VARDOUBLE && dataType2==VARDOUBLE)
                       tmp.varDouble = (double)((tHsrc1->data.varDouble) - tHsrc2->data.varDouble);
               }
               else {
                  print_error(E_SEM_TYPE, "vadny typ operandu [I_SUB]");
               }

            ///-----------pokud result exituje, prepisu data
            if (tHresult!=NULL)
                {
                   if (dataType1==VARINT && dataType2==VARINT)
                   {
                     tHresult->data.varInt = tmp.varInt; // uloim soucet do te exitusjici
                     tHresult->type=TypeOF;
                    }
                   else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                   {
                       tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                       tHresult->type=TypeOF;
                   }
             }
              else
               {
                  if (dataType1==VARINT && dataType2==VARINT)
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
                  else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
               }
         break;


         /*========================I_MUL============================================================*/
         case I_MUL:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;


         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_MUL]");
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt * tHsrc2->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble * (double) (tHsrc2->data.varInt));
                   else if(dataType1==VARINT && dataType2==VARDOUBLE)
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) * tHsrc2->data.varDouble);
                   else if(dataType1==VARDOUBLE && dataType2==VARDOUBLE)
                       tmp.varDouble = (double)((tHsrc1->data.varDouble) * tHsrc2->data.varDouble);
               }
               else {
                  print_error(E_SEM_TYPE, "vadny typ operandu [I_MUL]");
               }

            ///-----------pokud result exituje, prepisu data
            if (tHresult!=NULL)
                {
                   if (dataType1==VARINT && dataType2==VARINT)
                   {
                     tHresult->data.varInt = tmp.varInt; // uloim soucet do te exitusjici
                     tHresult->type=TypeOF;
                    }
                   else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                   {
                       tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                       tHresult->type=TypeOF;
                   }
             }
              else
               {
                  if (dataType1==VARINT && dataType2==VARINT)
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
                  else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
               }
         break;


         /*========================I_DIV==============================================================*/
         case I_DIV:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_DIV]");
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARDOUBLE;
                  // todouble(tHsrc1);
                  // todouble(tHsrc2);
                   tmp.varDouble = (double)((double)(tHsrc1->data.varInt) / (double)(tHsrc2->data.varInt));
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT){
                       tmp.varDouble = (tHsrc1->data.varDouble / (double) (tHsrc2->data.varInt));
                    }
                   else if(dataType1==VARINT && dataType2==VARDOUBLE)
                       tmp.varDouble = ( (double)(tHsrc1->data.varInt) / tHsrc2->data.varDouble);
                    else if(dataType1==VARDOUBLE && dataType2==VARDOUBLE)
                      tmp.varDouble = (tHsrc1->data.varDouble / tHsrc2->data.varDouble);

               }
               else {
                  print_error(E_SEM_TYPE, "vadny typ operandu [I_DIV]");
               }

            ///-----------pokud result exituje, prepisu data
            if (tHresult!=NULL)
                {
                   if (dataType1==VARINT && dataType2==VARINT)
                   {
                     tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                     tHresult->type=TypeOF;
                    }
                   else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                   {
                       tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                       tHresult->type=TypeOF;
                   }
             }
              else
               {
                  if (dataType1==VARINT && dataType2==VARINT)
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
                  else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
                  {
                      // polozka result neexistovala, pridam to nove vytvorene
                      TblInsert (active_htable, result, tmp, TypeOF);
                  }
               }
         break;

//---------------------------- KONKATENACE --------------------------------------------------------------

         /*========================I_CON===============================================================*/
         case I_CON:
         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         // overeni jestli data ubec existuji
         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_CON]");

         /// cekni typ pro konkatenaci
         if (dataType1==STRING && dataType2==STRING)
         {
                TypeOF = STRING;
                tmp.varString = konkatenace(tHsrc1->data.varString, tHsrc2->data.varString);
         }

         else if (dataType1==STRING && dataType2!=STRING)
         {
              TypeOF = STRING;
              tostring(tHsrc2);
              tHsrc2->type=STRING;
              tmp.varString = konkatenace(tHsrc1->data.varString, tHsrc2->data.varString);
         }

         else if (dataType1!=STRING && dataType2==STRING)
         {
              TypeOF = STRING;
              tostring(tHsrc1);
              tHsrc1->type=STRING;
              tmp.varString = konkatenace(tHsrc1->data.varString, tHsrc2->data.varString);
         }
         else print_error(E_SEM_TYPE, "vadny typ operandu");

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varString = tmp.varString; // uloim do te exitusjici
              } else {
                   // polozka result neexistovala, pridam
                   TblInsert (active_htable, result, tmp, TypeOF);
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

//---------------------------- POROVNAVANI ----------------------------------------------------------------

         /*========================I_G=================================================================*/
         case I_G:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;


         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         

         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

        // tmp
        tokenValue tmp;

         // typ: tHsrc1->type data: tHsrc1->data item: tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item v lokalni ani globalni TS neexistuje I_G");

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt > tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble > tHsrc2->data.varDouble) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)>0) datTyp = 1;
                 else datTyp = 0;
              }
              else if (tHsrc1->type==VARBOOL)
              {
                 if (tHsrc1->data.varInt > tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

         }
         else
         {
                  print_error(E_SEM_TYPE, "vadny typ operandu");
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varInt = datTyp; // uloim soucet do te exitusjici
              } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   tmp.varInt = datTyp;
                   TblInsert (active_htable, result, tmp, TypeOF);
               }
         break;

         /*========================I_GE===================================================================*/
         case I_GE:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;


         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type data: tHsrc1->data item: tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_GE]");

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt >= tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble >= tHsrc2->data.varDouble) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)>=0) datTyp = 1;
                 else datTyp = 0;
              }
              else if (tHsrc1->type==VARBOOL)
              {
                 if (tHsrc1->data.varInt >= tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }
         }
         else
         {
              print_error(E_SEM_TYPE, "item v lokalni ani globalni TS neexistuje [I_GE]");
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varInt = datTyp; // uloim soucet do te exitusjici
              } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   tmp.varInt = datTyp;
                   TblInsert (active_htable, result, tmp, TypeOF);
               }
         break;

         /*========================I_L=========================*/
         case I_L:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;


         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type data: tHsrc1->data item: tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_L]");

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt < tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble < tHsrc2->data.varDouble) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)<0) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARBOOL)
              {
                 if (tHsrc1->data.varInt < tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

         }
         else
         {
              print_error(E_SEM_TYPE, "item v lokalni ani globalni TS neexistuje [I_L]");
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varInt = datTyp; // uloim soucet do te exitusjici
              } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   tmp.varInt = datTyp;
                   TblInsert (active_htable, result, tmp, TypeOF);
               }
         break;

         /*========================I_LE==========================================================*/
         case I_LE:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type data: tHsrc1->data item: tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_LE]");

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt <= tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble <= tHsrc2->data.varDouble) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)<=0) datTyp = 1;
                 else datTyp = 0;
              }
              else if (tHsrc1->type==VARBOOL)
              {
                 if (tHsrc1->data.varInt <= tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

         }
         else
         {
              print_error(E_SEM_TYPE, "item v lokalni ani globalni TS neexistuje [I_LE]");
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varInt = datTyp; // uloim soucet do te exitusjici
              } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   tmp.varInt = datTyp;
                   TblInsert (active_htable, result, tmp, TypeOF);
               }
         break;


         /*========================I_ET=========================*/
         case I_ET:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;


         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type data: tHsrc1->data item: tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_ET]");

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt == tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble == tHsrc2->data.varDouble) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)==0) datTyp = 1;
                 else datTyp = 0;
              }
              else if (tHsrc1->type==VARBOOL)
              {
                 if (tHsrc1->data.varInt == tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }
         }
         else
         {
             datTyp = 0;
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varInt = datTyp; // uloim soucet do te exitusjici
              } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   tmp.varInt = datTyp;
                   TblInsert (active_htable, result, tmp, TypeOF);
               }
         break;


         /*========================I_NET=========================*/
         case I_NET:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrcGlob2 = (TblSearch (global_htable, src2));//global
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : tHsrcGlob2;

         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type data: tHsrc1->data item: tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_OTHER, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_NET]");

         if (tHsrc1->type!=tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt != tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble != tHsrc2->data.varDouble) datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)!=0) datTyp = 1;
                 else datTyp = 0;
              }
              else if (tHsrc1->type==VARBOOL)
              {
                 if (tHsrc1->data.varInt != tHsrc2->data.varInt) datTyp = 1;
                 else datTyp = 0;
              }

         }
         else
         {
             datTyp = 0;
         }

            // pokud result exituje, prepisu data
            if (tHresult!=NULL)
              {
                 tHresult->data.varInt = datTyp; // uloim soucet do te exitusjici
              } else {
                   // polozka result neexistovala, pridam to nove vytvorene
                   tmp.varInt = datTyp;
                   TblInsert (active_htable, result, tmp, TypeOF);
               }
         break;

//---------------------------- FUNKCE --------------------------------------------------------------



         /*========================I_TSW=========================*/
         case I_TSW:
         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;
 
         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "id funkce v lokalni ani globalni TS neexistuje [I_TSW]");
         else 
         {
            // Lokalni TS
            tHashTbl *local_htable_func;

            // naalokovani a inicializaci LOKALNI TS fce
            tableInit(&local_htable_func);

            // push adresy lokalni TS fce na stack
            pushStack(g_ptrs->function_stack,local_htable_func,NULL);//(TLitem*)(tHsrc1->data.pointer)

            // prepnuti kontextu
           /// active_htable = local_htable_func;
         }
         break;


         /*========================I_PARAM=========================*/
         case I_PARAM:
         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         
         // lokalni TS funkce
         tHashTbl *local_htable_Fce = &((topStack(g_ptrs->function_stack))->hashTbl);

         // nactu data na prohledani 3 TS
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         //tHsrcLocF1 = (TblSearch (local_htable_Fce, src1));
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 =  (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "id funkce v lokalni ani globalni TS neexistuje [I_PARAM]");
         else 
         {
             switch (tHsrc1->type)
             {
                case VARINT:
                  TypeOF = VARINT;
                  tmp.varInt=tHsrc1->data.varInt;
                break;

                case VARBOOL:
                  TypeOF = VARBOOL;
                  tmp.varInt=tHsrc1->data.varInt;
                break;

                case VARDOUBLE:
                  TypeOF = VARDOUBLE;
                  tmp.varDouble=tHsrc1->data.varDouble;
                break;

                case STRING:
                  TypeOF = STRING;
                  tmp.varString=tHsrc1->data.varString;
                break;

                default: // chyba
                break;
             }

            TblInsert (local_htable_Fce, src1, tmp, TypeOF);
         }
         break;

         /*========================I_CALL=========================*/
         case I_CALL:
         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;
 
         // lokalni TS dane funkce
         tHashTbl *local_htable_Fce = &((topStack(g_ptrs->function_stack))->hashTbl);

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "id funkce v lokalni ani globalni TS neexistuje [I_CALL]");
         else 
         {
              //ulozeni navratove adresy na stack
              (TLItem*)(/*&*/((topStack(g_ptrs->function_stack)).hashTbl->data.pointer)) = L->Act->Next;

              // prepnu listy
              ActiveList = (TLItem*)tHsrc1->data.pointer;

              // prepnuti kontextu
              active_htable = local_htable_Fce;
         }
         break;


         /*========================I_RETURN=========================*/
         case I_RETURN:
         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH nebo GLOBAL hash tabulky
         tHsrcGlob1 = (TblSearch (global_htable, src1));//global
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "id v lokalni ani globalni TS neexistuje [I_RETURN]");
         else 
         {
             switch (tHsrc1->type)
             {
                case VARINT:
                  TypeOF = VARINT;
                  tmp.varInt=tHsrc1->data.varInt;
                break;

                case VARBOOL:
                  TypeOF = VARBOOL;
                  tmp.varInt=tHsrc1->data.varInt;
                break;

                case VARDOUBLE:
                  TypeOF = VARDOUBLE;
                  tmp.varDouble=tHsrc1->data.varDouble;
                break;

                case STRING:
                  TypeOF = STRING;
                  tmp.varString=tHsrc1->data.varString;
                break;

                default: // chyba
                break;
             }

             // pop
             TblInsert (local_htable_Fce, src1, tmp, TypeOF);
         }
         break;


//---------------------------- INTERNI FUNKCE --------------------------------------------------------------

         /*========================I_BOOLVAL=========================*/
         case I_BOOLVAL:

         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH tabulky
         tHsrc1 = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "item v lokalni TS neexistuje [I_BOOLVAL]");
         else
         {
            toVARBOOL(tHsrc1);
         }
         break;

         /*========================I_DOUBLEVAL=========================*/
         case I_DOUBLEVAL:

         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH tabulky
         tHsrc1 = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "item v lokalni TS neexistuje [I_DOUBLEVAL]");
         else
         {
            todouble(tHsrc1);
         }
         break;

         /*========================I_INTVAL=========================*/
         case I_INTVAL:

         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH tabulky
         tHsrc1 = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "item v lokalni TS neexistuje [I_INTVAL]");
         else
         {
            tovarint(tHsrc1);
         }
         break;

         /*========================I_STRVAL=========================*/
         case I_STRVAL:

         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH tabulky
         tHsrc1 = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) print_error(E_SEM_OTHER, "item v lokalni TS neexistuje [I_STRVAL]");
         else
         {
            tostring(tHsrc1);
         }
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


//---------------------------- SKOKYY --------------------------------------------------------------


         /*========================I_JMP=========================*/
         case I_JMP:
         printf("==========================================================================\n");
         // nactu result z INSTRUKCE
         result = instr->result;
         printf("%s\n",result);
         // nactu result z GLOBALNI HASH tabulky
         tHresult = (TblSearch (global_htable, result));

         // pokud by cil skoku nebyl v globalni tabulce ->chyba
         if (tHresult==NULL) print_error(E_SEM_OTHER, "cil skoku v lokalni TS neexistuje [I_JMP]");
         else
         {

              // aktivuje instrukci v prave aktivnim listu, nejsu si jistej typama
              ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
         }
         printf("==========================================================================\n");
         break;


         /*========================I_JZ=========================*/
         case I_JZ:
         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         result = instr->result;

          // nactu id src1,src2 & result z HASH tabulky
          tHsrcGlob1 = (TblSearch (global_htable, src1));//global
          tHsrc1 = (TblSearch(active_htable,src1));
          tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // nactu id src1,src2 & result z GLOBALNI HASH tabulky
         tHresult = (TblSearch (global_htable, result));


         printf("%s kokotihlava %s \n",src1,result);

         // pokud by cil skoku, nebo zdrojova promenna nebyla v globalni tabulce ->chyba
         if (tHresult==NULL || tHsrc1==NULL) 
            print_error(E_SEM_OTHER, "item v lokalni ani globalni TS neexistuje I_JZ");
         else
         {
             switch (tHsrc1->type)
             {
                case VARINT: // pokud je true, aktivuj zaznam
                  if (tHsrc1->data.varInt==0) jump = 1;
                  else jump = 0;
                break;

                case VARBOOL:
                  printf("NIGGGAAAA\n");
                  if (tHsrc1->data.varInt==0) jump = 1;
                  else jump = 0;
                break;

                case VARDOUBLE:
                  if (tHsrc1->data.varDouble==0.0) jump = 1;
                  else jump = 0;
                break;

                case STRING:
                  if (strcmp(tHsrc1->data.varString,"")==0) jump = 1;
                  else jump = 0;
                break;

                default: // chyba
                break;
             }

           if (jump)
            { // proved skok - nejsu si jistej typama
              printf("SKACU!\n");
               ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
            }
         }
         // TblPrint(active_htable);
         // TblPrint(global_htable);
         break;


         /*========================I_JNZ=========================*/
         case I_JNZ:
         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         result = instr->result;

          // nactu id src1,src2 & result z HASH tabulky
          tHsrcGlob1 = (TblSearch (global_htable, src1));//global
          tHsrc1 = (TblSearch(active_htable,src1));
          tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : tHsrcGlob1;

         // pokud by cil skoku, nebo zdrojova promenna nebyla v globalni tabulce ->chyba
         if (tHresult==NULL || tHsrc1==NULL) print_error(E_SEM_OTHER, "item v lokalni ani globalni TS neexistuje I_JNZ");
         else
         {
             switch (tHsrc1->type)
             {
                case VARINT: // pokud je true, aktivuj zaznam
                  if (tHsrc1->data.varInt!=0) jump = 1;
                  else jump = 0;
                break;

                case VARBOOL:
                  if (tHsrc1->data.varInt!=0) jump = 1;
                  else jump = 0;
                break;

                case VARDOUBLE:
                  if (tHsrc1->data.varDouble!=0.0) jump = 1;
                  else jump = 0;
                break;

                case STRING:
                  if (strcmp(tHsrc1->data.varString,"")!=0) jump = 1;
                  else jump = 0;
                break;

                default: // chyba
                break;
             }

           if (jump)
            { // proved skok - nejsu si jistej typama
               ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
            }
         }
         break;


         /*========================I_LAB=========================*/
         case I_LAB:
           // dodela david *joke* :P
         break;


     }

          /*posun se na dalsi instrukci*/
        ActiveNextItem (ActiveList);
  }
  printf("======Tabulky po assertu======\n");
  TblPrint(global_htable);
  TblPrint(active_htable);
  printf("============\n");
// end func
}
