#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define val 2
#define MAX_DESC 25

FILE * fileTableData;
char buffer[9];
int totalGenCount;
int usefullData = 0;

typedef struct{
	char initialGene[MAX_DESC];
	char finalGene[MAX_DESC];
	double value;
}relation;

double test[3] = {0.16, 0.24, 0.8};
double test2[2];

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


void cleanBuffer(){ memset(buffer,'\0',strlen(buffer)); }

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
	void copy(relation *r1, relation *r2){
		*r1 = *r2;
	}

	void merging(int low, int mid, int high) {
		int l1, l2, i;
		for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++){
			if (test[l1] <= test[l2]) {
				test2[i] = test[l1++];
			} else {
				test2[i] = test[l2++];
			}
		}
		while(l1 <= mid) {
			test2[i++] = test[l1++];
		}
		while(l2 <= high) {
			test2[i++] = test[l2++];
		}
		for(i = low; i <= high; i++){
			test[i] = test2[i];
		}
	}

	void sort(int low, int high) {
		int mid;
		if (low < high) {
			mid = low + (high - low) / 2;
			sort(low, mid);
			sort(mid + 1, high);
			merging(low, mid, high);
		} else {
			return;
		}
	}



	//REVISAR
	void mergingR(relation arr[], int low, int mid, int high, bool inverted){
		int i, j, k;
		int n1 = mid - low + 1;
		int n2 = high - mid;

		relation L[n1], R[n2];

		printf("Here 1\n");
		for(i = 0; i < n1; i++){
			//copy(&L[i], &arr[low + i]);
			strcpy(L[i].initialGene, arr[low + i].initialGene);
			strcpy(L[i].finalGene, arr[low + i].finalGene);
			L[i].value = arr[low + i].value;
		}
		for (j = 0; j < n2; j++) {
			//copy(&R[i], &arr[mid + low + j]);
			strcpy(R[j].initialGene, arr[mid + low + j].initialGene);
			strcpy(R[j].finalGene, arr[mid + low + j].finalGene);
			R[j].value = arr[mid + low + j].value;
		}
		
		i = 0;
		j = 0;
		k = low;

		printf("Here 2\n");
		while(i < n1 && j < n2) {
			if (L[i].value <= R[j].value) {
				//copy(&arr[k], &L[i]);
				strcpy(arr[k].initialGene, L[i].initialGene);
				strcpy(arr[k].finalGene, L[i].finalGene);
				arr[k].value = L[i].value;
				i++;
			} else {
				//copy(&arr[k], &R[j]);
				strcpy(arr[k].initialGene, R[j].initialGene);
				strcpy(arr[k].finalGene, R[j].finalGene);
				arr[k].value = R[j].value;
				j++;
			}
		}

		printf("Here 3\n");
		while (i < n1) {
			//copy(&arr[k], &L[i]);
			strcpy(arr[k].initialGene, L[i].initialGene);
			strcpy(arr[k].finalGene, L[i].finalGene);
			arr[k].value = L[i].value;
			i++;
			k++;
		}

		while (j < n2) {
			//copy(&arr[k], &R[j]);
			strcpy(arr[k].initialGene, R[j].initialGene);
			strcpy(arr[k].finalGene, R[j].finalGene);
			arr[k].value = R[j].value;
			j++;
			k++;
		}

		printf("Here 6\n");
	}
	//REVISAR
	void sortR(relation arr[], int low, int high, bool inverted){
		int mid;
		if(low < high) {
			mid = low + (high - low) / 2;
			sortR(arr, low, mid, inverted);
			sortR(arr, mid + 1, high, inverted);
			mergingR(arr, low, mid, high, inverted);
		} else {
			return;
		}
	}

void printString(char *string){	printf("%s  -  ", string); }

void printRelation(relation pRelation){
	printString(pRelation.initialGene);
	printString(pRelation.finalGene);
	//printf("%c", '\0');
	printf("%lf\n", pRelation.value);
}

void printChain(relation chain[], int size){
	for(int i  = 0; i < size; i++){
		printRelation(chain[i]);
	}
}

