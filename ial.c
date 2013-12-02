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
#include "errors.h"

void tableInit(ProgramState *main){
	tHashTbl *hash_table;
	if((hash_table = malloc(sizeof(tHashTbl)))==NULL){
		main->err_code = ALLOC_ERR;
		return;
	}
	if((hash_table->tableItems = malloc(ALLOC*sizeof(item)))==NULL){
		main->err_code = ALLOC_ERR;
		return;
	}
	hash_table->size = ALLOC;
	main->table = hash_table;
}

// nahradit lepsi
int hashCode ( itemKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/// HLEDANI
item* TblSearch (tHashTbl *tab, itemKey key) 
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
void TblInsert (tHashTbl *tab, itemKey key, itemValue data) 
{

 item *AddNew = NULL;

  int  Hashed = hashCode(key);

  if (TblSearch(tab, key)!=NULL) 
  {
	/// provedu aktualizaci dat,pri shode,kdyztak se udela oprava... 
	TblSearch(tab, key)->data=data;
  } 
  else 
  {
	if((AddNew = malloc (sizeof(item))!=NULL) 
	{
		// new
		AddNew->itemKey = key;
		AddNew->data = data; /// [**data**]

		/// navazuju
	        AddNew->nextItem=tab->tableItems[Hashed];
                tab->tableItems[Hashed]=AddNew->nextItem;
	} 
	else 
	{ 		
		return;// a kurwa, => osetreni
	}
   }
}

/// PRECTE HODNOTU PROMENNE
itemValue* TblRead (tHashTbl *tab, itemKey key) 
{
  item *read;
  if ((read=TblSearch(tab,key)) != NULL)
  {
	return (read->data);	
  }
return NULL;
}


/// SMAZU VSECHNO
void TblDelete (tHashTbl *tab, itemKey key) 
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



