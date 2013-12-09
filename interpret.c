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

void tovarint(item *item);
void tostring(item *item);

/*  INTERPRET - vykona intepretaci jazyka IFJ13
 *  @param1: globalni TS
 *  @param2: Instrukcni seznam MAINU
 *  @param3: zasobnik adres TS a ptr na navrat do instrukcniho seznamu
 */
int interpret (tHashTbl *global_htable, TList *L, tHashTblStack *stack)
{

//------------------- INIT -------------------------------------------------------------


// => PROMENNE_______________________________

   // data aktualni instrukce [operandy]
   TInstr  *instr;
   char *src1;
   char *src2;
   char *result;

   // data aktualni instrukce z HASH table
   item *tHsrc1;
   item *tHsrc2;
   item *tHresult;

   // POMOCNE PROMENNE
   int dataType,dataType1,dataType2;
   //char *src1Data,*src2Data,*resultData;
   int TypeOF;
   int jump=0;

   // tmp
    tokenValue tmp;

    // pro porovnani
    int datTyp=0;

   // navratova dresa instrukcniho seznamu MAINU
   TLItem *nil = NULL;

// => LOKALNI TS_______________________________

   // Lokalni TS
   tHashTbl *local_htable_main;

   // naalokovani a inicializaci LOKALNI TS
   tableInit(local_htable_main);

   // init & push adresy lokalni TS na stack
   initStack(stack);
   pushStack(stack,*local_htable_main,nil);

// => AKTIVACE TS a INSTRUKCE___________________

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

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1
         dataType = (tHsrc1)->type;

          if (tHsrc1==NULL) return E_SEM_OTHER;
          else 
          {
                if (dataType==VARINT)
                {
                   // prepisu data operandu result daty operandu src1
                   tmp.varInt = (TblSearch (active_htable, src1))->data.varInt;

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
                    tmp.varDouble = (TblSearch (active_htable, src1))->data.varDouble;

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
                     tmp.varString = (TblSearch (active_htable, src1))->data.varString;

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
         }
         break;

//---------------------------- ARITMETICKE OPERACE -----------------------------------------------------

         /*========================I_ADD===================================================*/
         case I_ADD:

         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
             return E_SEM_OTHER;
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt + tHsrc1->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble + (double) (tHsrc2->data.varInt));
                   else
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) + tHsrc2->data.varDouble);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
             return E_SEM_OTHER;
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt - tHsrc1->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble - (double) (tHsrc2->data.varInt));
                   else
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) - tHsrc2->data.varDouble);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
             return E_SEM_OTHER;
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt * tHsrc1->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble * (double) (tHsrc2->data.varInt));
                   else
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) * tHsrc2->data.varDouble);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
             return E_SEM_OTHER;
           }

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARINT;
                   tmp.varInt = (tHsrc1->data.varInt / tHsrc1->data.varInt);
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                       tmp.varDouble = (tHsrc1->data.varDouble / (double) (tHsrc2->data.varInt));
                   else
                       tmp.varDouble = ( (double) (tHsrc1->data.varInt) / tHsrc2->data.varDouble);
               }
               else {
                  return E_SEM_TYPE; // bude chyba!?
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

//---------------------------- KONKATENACE --------------------------------------------------------------

         /*========================I_CON===============================================================*/
         case I_CON:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

         // overeni jestli data ubec existuji
         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

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
         else return E_SEM_OTHER;

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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

        // tmp
        tokenValue tmp;

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt > tHsrc2->data.varInt)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble > tHsrc2->data.varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)>0)  datTyp = 1;
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt >= tHsrc2->data.varInt)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble >= tHsrc2->data.varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)>=0)  datTyp = 1;
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt < tHsrc2->data.varInt)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble < tHsrc2->data.varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)<0)  datTyp = 1;
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt <= tHsrc2->data.varInt)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble <= tHsrc2->data.varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)<=0)  datTyp = 1;
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt == tHsrc2->data.varInt)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble == tHsrc2->data.varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)==0)  datTyp = 1;
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
         src1   = instr->src1;
         src2   = instr->src2;
         result = instr->result;

         // nactu id src1,src2 & result z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));
         tHsrc2   = (TblSearch (active_htable, src2));
         tHresult = (TblSearch (active_htable, result));

         // typ: tHsrc1->type   data:   tHsrc1->data   item:  tHsrc1 , tHsrc2 , tHresult

         datTyp=0;

         if (tHsrc1!=NULL || tHsrc2!=NULL) return E_SEM_OTHER;

         if (tHsrc1->type!=tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt != tHsrc2->data.varInt)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==VARDOUBLE)
              {
                 if (tHsrc1->data.varDouble != tHsrc2->data.varDouble)  datTyp = 1;
                 else datTyp = 0;
              }

              else if (tHsrc1->type==STRING)
              {
                 if (strcmp(tHsrc1->data.varString,tHsrc2->data.varString)!=0)  datTyp = 1;
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

//----------------------------  FUNKCE  --------------------------------------------------------------

         /*========================I_CALL=========================*/
         case I_CALL:
         break;

         /*========================I_RETURN=========================*/
         case I_RETURN:
         break;

//----------------------------  PREPNUTI KONTEXTU  --------------------------------------------------------------


/**********************************************
*
*     PREPNUTI KONTEXTU & DALSI INSTRUKCE
*
**************************************************/


//---------------------------- INTERNI FUNKCE --------------------------------------------------------------

         /*========================I_BOOLVAL=========================*/
         case I_BOOLVAL:

         // nactu id src1  z INSTRUKCE
         src1   = instr->src1;

         // nactu id src1  z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) return E_SEM_OTHER;
         else 
         {
            toVARBOOL(tHsrc1);
         }
         break;

         /*========================I_DOUBLEVAL=========================*/
         case I_DOUBLEVAL:

         // nactu id src1  z INSTRUKCE
         src1   = instr->src1;

         // nactu id src1  z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) return E_SEM_OTHER;
         else 
         {
            todouble(tHsrc1);
         }
         break;

         /*========================I_INTVAL=========================*/
         case I_INTVAL:

         // nactu id src1  z INSTRUKCE
         src1   = instr->src1;

         // nactu id src1  z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) return E_SEM_OTHER;
         else 
         {
            tovarint(tHsrc1);
         }
         break;

         /*========================I_STRVAL=========================*/
         case I_STRVAL:

         // nactu id src1  z INSTRUKCE
         src1   = instr->src1;

         // nactu id src1  z HASH tabulky
         tHsrc1   = (TblSearch (active_htable, src1));

         if (tHsrc1==NULL) return E_SEM_OTHER;
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
         // nactu result z INSTRUKCE
         result = instr->result;

         // nactu result z GLOBALNI HASH tabulky
         tHresult = (TblSearch (global_htable, result));

         // pokud by cil skoku nebyl v globalni tabulce ->chyba
         if (tHresult==NULL) return E_SEM_OTHER; 
         else 
         {
              // aktivuje instrukci v prave aktivnim listu, nejsu si jistej typama
              ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
         }
         break;


         /*========================I_JZ=========================*/
         case I_JZ:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         result = instr->result;

         // nactu id src1,src2 & result z GLOBALNI HASH tabulky
         tHsrc1   = (TblSearch (global_htable, src1));
         tHresult = (TblSearch (global_htable, result));

         // pokud by cil skoku, nebo zdrojova promenna nebyla v globalni tabulce ->chyba
         if (tHresult==NULL || tHsrc1==NULL) return E_SEM_OTHER;
         else 
         {
             switch (tHsrc1->type) 
             {
                case VARINT: // pokud je true, aktivuj zaznam
                  if (tHsrc1->data.varInt==0) jump = 1;
                  else  jump = 0;
                break;

                case VARBOOL:
                  if (tHsrc1->data.varInt==0)  jump = 1;
                  else  jump = 0;
                break;

                case VARDOUBLE:  
                  if (tHsrc1->data.varDouble==0.0)  jump = 1; 
                  else  jump = 0;
                break;

                case STRING:   
                  if (strcmp(tHsrc1->data.varString,"")==0)  jump = 1;
                  else  jump = 0;
                break;

                default: // chyba
                break;
             }

           if (jump) 
            {  // proved skok - nejsu si jistej typama
               ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
            }
         }
         break;


         /*========================I_JNZ=========================*/
         case I_JNZ:
         // nactu id src1,src2 & result z INSTRUKCE
         src1   = instr->src1;
         result = instr->result;

         // nactu id src1,src2 & result z GLOBALNI HASH tabulky
         tHsrc1   = (TblSearch (global_htable, src1));
         tHresult = (TblSearch (global_htable, result));

         // pokud by cil skoku, nebo zdrojova promenna nebyla v globalni tabulce ->chyba
         if (tHresult==NULL || tHsrc1==NULL) return E_SEM_OTHER;
         else 
         {
             switch (tHsrc1->type) 
             {
                case VARINT: // pokud je true, aktivuj zaznam
                  if (tHsrc1->data.varInt!=0) jump = 1;
                  else  jump = 0;
                break;

                case VARBOOL:
                  if (tHsrc1->data.varInt!=0)  jump = 1;
                  else  jump = 0;
                break;

                case VARDOUBLE:  
                  if (tHsrc1->data.varDouble!=0.0)  jump = 1; 
                  else  jump = 0;
                break;

                case STRING:   
                  if (strcmp(tHsrc1->data.varString,"")!=0)  jump = 1;
                  else  jump = 0;
                break;

                default: // chyba
                break;
             }

           if (jump) 
            {  // proved skok - nejsu si jistej typama
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

return EXIT_SUCCESS;
// end func
}
