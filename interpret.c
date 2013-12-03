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

#include "ial.c"
#include "ilist.h"
#include "interpret.h"
#include "errors.h"
#include <math.h>

void initOfAll () {
// fuck 1
// fuck 2
// fuck 3
}


void interpret (tHashTbl *table,TList *L) 
{
   TInstr    *instr;   // data aktualni instrukce
   TDataType *src1,*src2;
   TDataType *tmp;
   TDataType *result;

  while (IsActiveItem(L)) 
  {


      // nactu instrukci
      instr = (TInstr*) ReturnActiveInstr(L);

    /* -------------------------------------------------------------
     *	
     *  semantic controll 
     *
     * ------------------------------------------------------------ */


      switch (instr->operation) 
      {


	 /*========================I_ASS========================= (EKV.: mov) */ 
	 case I_ASS:

         src1 = instr->src1;
         result = instr->result;
	// type = instr->type;

	 TblInsert (table, result,src1,/* int type*/); 
	 break;

	 /*========================I_ADD=========================*/
	 case I_ADD:

         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

	if (/*TypeOfSearchedOperand1==BOOL*/ && /*TypeOfSearchedOperand2==BOOL*/) 
	{
	   if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
           {
	  	temp = (TblSearch (table, src1))->data + (TblSearch (table, src2))->data;
		TblInsert (table, result,temp,/* int type*/); 	
           }
	}
	else if (/*TypeOfSearchedOperand1==INTEGER*/ && /*TypeOfSearchedOperand2==INTEGER*/) 
	{
	   if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
           {
	  	temp = (TblSearch (table, src1))->data + (TblSearch (table, src2))->data;
		TblInsert (table, result,temp,/* int type*/); 	
           }
	}
	else if (/*TypeOfSearchedOperand1==DOUBLE*/ && /*TypeOfSearchedOperand2==DOUBLE*/) 
	{
	   if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
           {
	  	temp = (TblSearch (table, src1))->data + (TblSearch (table, src2))->data;
		TblInsert (table, result,temp,/* int type*/); 	
           }
	}

       /// kurwa tozasrany typovani
       /// dal podle tabulky v zadani...

	 break;

         /*========================I_SUB=========================*/
	 case I_SUB:
         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
         {
		temp = (TblSearch (table, src1))->data - (TblSearch (table, src2))->data;
		TblInsert (table, result,temp,/* int type*/); 	
         }
	 break;

	 /*========================I_MUL=========================*/
	 case I_MUL:

         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
         {
		temp = ((TblSearch (table, src1))->data) * ((TblSearch (table, src2))->data);
		TblInsert (table, result,temp,/* int type*/); 	
         }
	 break;

	 /*========================I_DIV=========================*/
	 case I_DIV:

         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
         {
		temp = ((TblSearch (table, src1))->data) / ((TblSearch (table, src2))->data);
		TblInsert (table, result,temp,/* int type*/); 	
         }
	 break;

	 /*========================I_POW=========================*/
	 case I_POW:

         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
         {
		temp =  pow((TblSearch (table, src1))->data, (TblSearch (table, src2))->data);
		TblInsert (table, result,temp,/* int type*/); 	
         }
	 break;

	 /*========================I_CON=========================*/ 
	 case I_CON:

         src1 = instr->src1;
         src2 = instr->src2;
         result = instr->result;

         if (TblSearch (table, src1)!=NULL  && TblSearch (table, src2)!=NULL) 
         {
		temp = ConcString(TblSearch (table, src1))->data,( (TblSearch (table, src2))->data);
		TblInsert (table, result,temp,/* int type*/); 	
         }
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
	ActiveNextItem (L);
  }

}
