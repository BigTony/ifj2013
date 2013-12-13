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
	if ((strcmp(Titem->key,key)==0) || (Titem->key == key))
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
 //  item* Temp=NULL;
 //  for (int key=0;key<ALLOC;key++)
 //  {
	// while (tab->tableItems[key]!=NULL)
	// {
	//     Temp=tab->tableItems[key]->nextItem; /// ulozim nasledujici polozku
	//     free(tab->tableItems[key]->key);
	//     if(tab->tableItems[key]->type < STRING ){
	//     	if(tab->tableItems[key]->data.varString != NULL){
	//     		free(tab->tableItems[key]->data.varString);
	//     	}
	//     }
	//     free (tab->tableItems[key]);
	//     tab->tableItems[key] = Temp;         /// navazeme na nasledujici polozku
	// }
	// tab->tableItems[key]=NULL;	         /// polozka je jiz prazdna
 //  }
  free(tab);
}


/// FOR DEBUG HASH TABLE
void TblPrint( tHashTbl* tab )
{
  if (tab == NULL)
  {
    printf("je to null\n");
    return;
  }
	char pole[8][15]=
	{
	    "IDENTIFIKATOR\0",
	    "STRING\0",
	    "VARIABLE\0",
	    "VARINT\0",
	    "VARDOUBLE\0",
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
                        switch (ptr->type)
                        {
                           case VARINT:
			     printf (" (%s,%d,%s)",ptr->key,ptr->data.varInt, pole[ptr->type-1]);
                           break;

                           case VARDOUBLE:
			     printf (" (%s,%f,%s)",ptr->key,ptr->data.varDouble, pole[ptr->type-1]);
                           break;

                           case STRING:
			     printf (" (%s,%s,%s)",ptr->key,ptr->data.varString, pole[ptr->type-1]);
                           break;

                           case VARBOOL:
			     printf (" (%s,%d,%s)",ptr->key,ptr->data.varInt, pole[ptr->type-1]);
                           break;

                           default:
                             printf (" (%s,%d,%s)",ptr->key,ptr->data.varInt, pole[ptr->type-1]);
                           break;
                        }

			ptr = ptr->nextItem;
		   }
                }
		printf ("\n");
           i++;
	}
 }
///-/////////////////////-//
///-/Knuth-Morris-Pratt -//
///-////////////////////-//
 void createTableKmp(char* patt, int pattLen, int** fail)
{
    int i=2;
    int j=0;
    (*fail)[0]=-1;
    (*fail)[1]=0;
    for(;i<pattLen;i++)
    {
        j=(*fail)[i-1];
        while((j>0)&&(patt[j]!=patt[i-1]))
        {
            j=(*fail)[j];
        }
        (*fail)[i]=j+1;
    }
}

int getSubstringKmp(char* text,char* patt)
{
    int pattLen=strlen(patt);
    int* fail;
    if((fail=malloc(sizeof(int)*pattLen))==NULL){
        print_error(E_INTERN,"\nNealokovala se tabulka int* fail pro KMP algoritmus\n");
    }

    createTableKmp(patt,pattLen,&fail);

    int iT=0;int iP=0;
    while((text[iP+iT]!='\0')&&(patt[iP]!='\0'))
    {
        if(text[iP+iT]==patt[iP])
        {
            iP++;
        }
        else
        {
            iT=iT+(iP-(fail)[iP]);
            if(iP>0)
                iP=(fail)[iP];
        }
    }
    if(patt[iP]=='\0')
    {
            if(fail!=NULL){
            free(fail);}
        else{
            print_error(E_INTERN,"\nNeuvolnila se tabulka int* fail pro KMP algoritmus\n");
        }
    return iT;
    }

    else
    {
            if(fail!=NULL){
            free(fail);}
        else{
            print_error(E_INTERN,"\nNeuvolnila se tabulka int* fail pro KMP algoritmus\n");
        }
    return iT+1;
    }
}
///--------------------------------------//
///-------------------------------------//
}
//merge sort implementovan rekursivne
void merge_sort(char*in,char*out,int l,int r){
    int len=r-l;
    int mid=(len)/2+l;
    if (l<r)
    {
        merge_sort(in,out,l,mid);
        merge_sort(in,out,mid+1,r);
        msort(in,out,l,mid,mid+1,r);
    }

}
void msort(char*in,char*out,int l1,int r1,int l2,int r2){
    int ar=l1;
    int i=l1;

    while ((l1<=r1) &&(l2<=r2))
    {
        if (in[l1]<in[l2])
        {
            out[i++]=in[l1++];
        }
        else
        {
            out[i++]=in[l2++];
        }
    }
    while ((l1<=r1))
        {
            out[i++]=in[l1++];
        }
    while ((l2<=r2))
        {
            out[i++]=in[l2++];
        }
    memcpy(&(in[ar]),&(out[ar]),i-ar);
}


void full_merge_sort(char*array) {
int len = strlen(array)-1;
if (len > 0)
char*tmp;
{
	if(NULL!=(tmp=malloc(len*sizeof(char))))
	{
		merge_sort(array,tmp,0,len);
	 	free(tmp);
	}
	else 
	{
	print_error(E_INTERN,chyba pri alokaci pro merge_sort);
	}
}
}