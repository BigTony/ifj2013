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
#include <limits.h> 

int max (int a, int b, TIType type);

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
   item *tHsrc1;
   item *tHsrc2;
   item *tHresult = NULL;

   // POMOCNE PROMENNE
   int dataType1,dataType2;
   int TypeOF=0;
   int jump=0;
   int datTyp=0;

   // tmp
   tokenValue tmp;

        // Tabulka lokalnich funkci
   char* vestaveneFunkce[10]={
    "boolval", //BOOLVAL
    "doubleval", //DOUBLEVAL
    "intval", //INTVAL
    "strval", //STRVAL
    "get_string", //GET_STRING
    "put_string", //PUT_STRING
    "strlen", //STRLEN
    "get_substring",//GET_SUBSTRING
    "find_string", //FIND_STRING
    "sort_string", //SORT_STRING
};
   void (*fun[10]) (tHashTbl*tab,tHashTbl*NavrTab)={
    vs_boolval, //BOOLVAL
    vs_doubleval, //DOUBLEVAL
    vs_intval, //INTVAL
    vs_strval, //STRVAL
    vs_get_string, //GET_STRING
    vs_put_string, //PUT_STRING
    vs_strlen, //STRLEN
    vs_get_substring,//GET_SUBSTRING
    vs_find_string, //FIND_STRING
    vs_sort_string, //SORT_STRING
};


//____________________LOKALNI TS_____________________________________________

   // Lokalni TS pro MAIN & FCE
   tHashTbl *local_htable_main,*local_htable_Fce;

   // naalokovani a inicializaci LOKALNI TS
   tableInit(&local_htable_main);

   // init & push adresy lokalni TS na stack
   initStack(&g_ptrs->function_stack);
   pushStack(g_ptrs->function_stack,local_htable_main,NULL,NULL);

//_____________________AKTIVACE TS a INSTRUKCE___________________________________

   // ukazuje na aktivni LOKALNI TS, coz bude po inicializace ta jiz naalokovana
   tHashTbl *active_htable = local_htable_main;

   // aktivuju seznam MAINU
   TList *ActiveList = L;

   // aktivace prvni instrukce z prave provadenyho instrukcniho listu
   ActiveFirstItem (ActiveList);

    // printf("uplnej zacatek====\n");
    // PrintList(ActiveList);
    // printf("==============\n");

