#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE * fileTableData;
char buffer[9];
int totalGenCount;

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