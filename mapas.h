#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

FILE * fileTableData;
char buffer[9];
int totalGenCount;
int usefullData = 0;

/*Read Files*/
void fillBuffer(int _val) {
	if (strlen(buffer) == 0) {
		char dato= (char)_val;
		char auxiliar[] = {dato,'\0'};
		strcat(buffer,auxiliar);
	} else{
		char dato = (char)_val;
		char auxiliar[]={dato,'\0'};
		strcat(buffer,auxiliar);
	}
}


void cleanBuffer(){
	memset(buffer,'\0',strlen(buffer));
}

int escaner() {
	cleanBuffer();

	int ch;

	while (feof(fileTableData)==0) {   
		ch = fgetc(fileTableData);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
		
			return 1;
		}
		fillBuffer(ch);

	}
	return 0;
}

int countObjectsFiles(char * address){
	fileTableData = fopen(address,"r");
	int ch;


	while(feof(fileTableData) == 0) {  
		ch = fgetc(fileTableData);
		if (ch == '\n'){
			
			break;
		}
		if (ch==';'){
			totalGenCount ++;
		}
	}

	
	
	fclose(fileTableData);
	return totalGenCount;
}

void readFile(char Data[totalGenCount][totalGenCount][5],char * address,char header[totalGenCount][30]){
	fileTableData = fopen(address,"r");
	int startData = 0;
	int c;
	int columns = 0;
	int rows= 0;
	while (rows<totalGenCount+1 && feof(fileTableData) == 0){
		while (columns < totalGenCount+1){
			c = escaner();
			if (startData==1 && c == 1){
				char * data = malloc(5);
	 			strcpy(data,buffer);
				strcpy(Data[rows-1][columns],data);		
			}
			if (startData==0 && c==1){
				strcpy(header[columns],buffer);
			}
			if (startData==0 && c == 0){
				startData = 1;
			}
			columns ++;

		}
		
		columns = 0;
		rows ++;
		
	}
	

}

void parseTableData(char* probabilities[totalGenCount][totalGenCount]){

}

int checkTableData(double probabilities[totalGenCount][totalGenCount]){
	int good[3] = {1,1,1};
	int i, j;
	int mentioned[totalGenCount];
	
	//Check wrong probabilities
	for(i = 0; i < totalGenCount; i++){
		for(j = 0; j < totalGenCount; j++){
			if(i != j){//Ignores diagonals

				//Probabilities within 0 and 1
				if(probabilities[i][j] < 0 || probabilities[i][j] > 1){
					good[0] = 0;
				}

				//Table symmetry
				if(probabilities[i][j] != probabilities[j][i]){
					good[1] = 0;
				}

				//Marks a gene as mentioned
				if(probabilities[i][j] != -1){
					mentioned[i] = 1;
					mentioned[j] = 1;
					usefullData = 0;
				}
			}
		}
	}

	//Checks that every gene has been mentioned

	//Esto creo que no hace falta por lo que dice la especificacion que solo se consideran genes con informacion
	for(i = 0; i < totalGenCount; i++){
		if(mentioned[i] == 0){
			good[2] = 0;
		}
	}

	if(good[0] == 1 && good[1] == 1 && good[2] == 1){
		return 1;
	} else {
		return 0;
	}
}

//Quicksort
//Adapted from: http://quiz.geeksforgeeks.org/quick-sort/
	void swap(double *x, double *y) {
		double temp;
		temp = *x;
		*x = *y;
		*y = temp;
	}

	int partition(double arr[], int low, int high, bool inverted) {
		double pivot = arr[high];
		int i = (low - 1);

		for (int j = low; j < high; j++){
			if(inverted){
				if(arr[j] > pivot){
					i++;
					swap(&arr[i], &arr[j]);
				}
			} else {
				if(arr[j] <= pivot){
					i++;
					swap(&arr[i], &arr[j]);
				}
			}
		}
		swap(&arr[i + 1], &arr[high]);
		return (i + 1);
	}

	void quickSort(double arr[], int low, int high, bool inverted) {
		if(low < high) {
			int partitionPoint = partition(arr, low, high, inverted);
			quickSort(arr, low, partitionPoint - 1, inverted);
			quickSort(arr, partitionPoint + 1, high, inverted);
		}
	}

void createCromosmomeMaps(){
	char maps[25][totalGenCount][25];
}