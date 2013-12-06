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


#ifndef ILIST_H
#define ILIST_H
#include "errors.h"
// VÝČET JEDNOTLIVÝCH INSTRUKCÍ 3AC => [operation, src1, src2, result]
typedef enum {

//===== aritmeticke operace ==============
		 // src1 src2 result
   I_ASS,  // =   | src1 ...  result 
   I_ADD,  // +   | src1 src2 result
   I_SUB,  // -   | src1 src2 result
   I_MUL,  // *   | src1 src2 result
   I_DIV,  // /   | src1 src2 result

//===== Řetězcový operátor, konkatenace ==

   I_CON,  // .   | src1 src2 result 

//===== logické operace ==================

   I_G,    // >   | src1 src2 result
   I_GE,   // >=  | src1 src2 result
   I_L,    // <   | src1 src2 result
   I_LE,   // <=  | src1 src2 result
   I_ET,   // === | src1 src2 result
   I_NET,  // !== | src1 src2 result

//===== call & return fce =================

   I_CALL,     // 
   I_RETURN,   // 

//===== vestavene funkce =================

   I_TYPE, // BOOLVAL,DOUBLEVAL,INTVAL,STRVAL
   I_SORT, // sort string
   I_FIND, // find string
   I_STRL, // str.lenght
   I_PUTS, // put string
   I_GETS, // GET string
   I_GETSU,// GET SUBstring

//===== skoky ==========================

   I_JMP,  // nepodmíněný                 || ...  ... result  
   I_JZ,   // jump if zero        (false) || src1 ... result
   I_JNZ,  // jump if not zero    (true)  || src1 ... result
   I_LAB,  // znacka, no effect           || ...  ...  ...   

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
