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

#include <stdio.h>
#include "ilist.h"



// init seznamu instrukci
void InitList (TList *L) 
{
  L->Act    = NULL;
  L->Last   = NULL;
  L->First  = NULL;
}

// zrusi seznam instrukci
void DisposeList (TList *L) 
{
  TList *tmp = L->Act = L->Last = NULL;
  while (L->First!=NULL) 
  {
	tmp = L->First;
	L->First = L->First->Next;
	free (tmp);
  }
}


/**   
*
*  ---------	     ---------	      ---------
*  | main  |-------> | fce   |------->| fce2  |
*  ---------	     ---------	      ---------
*
**/

// vlozi instrukci na zacatek
void InsertFirst (TList *L,TInstr Instr) 
{
  TLItem *New;

  if ( (New=(TLItem *)malloc(sizeof(TLItem))) != NULL) 
  {
     New->Instruction = Instr;
     New->Next=L->First;
     L->First = New;
     
	
     if (!L->First->Next) 
	{
		L->Last = New;
	}
	
  }
}

// vlozi instrukci na konec
void InsertLast (TList *L,TInstr Instr) 
{
  TLItem *New;

  if ( (New=(TLItem *)malloc(sizeof(TLItem))) != NULL) 
  {
     New->Instruction = Instr;
     New->Next=NULL;

     if (L->First!=NULL) 
	{
		L->Last->Next = New;
	}
      else {
		L->First = New;
	}

	L->Last = New;
  }
}


/**
*
*  ---------
*  | main  |
*  ---------
*      |
*      |
*      |
*      |
*      v
*  ---------
*  | instr |
*  ---------
*      |
*      |
*      v  
*    
/*


/// doplni se



/*****************************************************************************
*
*        PRESUN AKTIVITY
*
******************************************************************************/

// over aktivitu
int IsActiveItem (TList *L) 
{
	if (L->Act!=NULL) 
	{
		return 1;	
	}

  return 0;
}

// Aktivuj prvni prvek
void ActiveFirstItem (TList *L) 
{
	L->Act = L->First;
}

// Aktivuj posledni prvek
void ActiveLastItem (TList *L) 
{
	L->Act = L->Last;
}

// POSUN AKTIVITY NA DALSI PRVEK
void ActiveNextItem (TList *L) 
{
  if (L->Act!=NULL) 
  {
	L->Act = L->Act->Next;
  }
}

// AKTIVUJ POLOZKU DANOU POINTEREM -> podmíněné & nepodmíněné skoky v interpreteru
void ActivePtrItem (TList *L,TLItem *Item) 
{
  if (Item!=NULL) 
  {
	L->Act = Item;
  }
}



/* funkce pro generovani instrukci */

void InsertInstrc(TList *L,TIType InstrNumber,){
  TInstr Instr;
  InsertLast(L,Instr);
}