//------------------- EXECUTE -------------------------------------------------------------

  /// cekuju jestli je instrukce aktivni, pokud ano = while (1), jinak = while (0) = end
  while (1) //IsActiveItem(ActiveList)
  {

     if (!(IsActiveItem(ActiveList)))
     {
           if (((topStack(g_ptrs->function_stack))->NavrInstrukce)!=NULL)
           {
              // aktivuju list z vrcholu stacku a nastav aktivni instrukci
              ActiveList = (topStack(g_ptrs->function_stack))->list;
              ActivePtrItem(ActiveList,(topStack(g_ptrs->function_stack))->NavrInstrukce);
              TblDelete((topStack(g_ptrs->function_stack))->hashTbl);
              // pop stack
              popStack(g_ptrs->function_stack);

              //Aktivuj lokalni TS fce
              active_htable = (topStack(g_ptrs->function_stack))->hashTbl;

              tokenValue nil;
              nil.varString = NULL;

              //vloz polozku
              TblInsert (active_htable, "$", nil, NIL);
              ActiveNextItem(ActiveList);
           }
          else break;
     }


      int muzuskocit = 1;

      // nactu aktivni instrukci z aktualniho instrukcniho listu
      instr = (TInstr*) ReturnActiveInstr (ActiveList);

      switch (instr->operation)
      {

         /*========================I_ASS=================================================== (EKV.: mov) */
         case I_ASS:
          // nactu id src1 & result
          src1 = instr->src1;
          result = instr->result;
         
          // nactu id src1 HASH tabulky
          tHsrc1 = (TblSearch(active_htable,src1));
          tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

          /// vyhledam vysledek
          tHresult = (TblSearch (active_htable, result));
    
          if (tHsrc1==NULL) {
             print_error(E_SEM_VAR, "item v lokalni ani globalni TS neexistuje [I_ASS] thsrc1");
          }
          else
          {
              if (tHresult!=NULL)
              {
                  tHresult->data = tHsrc1->data;
                  tHresult->type = tHsrc1->type;
              }
              else
              {
                 TblInsert (active_htable, result, tHsrc1->data, tHsrc1->type);
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
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_ADD]");
           }

             // nactu typ dat src1 & src2
             dataType1 = tHsrc1->type;
             dataType2 = tHsrc2->type;

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   /// OVERENI zdali neni INT_MAX, pokud jo tak pretypujem na double
                   if (max(tHsrc1->data.varInt,tHsrc2->data.varInt,I_ADD)==1)   // OK
                    {
                          TypeOF = VARINT; // vysledek bude int
                          tmp.varInt = (tHsrc1->data.varInt + tHsrc2->data.varInt);
                    }
                   else 
                   {
                          TypeOF = VARDOUBLE; // vysledek bude double
                          tmp.varDouble = ((double)tHsrc1->data.varInt + (double)tHsrc2->data.varInt);
                   }
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
                           if (TypeOF == VARINT) 
                            {
                               tHresult->data.varInt = tmp.varInt; // uloim soucet do te exitusjici
                               tHresult->type=TypeOF;
                            } else {
                               tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                               tHresult->type=TypeOF;
                            }
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
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_SUB]");
           }

              // nactu typ dat src1 & src2
              dataType1 = tHsrc1->type;
              dataType2 = tHsrc2->type;

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   /// OVERENI zdali neni INT_MAX, pokud jo tak pretypujem na double
                   if (max(tHsrc1->data.varInt,tHsrc2->data.varInt,I_SUB)==1)   // OK
                    {
                          TypeOF = VARINT; // vysledek bude int
                          tmp.varInt = (tHsrc1->data.varInt - tHsrc2->data.varInt);
                    }
                   else 
                   {
                          TypeOF = VARDOUBLE; // vysledek bude double
                          tmp.varDouble = ((double)tHsrc1->data.varInt - (double)tHsrc2->data.varInt);
                   }
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
                           if (TypeOF == VARINT) 
                            {
                               tHresult->data.varInt = tmp.varInt; // uloim soucet do te exitusjici
                               tHresult->type=TypeOF;
                            } else {
                               tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                               tHresult->type=TypeOF;
                            }
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
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_MUL]");
           }

             // nactu typ dat src1 & src2
             dataType1 = tHsrc1->type;
             dataType2 = tHsrc2->type;

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   /// OVERENI zdali neni INT_MAX, pokud jo tak pretypujem na double
                   if (max(tHsrc1->data.varInt,tHsrc2->data.varInt,I_MUL)==1)   // OK
                    {
                          TypeOF = VARINT; // vysledek bude int
                          tmp.varInt = (tHsrc1->data.varInt * tHsrc2->data.varInt);
                    }
                   else 
                   {
                          TypeOF = VARDOUBLE; // vysledek bude double
                          tmp.varDouble = ((double)tHsrc1->data.varInt * (double)tHsrc2->data.varInt);
                   }
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
                           if (TypeOF == VARINT) 
                            {
                               tHresult->data.varInt = tmp.varInt; // uloim soucet do te exitusjici
                               tHresult->type=TypeOF;
                            } else {
                               tHresult->data.varDouble = tmp.varDouble; // uloim soucet do te exitusjici
                               tHresult->type=TypeOF;
                            }
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
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

          // pokud src1 nebo src2 nebudou mit prirazenou hodnotu -> syntax error
          if (tHsrc1==NULL || tHsrc2==NULL)
           {
              print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_DIV]");
           }

               // nactu typ dat src1 & src2
               dataType1 = tHsrc1->type;
               dataType2 = tHsrc2->type;

              ///----------- cekuju provadim operace
              if (dataType1==VARINT && dataType2==VARINT)
               {
                   // vysledek bude int
                   TypeOF = VARDOUBLE;

                   // [===overujeme jestli se nedeli nulou===]
                   if ((double)(tHsrc2->data.varInt)==0.0)
                    {
                           print_error(E_SEM_DIV_ZERO, "NELZE DELIT NULOU! [I_DIV]");
                    }
                   else
                    {
                        tmp.varDouble = (double)((double)(tHsrc1->data.varInt) / (double)(tHsrc2->data.varInt));
                    }
               }
              else if ((dataType1==VARDOUBLE || dataType1==VARINT) && (dataType2==VARDOUBLE || dataType2==VARINT))
               {
                   // vysledek bude double
                   TypeOF = VARDOUBLE;

                   // pokud prvni double a druhy int, int pretypuju a naopak
                   if (dataType1==VARDOUBLE && dataType2==VARINT)
                    {
                           // [===overujeme jestli se nedeli nulou===]
                          if ((double)(tHsrc2->data.varInt)==0.0)
                          {
                              print_error(E_SEM_DIV_ZERO, "NELZE DELIT NULOU! [I_DIV]");
                          }
                          else {
                             tmp.varDouble = (tHsrc1->data.varDouble / (double) (tHsrc2->data.varInt));
                          }
                    }
                   else if(dataType1==VARINT && dataType2==VARDOUBLE)
                    {
                          // [===overujeme jestli se nedeli nulou===]
                         if (tHsrc2->data.varDouble==0.0)
                         {
                             print_error(E_SEM_DIV_ZERO, "NELZE DELIT NULOU! [I_DIV]");
                         }
                         else {
                            tmp.varDouble = ( (double)(tHsrc1->data.varInt) / tHsrc2->data.varDouble);
                         }
                    }
                    else if(dataType1==VARDOUBLE && dataType2==VARDOUBLE)
                    {
                         // [===overujeme jestli se nedeli nulou===]
                         if (tHsrc2->data.varDouble==0.0)
                         {
                             print_error(E_SEM_DIV_ZERO, "NELZE DELIT NULOU! [I_DIV]");
                         }
                         else {
                            tmp.varDouble = (tHsrc1->data.varDouble / tHsrc2->data.varDouble);
                         }
                    }

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
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         // overeni jestli data ubec existuji
         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_CON]");

         // nactu typ dat src1 & src2
         dataType1 = tHsrc1->type;
         dataType2 = tHsrc2->type;

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

