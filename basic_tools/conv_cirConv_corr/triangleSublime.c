/*
 * main.c
 */

#include <stdio.h>
//#include "usbstk5515.h"
//#include "usbstk5515_gpio.h"
#define num_pulse 5.0
#define period 2.0     //in seconds
#define peak_location   50.0     //in  percentage
#define resolution  32767.0
///int * square( int len );

//fucntion to genrate only one time period square wave
int * triangle( int * len ){

  // defing sampling time Ts = 1/(10*period)
  int Ts = (resolution/(20*period));
  // intialising an array of size period/Ts
  int total_data_points = (period*resolution)/Ts ;
  *len = total_data_points ;

  printf(" total_data_points =%d\n", total_data_points);
  if( total_data_points > 100 ){
    printf("total_data_points = %d\n", total_data_points);
    printf("%s\n"," array bound excceds reduce sampling time") ;
  }

  static int triangle_data[ 100 ] ;
    printf(" (total_data_points)*(duty/100) = %lf\n", (total_data_points)*(peak_location/100));


    int  i ;
    int j ;

    int max_x = total_data_points ;
    int peak_location_x = max_x*(peak_location/100) ;

    int peak_to_end_dis = max_x - peak_location_x;
if (peak_to_end_dis == 0)
{
  for(i = 0 ; i < peak_location_x ; i++){
    triangle_data[i] = (resolution/peak_location_x)*i  ;                  //resolution is acting as max height of triangle 
      printf("%d\n", triangle_data[i]);
    }
    return triangle_data ; 
}
else if ( peak_to_end_dis == max_x){
  for( j = peak_location_x ; j < max_x ; j++){
      triangle_data[j] =  -(resolution*j)/(peak_to_end_dis) + max_x*resolution/peak_to_end_dis;
    }
  return triangle_data ;
}
else {
  for(i = 0 ; i < peak_location_x ; i++){
    triangle_data[i] = (resolution/peak_location_x)*i  ;                  //resolution is acting as max height of triangle 
      printf("%d\n", triangle_data[i]);
      //printf(" num of ones %d\n", );
  }

  for( j = peak_location_x ; j < max_x ; j++){
      triangle_data[j] =  -(resolution*j)/(peak_to_end_dis) + max_x*resolution/peak_to_end_dis;
    }
  return triangle_data ;
  }
}




int main(void)
{
  //pointer to an array
  int length ;
  int * data ;
  data = triangle(  &length ) ;
  int  i ;
  int j ;
  for( i= 0 ; i < num_pulse ; i++){
      for( j =0 ; j < length ; j++){
      printf("output=%d\n",*(data + j)) ;
      }
  }
  
    return 0;
}

