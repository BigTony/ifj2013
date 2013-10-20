typedef enum { 

//===== aritmeticke operace ==============

           // dst, src1, src2
   I_ASS,  // =       
   I_ADD,  // +       
   I_SUB,  // -
   I_MUL,  // *  
   I_DIV,  // /   
   I_POW,  // ^

//===== logické operace ==================

   I_G,    // >
   I_GE    // >=
   I_L,    // <
   I_LE,   // <=
   I_E,    // ==
   I_NE,   // !=

//===== vestavene funkce =================

   I_TYPE, // BOOLVAL,DOUBLEVAL,INTVAL,STRVAL ---> NEHOTOVA
   I_SORT, // sort string
   I_FIND, // find string
   I_STRL, // str.lenght
   I_PUTS, // put string
   I_GETS, // GET string
   I_GETSU,// GET SUBstring

//===== skoky =================

   I_JMP,  // nepodmíněný
   //// NEDODELANO ------------

} TInstr;
