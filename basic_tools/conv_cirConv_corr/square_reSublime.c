/*
 * main.c
 */

#include <stdio.h>
//#include "usbstk5515.h"
//#include "usbstk5515_gpio.h"
#define num_pulse 5.0
#define period 2.0     //in seconds
#define duty 20.0       // in percentage
#define resolution  32767
///int * square( int len );

//fucntion to genrate only one time period square wave
int * square( int * len ){

	// defing sampling time Ts = 1/(10*period)
	int Ts = (resolution/(10*period));
	// intialising an array of size period/Ts
	int total_data_points = (period*resolution)/Ts ;
	*len = total_data_points ;

	printf(" total_data_points =%d\n", total_data_points);
	if( total_data_points > 100 ){
		printf("total_data_points = %d\n", total_data_points);
		printf("%s\n"," array bound excceds reduce sampling time") ;
	}

	static int square_data[ 100 ] ;
		printf(" (total_data_points)*(duty/100) = %lf\n", (total_data_points)*(duty/100));

		int  i ;
		int j ;
	for(i = 0 ; i < (total_data_points)*(duty/100) ; i++){
		square_data[i] = 1  ;
			printf("%d\n", square_data[i]);
			//printf(" num of ones %d\n", );
	}

	for( j = (total_data_points)*(duty/100) ; j < (total_data_points) ; j++){
			square_data[j] = 0;
		}
	return square_data ;
}


int main(void)
{
	//pointer to an array
	int length ;
	int * data ;
	data = square(  &length ) ;
	int  i ;
	int j ;
	for( i= 0 ; i < num_pulse ; i++){
			for( j =0 ; j < length ; j++){
			printf("output=%d\n",*(data + j)) ;
			}
	}
	
    return 0;
}