//---------------------------- POROVNAVANI ----------------------------------------------------------------

         /*========================I_G=================================================================*/
         case I_G:

         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         // nactu id src1,src2 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         // tmp
         tokenValue tmp;

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item v lokalni ani globalni TS neexistuje I_G");

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

         // nactu id src1,src2 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_GE]");

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

         // nactu id src1,src2 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_L]");

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

         // nactu id src1,src2 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_LE]");

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

         // nactu id src1,src2 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_ET]");

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

         // nactu id src1,src2 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // nactu id src1,src2 & result z HASH nebo GLOBAL hash tabulky
         tHsrc2 = (TblSearch (active_htable, src2));
         tHsrc2 = (tHsrc2!=NULL) ? tHsrc2 : (TblSearch (global_htable, src2));

         tHresult = (TblSearch (active_htable, result));

         datTyp=0;

         if (tHsrc1==NULL || tHsrc2==NULL) print_error(E_SEM_VAR, "item src1 nebo src2 v lokalni ani globalni TS neexistuje [I_NET]");

         if (tHsrc1->type==tHsrc2->type)
         {
              TypeOF = VARBOOL;

              if (tHsrc1->type==VARINT)
              {
                 if (tHsrc1->data.varInt != tHsrc2->data.varInt) 
                    datTyp = 1;
                 else 
                    datTyp = 0;
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
             datTyp = 1;
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
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));
 
         if (tHsrc1==NULL){
            int i=0;
            while (i<10)
            {
              if (strcmp (src1,vestaveneFunkce[i] )==0)
              {
                            // Lokalni TS
            tHashTbl *local_htable_func;

            // naalokovani a inicializaci LOKALNI TS fce
            tableInit(&local_htable_func);

            // push adresy lokalni TS fce na stack
            pushStack(g_ptrs->function_stack,local_htable_func,NULL,ActiveList);//(TLitem*)(tHsrc1->data.pointer)
                break;
              }
              i++;
            }
          if (i==10)
          {
            print_error(E_SEM_FCE, "id funkce v lokalni ani globalni TS neexistuje [I_TSW]");
          }
          
        }
         else
         {
            // Lokalni TS
            tHashTbl *local_htable_func;

            // naalokovani a inicializaci LOKALNI TS fce
            tableInit(&local_htable_func);

            // push adresy lokalni TS fce na stack
            pushStack(g_ptrs->function_stack,local_htable_func,NULL,ActiveList);//(TLitem*)(tHsrc1->data.pointer)
         }
         break;


         /*========================I_PARAM=========================*/
         case I_PARAM:
         // nactu id src1,src2 & result z INSTRUKCE
         src1 = instr->src1;
         result = instr->result;

         // lokalni TS funkce
         local_htable_Fce = (topStack(g_ptrs->function_stack))->hashTbl;

         // nactu data na prohledani 3 TS
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         if (tHsrc1==NULL) {
             print_error(E_SEM_VAR, "id funkce v lokalni ani globalni TS neexistuje [I_PARAM]");
         }
         else
         {
            TblInsert (local_htable_Fce, result, tHsrc1->data, tHsrc1->type);
         }
         break;


         /*========================I_CHCKPAR=========================*/
         case I_CHCKPAR:
         // nactu id src1 & result z INSTRUKCE
         src1 = instr->src1;
         result = instr->result;

         tHsrc1 = (TblSearch (active_htable, src1));
         
         if (tHsrc1==NULL) print_error(E_SEM_PARAM, "nespravny pocet parametru funkce [I_CHCKPAR]");
         else
         {
             TblInsert(active_htable,result,tHsrc1->data,tHsrc1->type);
         }

         break;

         /*========================I_CALL=========================*/
         case I_CALL:
         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;
                        int i=0;
                        while (i<10)
                        {
                                if (strcmp (src1,vestaveneFunkce[i] )==0)
                                {
                                        (*fun[i])((topStack(g_ptrs->function_stack))->hashTbl, active_htable);
                                        popStack(g_ptrs->function_stack);
                                        break;        
                                }
                                i++;
                        }
                        if (i<10)
                        {
                        break;
                        }


         // nactu id src1 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));
 
         // lokalni TS dane funkce
         local_htable_Fce = (topStack(g_ptrs->function_stack)->hashTbl);

         if (tHsrc1==NULL) print_error(E_SEM_FCE, "id funkce v lokalni ani globalni TS neexistuje [I_CALL]");
         else
         {
              //ulozeni navratove adresy na stack
              (topStack(g_ptrs->function_stack))->NavrInstrukce = ActiveList->Act;

              // prepnu listy
              ActiveList = (TList*)(tHsrc1->data.pointer);
              ActiveFirstItem(ActiveList);
              muzuskocit = 0;

              // prepnuti kontextu
              active_htable = local_htable_Fce;
         }
         break;


         /*========================I_RETURN=========================*/
         case I_RETURN:
         // nactu id src1 z INSTRUKCE
         src1 = instr->src1;

         // nactu id src1 z HASH nebo GLOBAL hash tabulky
         tHsrc1 = (TblSearch (active_htable, src1));
         tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         if (((topStack(g_ptrs->function_stack))->NavrInstrukce)==NULL){
            print_error(E_OK,"");
         }

         if (tHsrc1==NULL) {
            print_error(E_SEM_VAR, "id v lokalni ani globalni TS neexistuje [I_RETURN]");
         }
         else
         {
              // aktivuju list z vrcholu stacku a nastav aktivni instrukci
              ActiveList = (topStack(g_ptrs->function_stack))->list;
              ActivePtrItem(ActiveList,(topStack(g_ptrs->function_stack))->NavrInstrukce);
              TblDelete((topStack(g_ptrs->function_stack))->hashTbl);
              // pop stack
              popStack(g_ptrs->function_stack);

              //Aktivuj lokalni TS fce
              active_htable = (topStack(g_ptrs->function_stack))->hashTbl;

              //vloz polozku
              TblInsert (active_htable, "$", tHsrc1->data, tHsrc1->type);
         }
         break;


