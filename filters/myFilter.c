#include<stdio.h>
#define lenght_coff = 6 ; 
#define M  6        // lenght of coefficient i=whic are going to be multiplied wiht x
#define N  6        // lenght of coefficient i=whic are going to be multiplied wiht y  
#define input_len  23

/// normalised coefficeints 
float static coff_x[6] = {0.0004, 0.0018, 0.0037, 0.0037, 0.0018, 0.0004 } ;
float static coff_y[6] = {1.0000, -3.4789, 5.0098, -3.6995, 1.3942, -0.2138} ;
float static input_to_filter[] = {8, 9, 4,6,4,2,4,6,74,65,4,4,7,5,65,1,8,5,65,7,54,57,54};
float static past_values[M] ;
float static output_data[input_len]; 
//defining my filter 
float  myFilter( int n  ){

	int itr_y , itr_x; 
	float value =0 ; 
	if( n > M -1 ) itr_x = M -1;
	else itr_x = n ;

	if( n > N -1) itr_y = N-1 ;
	else itr_y = n ;

	int i , j; 
	for ( i = 0 ; i <= itr_x; i++) {
		 //printf("%s\n", "yes" );
	 value = value + coff_x[i]*input_to_filter[n -i] ;
		}
	for ( j = 1 ; j <= itr_y; j++) { 
		//printf("%s\n", "no" );
		value = value - coff_y[j]*output_data[n-j];
		}
	//for ( j = 1 ; j < itr_y; j++)  value = value - coff_y[j]*past_values[N-j];

	return value ; 
}

int main( void ){
	int k = 0 ;
	int s = 0 ;
	float filtered_data ;
	
	//float static past_values[ N ] ; 
	while( k < input_len){
				filtered_data = myFilter( k) ;
				printf("%f\n", filtered_data);
				output_data[k] = filtered_data ; 

				//for( s = 1 ; s < N ; s++) past_values[s] = past_values[s+1];
				//past_values[N-1] = filtered_data ;
				k= k + 1 ; 
			}
}
