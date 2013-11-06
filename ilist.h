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

typedef enum { 

//===== aritmeticke operace ==============

   I_ASS,  // =       
   I_ADD,  // +       
   I_SUB,  // -
   I_MUL,  // *  
   I_DIV,  // /   
   I_POW,  // ^

//===== Řetězcový operátor ==================

   I_DOT,  // .  (konkatenance řetězců)     

//===== logické operace ==================

   I_G,    // >
   I_GE    // >=
   I_L,    // <
   I_LE,   // <=
   I_E,    // ==
   I_NE,   // !=
   I_ET,   // ===
   I_NET,  // !==

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
   I_JZ,   // jump if zero	 (false)
   I_JNZ,  // jump if not zero	 (true)
   I_LAB,  // 

   //// NEDODELANO ------------

} TIType;


typedef struct {
  TIType operation;
  void *src1;
  void *src2;
  void *result;
} TInstr;
