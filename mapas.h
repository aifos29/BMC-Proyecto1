#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

FILE * fileTableData;
char buffer[9];
int totalGenCount;
int usefullData = 0;

typedef struct{
	char initialGene[25];
	char finalGene[25];
	double value;
}relation;

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



bool checkTableData(double probabilities[totalGenCount][totalGenCount]){
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
		return true;
	} else {
		return false;
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

	//REVISAR
	void swapString(char **x, char **y, int size){
		char *temp = *x;
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

	//REVISAR
	void swapRelationValues(relation arr[], int i, int j){
		swap(&arr[i]->value, &arr[j]->value);
		swapString(&arr[i]->finalGene, &arr[j]->finalGene, 25);
		swapString(&arr[i]->initialGene, &arr[j]->initialGene, 25);
	}
	//REVISAR
	int partitionR(relation arr[], int low, int high, bool inverted){
		double pivot = arr[high]->value;
		int i = (low - 1);

		for (int j = low; j < high; j++){
			if(inverted){
				if(arr[j]->value > pivot){
					i++;
					swapRelationValues(arr, i, j);
				}
			} else {
				if(arr[j]->value <= pivot){
					i++;
					swapRelationValues(arr, i, j);
				}
			}
		}
	}
	//REVISAR
	void quickSortR(relation arr[], int low, int high, bool inverted){
		if(low < high) {
			int partitionPoint = partitionR(arr, low, high, inverted);
			quickSortR(arr, low, partitionPoint - 1, inverted);
			quickSortR(arr, partitionPoint + 1, high, inverted);
		}
	}

char** findChains(relation relations[], int relationsAmount, int maxChains){
	relation chains[maxChains][relationsAmount];
	
	int chainsUsed = 0;//Amount of chains created

	int activeChains[maxChains];//Marca 1 en la posicion de una cadena que aun se puede usar, 0 si se debe ignorar

	int relationsInChain[maxChains];//Amounts of relations within a Chain
	int lengthTakenInA[maxChains];//Distance used in Chromosome A
	int lengthTakenInB[maxChains];//Distance used in Chromosome B, if value > 0.50

	int i, j, k, posInitialGene, posFinalGene;

	bool joinedToChain;
	for (i = 0; i < relationsAmount; i++) {
		joinedToChain = false;
		for (j = 0; j < chainsUsed; j++) {
			posInitialGene = -1;
			posFinalGene = -1;
			for (k = 0; k < relationsInChain[j]; k++) {
				if (strcmp(relations[i]->initialGene, chains[j][k]->initialGene) != 0) {
					posInitialGene = k;
				}
				if (strcmp(relations[i]->finalGene, chains[j][k]->finalGene) != 0) {
					posFinalGene = k;
				}
			}

			if(posInitialGene == -1 && posFinalGene == -1){//Relation not yet found in map
				joinedToChain = false;
			} else if(posInitialGene == posFinalGene){
				//Continues
			} else if (posInitialGene != -1 && posFinalGene == -1){
				chains[chainsUsed] = chains[j];
				relation temporal, temporal2;

				//Array frente
				temporal->initialGene = relation[i]->finalGene;
				temporal->finalGene = chain[j][posInitialGene]->finalGene;
				temporal->value = chain[j][posInitialGene]->value - relation[i]->value;

				chain[j][posInitialGene]->finalGene = relation[i]->finalGene;
				chain[j][posInitialGene]->value = relation[i]->value;

				//Array atras
				temporal2->initialGene = relation[i]->finalGene;
				temporal2->finalGene = chain[j][posInitialGene]->initialGene;
				temporal2->value = relation[i]->value;

				if(posInitialGene != 0){
					chain[chainsUsed][posInitialGene - 1]->finalGene = relation[i]->finalGene;
					chain[chainsUsed][posInitialGene - 1]->value -= relation[i]->value;
				}

				//Shift right chain relations
				for(k = relationsInChain[j]; k > posInitialGene + 1; k--){
					chain[j][k] = chain[j][k-1];
					chain[chainsUsed][k] = chain[chainsUsed][k-1];
				}
				chain[j][posInitialGene + 1] = temporal
				chain[chainsUsed][posInitialGene + 1] = chain[chainsUsed][posInitialGene];
				chain[chainsUsed][posInitialGene] = temporal;
				joinedToChain = true;

				relationsInChain[j]++;
				relationsInChain[chainsUsed] = relationsInChain[j];
				chainsUsed++;
				break;
			}
		}
		if (!joinedToChain) {//Chain is completely independent of others, MAY BE TAKEN OUTSIDE OF INITIAL FOR
			chains[chainsUsed][0] = relations[i];
			chains[chainsUsed][1]->initialGene = relations[i]->finalGene;
			chains[chainsUsed][1]->finalGene = "";
			chains[chainsUsed][1]->value = 0;

			relationsInChain[chainsUsed] = 2;
			activeChains[chainsUsed] = 1;
			chainsUsed++;
		}
	}
	return chains;
}

void createCromosmomeMaps(){
	relation relations[25];
	char chains[25] = findChains(relations, 25, 100);
}