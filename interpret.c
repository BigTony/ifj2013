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

#include "ilist.h"
#include "interpret.h"

void initOfAll () {
// fuck 1
// fuck 2
// fuck 3
}


void interpret (/*Tabulka_symbolu*/) 
{
   TInstr   *instruction;   // data aktualni instrukce
   TDataType *src1;
   TDataType *src2;
   TDataType *destination;

  while (/*instruction list!=NULL*/) 
  {

    /* -------------------------------------------------------------
     *	
     *  semantic controll 
     *
     * ------------------------------------------------------------ */

      switch (/*instruction*/) 
      {

	 /*========================I_ASS========================= (EKV.: mov) */ 
	 case I_ASS:
	 break;

	 /*========================I_ADD=========================*/
	 case I_ADD:
	 break;

         /*========================I_SUB=========================*/
	 case I_SUB:
	 break;

	 /*========================I_MUL=========================*/
	 case I_MUL:
	 break;

	 /*========================I_DIV=========================*/
	 case I_DIV:
	 break;

	 /*========================I_POW=========================*/
	 case I_POW:
	 break;

	 /*========================I_CON=========================*/ 
	 case I_CON:
	 break;

	 /*========================I_G=========================*/ 
	 case I_G:
	 break;

	 /*========================I_GE=========================*/
	 case I_GE:
	 break;

	 /*========================I_L=========================*/ 
	 case I_L:
	 break;

	 /*========================I_LE=========================*/ 
	 case I_LE:
	 break;

	 /*========================I_E=========================*/ 
	 case I_E:
	 break;

	 /*========================I_NE=========================*/ 
	 case I_NE:
	 break;


	 /*========================I_ET=========================*/ 
	 case I_ET:
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
	ActiveNextItem (/*TS_table->TList*/);
  }

}