//---------------------------- SKOKYY --------------------------------------------------------------


         /*========================I_JMP=========================*/
         case I_JMP:
         // nactu result z INSTRUKCE
         result = instr->result;

         // nactu result z GLOBALNI HASH tabulky
         tHresult = (TblSearch (global_htable, result));

         // pokud by cil skoku nebyl v globalni tabulce ->chyba
         if (tHresult==NULL) print_error(E_INTERN, "cil skoku v lokalni TS neexistuje [I_JMP]");
         else
         {
              // aktivuje instrukci v prave aktivnim listu
              ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
         }
         break;


         /*========================I_JZ=========================*/
         case I_JZ:
          // nactu id src1,src2 & result z INSTRUKCE
          src1 = instr->src1;
          result = instr->result;

          // nactu id src1,src2 & result z HASH tabulky
          tHsrc1 = (TblSearch(active_htable,src1));
          tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

          // nactu id src1,src2 & result z GLOBALNI HASH tabulky
          tHresult = (TblSearch (global_htable, result));

         // pokud by cil skoku, nebo zdrojova promenna nebyla v globalni tabulce ->chyba
         if (tHresult==NULL || tHsrc1==NULL) {
            print_error(E_INTERN, "item v lokalni ani globalni TS neexistuje I_JZ");
         }
         else
         {
             switch (tHsrc1->type)
             {
                case VARINT: // pokud je true, aktivuj zaznam
                  if (tHsrc1->data.varInt==0) jump = 1;
                  else jump = 0;
                break;

                case VARBOOL:
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
            { // proved skok
               ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
            }
         }
         break;


         /*========================I_JNZ=========================*/
         case I_JNZ:
          // nactu id src1,src2 & result z INSTRUKCE
          src1 = instr->src1;
          result = instr->result;

          // nactu id src1,src2 & result z HASH tabulky
          tHsrc1 = (TblSearch(active_htable,src1));
          tHsrc1 = (tHsrc1!=NULL) ? tHsrc1 : (TblSearch (global_htable, src1));

         // pokud by cil skoku, nebo zdrojova promenna nebyla v globalni tabulce ->chyba
         if (tHresult==NULL || tHsrc1==NULL) print_error(E_INTERN, "item v lokalni ani globalni TS neexistuje I_JNZ");
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
            { // proved skok
               ActivePtrItem (ActiveList,((TLItem *)tHresult->data.pointer));
            }
         }
         break;


         /*========================I_LAB=========================*/
         case I_LAB:
         break;

         default:
          print_error(E_SYN,"Neznama instrukce");
     }

      if(muzuskocit){
        /*posun se na dalsi instrukci*/
          ActiveNextItem(ActiveList);
      }

  
  }
       // printf("================KONEC============================\n");
       // printf("%p\n",(void *)active_htable);
       // printf("instruction list\n");
       // PrintList(ActiveList);
       // printf("================\n");
       // TblPrint(global_htable);
       // TblPrint(active_htable);
       // printf("============================================\n");
       // printf("============================================\n");
       // printf("============================================\n");
           
// end func
}


/// FUNKCE ZJISTI, kdy ma a nema pretypovat promenne
int max (int a, int b, TIType type) 
{

double temp;
int is_ok=0;

double g = (double) INT_MAX;
double l = -g;

 switch (type) 
 {
   case I_ADD:
   temp = (double)a + (double)b;
   if ((temp>=l) && (temp<=g)) 
   {
      is_ok=1;  // je to ok tak 1
   }
   else 
   {
      is_ok=0;
   }
  break;

  case I_SUB:
   temp = (double)a - (double)b;
   if ((temp>=l) && (temp<=g)) 
   {
      is_ok=1;  // je to ok tak 1
   }
   else 
   {
      is_ok=0;
   }
  break;

  case I_MUL:
   temp = (double)a * (double)b;
   if ((temp>=l) && (temp<=g)) 
   {
      is_ok=1;  // je to ok tak 1
   }
   else 
   {
      is_ok=0;
   }
  break;

  default:
  break;
 }


return is_ok;
}
