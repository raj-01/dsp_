#include <stdio.h>
#include <stdlib.h>
//reading and writing to a file in c 

int main(){
	FILE *fp ;
	char data_char ;
	fp = fopen("data.txt","r"); // reading mode 
	if(fp == NULL){
		perror(" file not found");
		printf("%s\n","file not present" );
		exit(0);
	}
	// for reading charachters 
	/*while( (data_char = fgetc(fp)) != EOF) printf("%c", data_char);*/

	//for reading integers/float  
	float data_float ; 
	
	while( !feof(fp)){
		fscanf (fp, "%f", &data_float);
		printf("%f\n", data_float);
		}

	fclose(fp);
	return 0 ;
}