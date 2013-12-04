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

#pragma once

// VÝČET JEDNOTLIVÝCH INSTRUKCÍ => [operation, src1, src2, result]
typedef enum {

//===== aritmeticke operace ==============
		 // src1 src2 result
   I_ASS,  // = ||| src1 ...  result 
   I_ADD,  // + ||| src1 src2 result
   I_SUB,  // - ||| src1 src2 result
   I_MUL,  // * ||| src1 src2 result
   I_DIV,  // / ||| src1 src2 result

//===== Řetězcový operátor ==================

   I_CON, // . (konkatenance řetězců)

//===== logické operace ==================

   I_G,    // >
   I_GE,   // >=
   I_L,    // <
   I_LE,   // <=
   I_E,    // ==
   I_NE,   // !=
   I_ET,   // ===
   I_NET,  // !==

//===== call & return fce =================

   I_CALL,     // 
   I_RETURN,  // 

//===== vestavene funkce =================

   I_TYPE, // BOOLVAL,DOUBLEVAL,INTVAL,STRVAL ---> NEHOTOVA
   I_SORT, // sort string
   I_FIND, // find string
   I_STRL, // str.lenght
   I_PUTS, // put string
   I_GETS, // GET string
   I_GETSU,// GET SUBstring

//===== skoky ==========================

   I_JMP,  // nepodmíněný
   I_JZ,   // jump if zero        (false)
   I_JNZ,  // jump if not zero    (true)
   I_LAB,  // navěští, no effect

} TIType;


// STRUKTURA INSTRUKCE
typedef struct {
  TIType operation;
  char *src1;
  char *src2;
  char *result;
} TInstr;

// STRUKTURA POLOZKY SEZNAMU
typedef struct tlitem
{
  struct tlitem *Next;
  TInstr *Instruction; // or (void *) Instruction
} TLItem;
  
// STRUKTURA SEZNAMU  
typedef struct
{
  TLItem *Act;    // ukazatel na aktivni prvek
  TLItem *First;  // ukazatel na prvni prvek
  TLItem *Last;   // ukazatel na posledni prvek
} TList;