void copyString(char str1[], char str2[]){
    int i = 0;
    while (str2[i] != '\0'){
        str1[i] = str2[i];
        i++;
    }
    str1[i] = '\0';
}

void copyRelation(relation r1, relation r2){
	copyString(r1.initialGene, r2.initialGene);
	copyString(r1.finalGene, r2.finalGene);
	r1.value = r2.value;
}

int * findChains(relation relations[], int relationsAmount, int maxChains, relation chains[maxChains][relationsAmount]){
	//relation chains[maxChains][relationsAmount];
	
	int chainsUsed = 0;//Amount of chains created

	int activeChains[100];//Marca 1 en la posicion de una cadena que aun se puede usar, 0 si se debe ignorar

	static int relationsInChain[100];//Amounts of relations within a Chain
	//int lengthTakenInA[100];//Distance used in Chromosome A
	//int lengthTakenInB[100];//Distance used in Chromosome B, if value > 0.50

	int posInitialGene, posFinalGene;

	bool joinedToChain;
	////For each relation recived
	for (int i = 0; i < relationsAmount; i++) {
		printf("---------------------------------------Current relation: "); printRelation(relations[i]);
		relation currentRelation = relations[i];
		joinedToChain = false;
		printf("Chains to check: %i\n", chainsUsed);
		int chainsToAdd = 0;
		int toIgnore = 0;
		//Check for each existing chain
		for (int j = 0; j < chainsUsed; j++) {
			relation *currentChain = chains[j];
			int j2 = j + 1;
			printf("\n\n+++++++++++++++++++++++++Value of J: %i\n", j);	///Chain being checked

			printf("Chain to check:\n"); printChain(chains[j], relationsInChain[j2]);

			posInitialGene = -1;			///Reset position of initial gene to 0
			posFinalGene = -1; 				///Reset position of final gene to 0
			for (int k = 0; k < relationsInChain[j2]; k++) {
				//Set position of initial gene to the relation which contains it
				if (strcmp(relations[i].initialGene, chains[j][k].initialGene) == 0) {
					posInitialGene = k;
				}
				//Set position of final gene to the relation which contains it
				if (strcmp(relations[i].finalGene, chains[j][k].finalGene) == 0) {
					posFinalGene = k;
				}
			}
			printf("posInitialGene: %i  -  posFinalGene: %i\n", posInitialGene, posFinalGene);

			if(posInitialGene == -1 && posFinalGene == -1){//Relation not yet found in map
				joinedToChain = false;
			} else if(posInitialGene == posFinalGene){
				//Continues
			} else if (posInitialGene != -1 && posFinalGene == -1){
				if(activeChains[j] == 1){
					int dummy = j;
					int y = chainsUsed + ((dummy - toIgnore)*2);

					//////////////////////////////////VALIDAR PARA DISTANCIAS
					//Create alternate chain 1
					printf("CREATE ALTERNATE 1\n");
					int varTemporal = relationsInChain[j2];
					int n = 0;
					int m = 0;
					relation r1[100];
					while(n < varTemporal){
						if(m == posInitialGene){
							copyString(r1[m].initialGene, currentChain[n].initialGene);
							copyString(r1[m].finalGene, currentRelation.finalGene);
							r1[m].value = currentRelation.value;
							m++;
							n++;
						} else if(m == posInitialGene + 1){
							copyString(r1[m].initialGene, currentRelation.finalGene);
							copyString(r1[m].finalGene, currentChain[n].initialGene);
							r1[m].value = currentChain[n-1].value - currentRelation.value;
							m++;
						} else {
							copyString(r1[m].initialGene, currentChain[n].initialGene);
							copyString(r1[m].finalGene, currentChain[n].finalGene);
							r1[m].value = currentChain[n].value;
							n++;
							m++;
						}
					}
					relationsInChain[y + 1] = varTemporal + 1;
					printf("Chain %i: \n", y); printChain(r1, relationsInChain[y + 1]); printf("\n");

					int z = y + 1;
					//Create alternate chain 2
					printf("CREATE ALTERNATE 2\n");
					int n2 = 0;
					int m2 = 0;
					relation r2[100];
					while(n2 < varTemporal){
						if(m2 == posInitialGene){
							copyString(r2[m2].initialGene, currentRelation.finalGene);
							copyString(r2[m2].finalGene, currentChain[n2].initialGene);
							r2[m2].value = currentRelation.value;
							m2++;
						} else if(m2 == posInitialGene + 1){
							copyString(r2[m2].initialGene, currentChain[n2].initialGene);
							copyString(r2[m2].finalGene, currentChain[n2].finalGene);
							r2[m2].value = currentChain[n2].value;
							n2++;
							m2++;
						} else {
							copyString(r2[m2].initialGene, currentChain[n2].initialGene);
							copyString(r2[m2].finalGene, currentChain[n2].finalGene);
							r2[m2].value = currentChain[n2].value;
							n2++;
							m2++;
						}
					}
					relationsInChain[z + 1] = varTemporal + 1;
					printf("Chain %i: \n", z); printChain(r2, relationsInChain[z + 1]); printf("\n");

					for(int h = 0; h < varTemporal + 1; h++){
						copyString(chains[y][h].initialGene, r1[h].initialGene);
						copyString(chains[y][h].finalGene, r1[h].finalGene);
						chains[y][h].value = r1[h].value;

						copyString(chains[y + 1][h].initialGene, r2[h].initialGene);
						copyString(chains[y + 1][h].finalGene, r2[h].finalGene);
						chains[y + 1][h].value = r2[h].value;
					}

					activeChains[dummy] = 0;
					activeChains[y] = 1;
					activeChains[z] = 1;
					chainsToAdd += 2;
					joinedToChain = true;
				} else {
					toIgnore++;
				}
			} else if(posInitialGene != -1 && posFinalGene != -1 && posInitialGene != posInitialGene){
				//Genes separated in chain, chain is not valid
				activeChains[j] = 0;
				joinedToChain = true;
			}
		}
		if (joinedToChain == false) {//Chain is completely independent of others, MAY BE TAKEN OUTSIDE OF INITIAL FOR
			printf("NOT JOINED\n");
			strcpy(chains[chainsUsed][0].initialGene, relations[i].initialGene);
			strcpy(chains[chainsUsed][0].finalGene, relations[i].finalGene);
			chains[chainsUsed][0].value = relations[i].value;

			strcpy(chains[chainsUsed][1].initialGene, relations[i].finalGene);
			strcpy(chains[chainsUsed][1].finalGene, "");
			chains[chainsUsed][1].value = 0;

			relationsInChain[chainsUsed + 1] = 2;
			activeChains[chainsUsed] = 1;
			chainsToAdd = 1;
		}
		printf("Chains to add: %i", chainsToAdd);
		chainsUsed += chainsToAdd;
		printf("\n\nCURRENT CHAINS\n");
		for (int a = 0; a < chainsUsed; a++){
			printf("Chain: %i\n", a);
			printChain(chains[a], relationsInChain[a + 1]);
			if(activeChains[a] == 1) { printf(" ACTIVE");}
			printf("\n");
		}
		printf("\n\n");
	}
	relationsInChain[0] = chainsUsed;
	return relationsInChain;
}

void createCromosmomeMaps(){
	relation relations[3];
	relation r1 = {"A", "B", 0.24};
    relation r2 = {"A", "F", 0.16};
    relation r3 = {"A", "C", 0.08};
    //relation r4 = {"F", "B", 0.08};
    relations[0] = r1;
    relations[1] = r2;
    relations[2] = r3;
    //relations[3] = r4;

    printChain(relations, 4);
    
    printf("SORT\n");
    
    //sortR(relations, 0, 3 - 1, true);
    
    //printf("TEST\n");
    
    printChain(relations, 4);

	relation chains[100][100];
	int *relationsInChain = findChains(relations, 3, 100, chains);
	int chainsUsed = relationsInChain[0];
	printf("HERE\n");
	for(int i = 0; i < chainsUsed; i++){
		//printChain(chains[i], 4);	
		//printf("\n");
	}
}