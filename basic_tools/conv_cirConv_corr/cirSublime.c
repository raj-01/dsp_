/*
 * main.c
 */

#include <stdio.h>
//#include "usbstk5515.h"
//#include "usbstk5515_gpio.h"
#define IR_length 12
#define In_length 12
#define tmp_l In_length //longest lengths

int x[In_length] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 1};
int array[tmp_l];
void circbuff();
int *inPtr;
int *outPtr;
int *coeff;
int coefs[IR_length] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 1};
int input;
int output;


static int buffer[In_length]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //buffer of longest sequence

void circbuff(void )
{
	int i;
  int j;
	long int accumulator=0;
	int temp;

     //circular shifting the buffer 
    temp=buffer[In_length-1];

    for(j=In_length-1 ; j>0 ; j--){
          buffer[j] = buffer[j-1];
          //printf(" buffer[j]=%d\n", buffer[j]);
        }
    buffer[0]=temp;
	

   	for(i=0;i<IR_length;i++)
   		accumulator+=(((long int)buffer[i])*((long int)(*(coeff+i))));

   	(*outPtr)=((int)accumulator);
    
}




int main(void)
 {
int j=0,i=0;
coeff = &coefs[0]; 	//coefs is coefficient of impulse response defined in fdacoefs_int.h
inPtr = &input;		//inPtr is a globally declared pointer to a int
outPtr = &output;
//	SYS_EXBUSSEL = 0x6100;
//    USBSTK5515_init( );
//temp_length=IR_length+In_length-1;

//static int inbuffer[In_length]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	for(i=0;i<In_length;i++){
	buffer[i]= x[In_length - i -1 ];
  printf("%d\n", buffer[i]);
}

    while(j<tmp_l) // loop forever
    {
        /*  Read input data using a probe-point connected to a host file. */
      //printf("x=%d\n",x[j]);

    	circbuff();

    	array[j]=output;
    	printf("output=%d\n",array[j]);

      j=j+1;
    }
    return 0;
}

/*
 * FUNCTION:     Read input signal and write processed output signal
 *              using ProbePoints
 * PARAMETERS: none.
 * RETURN VALUE: none.
 */
/*static void dataInput()
{
    do data I/O
    return;
}*/


