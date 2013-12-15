// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin - xseidl01
// Slunsky Tomas - xsluns01
// Skacel David - xskace12
// Skyva Petr - xskyva02
////////////////////////////////
// ilist.h
//
//


#ifndef ILIST_H
#define ILIST_H
#include "errors.h"
// VÝČET JEDNOTLIVÝCH INSTRUKCÍ 3AC => [operation, src1, src2, result]
typedef enum {

//===== aritmeticke operace ==============
		 // src1 src2 result
   I_ASS,  // =   | src1 ...  result 0
   I_ADD,  // +   | src1 src2 result 1
   I_SUB,  // -   | src1 src2 result 2
   I_MUL,  // *   | src1 src2 result 3
   I_DIV,  // /   | src1 src2 result 4

//===== Řetězcový operátor, konkatenace ==

   I_CON,  // .   | src1 src2 result 5

//===== logické operace ==================

   I_G,    // >   | src1 src2 result 6
   I_GE,   // >=  | src1 src2 result 7
   I_L,    // <   | src1 src2 result 8 
   I_LE,   // <=  | src1 src2 result 9
   I_ET,   // === | src1 src2 result 10
   I_NET,  // !== | src1 src2 result 11

//===== call & return fce =================

   I_TSW,     //    | src1 ... ...
   I_PARAM,   //    | src1 ... ...
   I_CHCKPAR, //    | src1 ... result
   I_CALL,    //    | src1 ... ...
   I_RETURN,  //    | src1 ... ...

//===== skoky ==========================

   I_JMP,  // nepodmíněný                 || ...  ... result 24
   I_JZ,   // jump if zero        (false) || src1 ... result 25
   I_JNZ,  // jump if not zero    (true)  || src1 ... result 26
   I_LAB,  // znacka, no effect           || ...  ...  ...   27

} TIType;


// STRUKTURA INSTRUKCE
typedef struct TInstr{
  TIType operation;
  char *src1;
  char *src2;
  char *result;
} TInstr;

// STRUKTURA POLOZKY SEZNAMU
typedef struct TLItem
{
  struct TLItem *Next;
  TInstr *Instruction; // or (void *) Instruction
} TLItem;
  
// STRUKTURA SEZNAMU  
typedef struct TList
{
  TLItem *Act;    // ukazatel na aktivni prvek
  TLItem *First;  // ukazatel na prvni prvek
  TLItem *Last;   // ukazatel na posledni prvek
} TList;

void InitList (TList *L);
TList *CreateList ();
void DisposeList (TList *L) ;
void InsertFirst (TList *L,TInstr *Instr) ;
void InsertLast (TList *L,TInstr *Instr);
void InsertInstLast (TList *L,char *src1,char* src2,char* dest,TIType type) ;

int IsActiveItem (TList *L) ;
void ActiveFirstItem(TList *L);
void ActiveLastItem (TList *L) ;
void ActiveNextItem (TList *L) ;
void ActivePtrItem (TList *L,TLItem *Item) ;
TLItem * ReturnActiveItem (TList *L);
TInstr * ReturnActiveInstr (TList *L) ;

#endif
