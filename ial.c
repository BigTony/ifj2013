// Project interpret IFJ2013
// Dev team:
////////////////////////////////
// Seidl Antonin  - xseidl01
// Slunsky Tomas  - xsluns01
// Skacel David   - xskace12
// Skyva Petr     - xskyva02
////////////////////////////////
// ial.c
// Iplementation sort and find functions
//
#include "ial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tableItemsInit(tHashTbl *hash_table){
	if(((hash_table->tableItems) = (item**)malloc(ALLOC*sizeof(item*)))==NULL){
		print_error(E_INTERN,"chyba pri alokaci tableItems");
		return;
	}
	hash_table->size = ALLOC;
	for(int i = 0;i<ALLOC;(hash_table->tableItems)[i++] = NULL);
}

void tableInit(tHashTbl **hash_table){
	if((*hash_table = (tHashTbl*)malloc(sizeof(tHashTbl)))==NULL){
		print_error(E_INTERN,"chyba pri alokaci tHashTbl");
		return;
	}
	tableItemsInit(*hash_table);
}


// nahradit lepsi
int hashCode ( char* key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % ALLOC );
}

/// HLEDANI
item* TblSearch (tHashTbl *tab, char* key) 
{
  item *Titem;
  int  Hashed = hashCode(key);

  for (Titem=tab->tableItems[Hashed]; Titem!=NULL; Titem=Titem->nextItem) 
  {
	if (strcmp(Titem->key,key)==0) 
	{
		return Titem;
	} 
  }
 return NULL;
}


/// VLOZENI NOVE POLOZKY
void TblInsert (tHashTbl *tab, char* key,tokenValue data, int type) 
{

  item *AddNew = NULL;

  int  Hashed = hashCode(key);


  if ((AddNew=TblSearch(tab, key))!=NULL) 
  {
	/// provedu aktualizaci dat,pri shode,kdyztak se udela oprava... 
	AddNew->data = data;
	AddNew->type = type;
  } 
  else 
  {
	if((AddNew = (item*) malloc (sizeof(item)))!=NULL) 
	{
		// new
		AddNew->key = key;
		AddNew->data = data; /// [**data**]
		AddNew->type = type;
		/// navazuju
	   AddNew->nextItem=tab->tableItems[Hashed];
      tab->tableItems[Hashed]=AddNew;
	} 
	else 
	{ 		
		print_error(E_INTERN,"Chyba alokace hst item" );
	}
   }
}

/// PRECTE HODNOTU PROMENNE
tokenValue* TblReadData (tHashTbl *tab, char* key) 
{
  item *read;
  if ((read=TblSearch(tab,key)) != NULL)
  {
	return &(read->data);	
  }
return NULL;
}

/// PRECTE TYP PROMENNE
int TblReadType (tHashTbl *tab, char* key) 
{
  item *read;
  if ((read=TblSearch(tab,key)) != NULL)
  {
	return (read->type);	
  }
return 0;
}


/// SMAZU VSECHNO
void TblDelete (tHashTbl *tab) 
{
  item* Temp=NULL;
  for (int key=0;key<ALLOC;key++) 
  {
	while (tab->tableItems[key]!=NULL) 
	{
	    Temp=tab->tableItems[key]->nextItem; /// ulozim nasledujici polozku
	    free (tab->tableItems[key]);
	    tab->tableItems[key] = Temp;         /// navazeme na nasledujici polozku
	}
	tab->tableItems[key]=NULL;	         /// polozka je jiz prazdna
  }
}


/// FOR DEBUG HASH TABLE
void TblPrint( tHashTbl* tab ) 
{

	char pole[8][15]= 
	{
	    "IDENTIFIKATOR\0",
	    "VARINT\0",
	    "VARDOUBLE\0",
	    "VARIABLE\0",
	    "STRING\0",
	    "VARBOOL\0",
	    "NIL\0"
	};	

	printf ("------------HASH TABLE--------------\n");
	printf ("----(id, data, typ)---------------\n");

        int i=0;
	while (i<ALLOC) 
        {
		printf ("%i:",i);
		item* ptr = (tab->tableItems)[i];
                if (ptr==NULL);
                else 
                {
		   while ( ptr != NULL ) 
                   {
			printf (" (%s,%d,%s)",ptr->key,ptr->data.varInt, pole[ptr->type-1]);
			ptr = ptr->nextItem;
		   }
                }
		printf ("\n");
           i++; 
	}
 }




