#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void merge(char endArray[],char tempArray[],int left,int mid,int right);
void merge_sort(char endArray[],char tempArray[],int left,int right);


void merge_sort(char endArray[],char tempArray[],int left,int right){
	int mid;
	if (left < right){
      mid = (left + right) / 2;
      merge_sort(endArray, tempArray, left, mid);
      merge_sort(endArray, tempArray, mid + 1, right);
      merge(endArray, tempArray, left, mid, right);
    }
}

void merge(char endArray[],char tempArray[], int left,int mid,int right){
	return;
}

void initString(){
	char endArray[6] = { 'a', 'd', 'c','g','z','b' };
	char tempArray[6];
	merge_sort(endArray,tempArray,0,5);
}

int main(int argc,char** argv){
	initString();
	return 0;
}