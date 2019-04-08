/*
 * main.c
 */

#include <stdio.h>
//#include "usbstk5515.h"
//#include "usbstk5515_gpio.h"
#define IR_length 12
#define In_length 12
#define corr_l IR_length+In_length-1

int x[In_length] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 1};
int array[corr_l];
void corrbuff();
int *inPtr;
int *outPtr;
int *coeff;
int coefs[IR_length] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 1};
int input;
int output;


static int buffer[In_length]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //buffer of longest sequence

void corrbuff(void)
{
	int i;
	long int accumulator=0;
	
   	for(i=IR_length-1;i>0;i--)
    	buffer[i]=buffer[i-1];

   	buffer[0]=(*inPtr);

   	for(i=0;i<IR_length;i++)
   		accumulator+=(((long int)buffer[i])*((long int)(*(coeff+i))));

   	(*outPtr)=((int)accumulator);
}




int main(void)
{
int j=0;
coeff = &coefs[0]; 	//coefs is coefficient of impulse response defined in fdacoefs_int.h
inPtr = &input;		//inPtr is a globally declared pointer to a int
outPtr = &output;
//	SYS_EXBUSSEL = 0x6100;
//    USBSTK5515_init( );
//temp_length=IR_length+In_length-1;

//static int inbuffer[In_length]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    while(j<corr_l) // loop forever
    {
        /*  Read input data using a probe-point connected to a host file. */
      //printf("x=%d\n",x[j]);

    	if(j>In_length-1)
    	{
    		input=0;
    	}
    	else input=x[In_length-j-1];

    	corrbuff();

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


