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

relation chains[100][100];
relation chains1[100][100];
relation chains2[100][100];

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

void printString(char string[]){
	int i = 0;
	while(string[i] != '\0'){
		printf("%c", string[i]);
		i++;
	}
}

void printRelation(relation pRelation){
	printString(pRelation.initialGene);
	printf("  --  ");
	printString(pRelation.finalGene);
	printf("  --  ");
	printf("%lf\n", pRelation.value);
}

void printChain(relation chain[], int size){
	printf("SIZE: %i\n", size);
	for(int i  = 0; i < size; i++){
		printRelation(chain[i]);
	}
	printf("WHAT\n");
}

void copyString(char str1[], char str2[]){
    for (int i = 0; i < MAX_DESC; i++){
        str1[i] = str2[i];
    }
}

int compareDoubles(const void * a, const void * b){
	return ( *(double*)a - *(double*)b );
}

int compareRelations(const void * a, const void *b){
	relation f = *((relation*)a);
	relation s = *((relation*)b);
	if(f.value > s.value) return -1;
	if(f.value < s.value) return 1;
	return 0;
}

int * findChains(relation *relations, int relationsAmount, int maxChains){

	int chainsUsed = 1;//Amount of chains created

	int activeChains[100];//Marca 1 en la posicion de una cadena que aun se puede usar, 0 si se debe ignorar

	static int relationsInChain[100];//Amounts of relations within a Chain
	//int lengthTakenInA[100];//Distance used in Chromosome A
	//int lengthTakenInB[100];//Distance used in Chromosome B, if value > 0.50

	int i, j, k, y, n, m, dummy, varTemporal, newArray, newArray2, newSize, posInitialGene, posFinalGene, chainsToAdd, toIgnore;

	relation currentRelation = relations[0];
	y = 0;

	//Generate initial chain
	copyString(chains[0][0].initialGene, currentRelation.initialGene);
	copyString(chains[0][0].finalGene, currentRelation.finalGene);
	chains[0][0].value = currentRelation.value;

	copyString(chains[0][1].initialGene, currentRelation.finalGene);
	copyString(chains[0][1].finalGene, "**");
	chains[0][1].value = 0;

	activeChains[0] = 1;
	relationsInChain[0] = 2;

	////For each relation recived
	for (i = 1; i < relationsAmount; i++) {
		toIgnore = 0;
		chainsToAdd = 0;
		currentRelation = relations[i];
		printf("\n---------------------------------------Current relation: "); printRelation(currentRelation);
		printf("\nChains to check: %i\n", chainsUsed);
		
		//Check for each existing chain
		for (j = 0; j < chainsUsed; j++) {
			relation *currentChain = chains[j];
			printf("\n\n+++++++++++++++++++++++++Value of J: %i\n", j);	///Chain being checked
			printf("\nChain to check: \n"); printChain(currentChain, relationsInChain[j]);
			
			if(activeChains[j] == 1){//If chain is active then evaluate
				
				posInitialGene = -1;			///Reset position of initial gene to 0
				posFinalGene = -1; 				///Reset position of final gene to 0
				for (k = 0; k < relationsInChain[j]; k++) {
					//Set position of initial gene to the relation which contains it
					if (strcmp(currentRelation.initialGene, currentChain[k].initialGene) == 0) {
						posInitialGene = k;
					}
					//Set position of final gene to the relation which contains it
					if (strcmp(currentRelation.finalGene, currentChain[k].finalGene) == 0) {
						posFinalGene = k;
					}
				}
				printf("\nposInitialGene: %i  -  posFinalGene: %i\n", posInitialGene, posFinalGene);

				if(posInitialGene == -1 && posFinalGene == -1){//Relation not yet found in map
					continue;
				} else if(posInitialGene == posFinalGene){
					printf("INITIAL AND FINAL EQUAL\n");
					printf("%lf\n", currentChain[posInitialGene].value);
					printf("%lf\n", currentRelation.value);
					printf("%i\n", (int)(currentChain[posInitialGene].value - currentRelation.value));
					if((int)(currentChain[posInitialGene].value - currentRelation.value) != 0){
						activeChains[j] = 0;
						printf("RIGHT HERE\n");
					}
				} else if (posInitialGene != -1 && posFinalGene == -1){

					n = 0;// int n2 = 0;
					m = 0;// int m2 = 0;
					dummy = j;
					varTemporal = relationsInChain[j];

					newArray = chainsUsed + ((dummy - toIgnore) * 2);
					newArray2 = newArray + 1;

					//////////////////////////////////VALIDAR PARA DISTANCIAS
					//Create alternate chain 1
					newSize = varTemporal + 1;
					while(n < varTemporal){
						if(m == posInitialGene){
							copyString(chains1[y][m].initialGene, currentChain[n].initialGene);
							copyString(chains1[y][m].finalGene, currentRelation.finalGene);
							chains1[y][m].value = currentRelation.value;
							m++;
							n++;
						} else if(m == posInitialGene + 1){
							int tempN = n - 1;
							copyString(chains1[y][m].initialGene, currentRelation.finalGene);
							copyString(chains1[y][m].finalGene, currentChain[n].initialGene);
							chains1[y][m].value = currentChain[tempN].value - currentRelation.value;
							m++;
						} else {
							copyString(chains1[y][m].initialGene, currentChain[n].initialGene);
							copyString(chains1[y][m].finalGene, currentChain[n].finalGene);
							chains1[y][m].value = currentChain[n].value;
							n++;
							m++;
						}
					}
					//printf("\nChain b %i: \n", y); printChain(r, relationsInChain[y]); printf("\n");

					n = 0; m = 0;
					while(n < varTemporal){
						if(posInitialGene == 0){
							if(m == posInitialGene){
								copyString(chains2[y][m].initialGene, currentRelation.finalGene);
								copyString(chains2[y][m].finalGene, currentChain[n].initialGene);
								chains2[y][m].value = currentRelation.value;
								m++;
							} else if(m == posInitialGene +1){
								copyString(chains2[y][m].initialGene, currentChain[n].initialGene);
								copyString(chains2[y][m].finalGene, currentChain[n].finalGene);
								chains2[y][m].value = currentChain[n].value;
								n++;
								m++;
							} else {
								copyString(chains2[y][m].initialGene, currentChain[n].initialGene);
								copyString(chains2[y][m].finalGene, currentChain[n].finalGene);
								chains2[y][m].value = currentChain[n].value;
								n++;
								m++;
							}
						}else {
							if(m == posInitialGene - 1){
								copyString(chains2[y][m].initialGene, currentChain[n].initialGene);
								copyString(chains2[y][m].finalGene, currentRelation.finalGene);
								chains2[y][m].value = currentChain[n].value - currentRelation.value;
								m++;
							} else if(m == posInitialGene){
								copyString(chains2[y][m].initialGene, currentRelation.finalGene);
								copyString(chains2[y][m].finalGene, currentChain[n].finalGene);
								chains2[y][m].value = currentRelation.value;
								n++;
								m++;
							} else {
								copyString(chains2[y][m].initialGene, currentChain[n].initialGene);
								copyString(chains2[y][m].finalGene, currentChain[n].finalGene);
								chains2[y][m].value = currentChain[n].value;
								n++;
								m++;
							}
						}
					}
					//printf("\nChain a %i: \n", z); printChain(chains[z], relationsInChain[z]); printf("\n");

					for (int h = 0; h < newSize; h++) {
						copyString(chains[newArray][h].initialGene, chains1[y][h].initialGene);
						copyString(chains[newArray][h].finalGene, chains1[y][h].finalGene);
						double tempValue = chains1[y][h].value;
						chains[newArray][h].value = tempValue;

						copyString(chains[newArray2][h].initialGene, chains2[y][h].initialGene);
						copyString(chains[newArray2][h].finalGene, chains2[y][h].finalGene);
						double tempValue2 = chains2[y][h].value;
						chains[newArray2][h].value = tempValue2;
					}

					relationsInChain[newArray] = newSize;
					relationsInChain[newArray2] = newSize;

					activeChains[dummy] = 0;
					activeChains[newArray] = 1;
					activeChains[newArray2] = 1;
					chainsToAdd += 2;
					y++;

				} else if(posInitialGene != -1 && posFinalGene != -1 && posInitialGene != posFinalGene){
					//Genes separated in chain, chain is not valid
					printf("\n\nCHAIN BECOMES INNACTIVE\n");
					activeChains[j] = 0;
				}

			} else {//Else ignore
				toIgnore++;
				printf("\n\nCHAIN IS INNACTIVE\n");
			}
		}
		/*
			if (joinedToChain == false) {//Chain is completely independent of others, MAY BE TAKEN OUTSIDE OF INITIAL FOR
				//printf("\n\nNOT JOINED\n");
				copyString(chains[chainsUsed][0].initialGene, currentRelation.initialGene);
				copyString(chains[chainsUsed][0].finalGene, currentRelation.finalGene);
				chains[chainsUsed][0].value = currentRelation.value;

				copyString(chains[chainsUsed][1].initialGene, currentRelation.finalGene);
				copyString(chains[chainsUsed][1].finalGene, "**");
				chains[chainsUsed][1].value = 0;

				relationsInChain[chainsUsed] = 2;
				activeChains[chainsUsed] = 1;
				chainsToAdd = 1;
			}
		*/
		chainsUsed += chainsToAdd;
		printf("\nCURRENT CHAINS: %i\n", chainsUsed);
		for(int t = 0; t < chainsUsed; t++){
			printf("Chain %i with %i relations: ", t, relationsInChain[t]);
			if(activeChains[t] == 1) { printf("ACTIVE\n"); } else { printf("INNACTIVE\n");}
			printChain(chains[t], relationsInChain[t]);
			printf("\n\n");
		}
		printf("END HERE\n");
	}
	relationsInChain[0] = chainsUsed;
	return relationsInChain;
}

void createCromosmomeMaps(){
	relation r1 = {"A", "B", 0.24};
    relation r2 = {"A", "F", 0.16};
    relation r3 = {"A", "C", 0.08};
    relation r4 = {"F", "B", 0.08};

	relation relations[] = {r2,r1,r3,r4};

    printChain(relations, 4);
    
    printf("SORT\n");
    
    qsort(relations, 4, sizeof(relation), compareRelations);    
    printChain(relations, 4);

	int *relationsInChain = findChains(relations, 4, 100);
	//int chainsUsed = relationsInChain[0];
	//printf("RESULT------------------------------------------------------\n");
	//for(int i = 0; i < chainsUsed; i++){
	//	printChain(chains[i], relationsInChain[i]);	
	//	printf("\nCHAIN\n");
	//}
}