#include<stdio.h>
#define lenght_coff = 6 ; 
#define M  6        // lenght of coefficient i=whic are going to be multiplied wiht x
#define N  6        // lenght of coefficient i=whic are going to be multiplied wiht y  
#define input_len  23
/////////////////NOTE THESE ARE NORMATLISED COEFFICENTS ........!!!!!!!! , NORMALISED MY Y(0)
float static coff_x[M] = {0.0004, 0.0018, 0.0037, 0.0037, 0.0018, 0.0004 } ;
float static coff_y[N] = {1.0000, -3.4789, 5.0098, -3.6995, 1.3942, -0.2138} ;

float static input_to_filter[] = {8, 9, 4,6,4,2,4,6,74,65,4,4,7,5,65,1,8,5,65,7,54,57,54};
float static buffer_pastValues[N] = {0, 0, 0, 0, 0, 0};
float static buffer_input[M] = {0, 0, 0, 0, 0, 0};
float static past_values[M] ;
float static input ; 
float static output ;
float static *outPtr;
float static *inPtr ;

//defining my filter 
void  myFilter(void ){

	int i_x = M ; 
	int i_y = N ;
	float value = 0 ;  

   	for(i_x=M;i_x>0;i_x--) buffer_input[i_x]=buffer_input[i_x-1];
	buffer_input[0]=(*inPtr);
   

	int i = 0 , j = 1 ; 
	for ( i = 0 ; i < M; i++) { value = value + coff_x[i]*buffer_input[i];  printf("inPtr %f\n", value);}
	//for ( j = 1 ; j < itr_y, j++)  value = value - coff_y[j]*output_data[n-j];
	for ( j = 0 ; j < N; j++)  value = value - coff_y[j]*buffer_pastValues[j];

	(*outPtr) =  value ;  

	for(i_y=N;i_y>1;i_y--)
    	buffer_pastValues[i_y-1]=buffer_pastValues[i_y-2];

   	buffer_pastValues[1]= value;
}

// as degree of denominator for iir filters are always greater or equals to the numerator
// so choosing N as parameter of variation   

int main( void ){

	inPtr = &input ;
	outPtr = &output;
	int k = 0 ;
	int s = 0 ;

	while( k < input_len){
				input = input_to_filter[k];
				myFilter() ;
				printf("%f\n", output);
				k++ ; 
			}
}
