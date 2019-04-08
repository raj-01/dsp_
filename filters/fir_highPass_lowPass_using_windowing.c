#include <stdio.h>
//#include "usbstk5515.h"
//#include "usbstk5515_gpio.h"
#include <math.h>


#define fs 500
#define f2 100
#define f1 30

float static wp=M_PI*2*f1/fs;
float static ws=M_PI*2*(f1+30)/fs;
//float static wc=(ws+wp)/2;
//float static sin_;
//float static *hd;
//float static *h_n_low;
//float static *h_n_high;
int static M;
float static output;
float static input;
float static *outPtr;
float static *inPtr;
float static *coeff;
#define in_len 10 
float static data_in[in_len] = { 1, 2, 2, 4, 5, 6, 8, 4, 5, 6};
  // data_in len 
#define M_max 20 
float static buffer[M_max] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float static wn[M_max] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //M , lets its max value be 20
float static hd[M_max] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float static h_n_low[M_max] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float static h_n_high[M_max] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//M , lets its max value be 20
///////////////////////////////////////////////////////////////////////////

void Rectangular(void){
	float m1;
	int n;
	m1= (0.92*M_PI)/(ws-wp);
	printf("m1 = %f\n", m1 );
	 M= ceil(m1) ;
	printf("M vyu= %i\n",M );
	for(n=0; n<=M; n++){
		wn[n]=1;
	}
		
}
void Hamming(void){
	float m2;
	int n;
	m2= (3.32*M_PI)/(ws-wp);
	M=ceil(m2);
	for(n=0; n<=M; n++){
		wn[n]=0.54-0.46*cos(2*M_PI*n/(M-1));     // doubt 
	}
}
void Hanning(void){
	float m3;
	int n;
	m3= (3.11*M_PI)/(ws-wp);
	M=ceil(m3) ;
	for(n=0; n<=M; n++){
		wn[n]=0.5-0.5*cos((2*M_PI*n)/(M-1));  // doubt 
	}
}

//////////////////////////////////////////////////////////////////////////////
void lowpass(void){
	float k = 0.0001;
	int i = 0 ; 
	float wc=(ws+wp)/2;
	for(i=0; i<=M; i++){
		hd[i]=(wc/M_PI)*(sin(wc*(k-((M-1)/2)))/(wc*(k-((M-1)/2))));
		h_n_low[i]=(hd[i])*(wn[i]);
		k++ ; 
	}
	
}

void highpass(void){
	float k = 0.000001;
	int i = 0 ; 
	float  wc=(ws+wp)/2;
	for(i=0; i<=M; i++){
		/////////////////////////////////////////////////////////////////////// doubt 
		hd[i]=(sin(M_PI*k)/(M_PI*k))-sin(wc*k)/(M_PI*k);    
		////////////////////////////////////////////////////////////////////// doubt 
		h_n_high[i]=hd[i]*wn[i];
		k++; 
	}
}

// we have to remember M past input values 


void linearbuff(void)
{
	int i = M;
	float accumulator = 0 ; 
   	for(i=M;i>0;i--)
    	buffer[i]=buffer[i-1];

   	buffer[0]=(*inPtr);
   //	printf("h_n_high =%f\n",h_n_low[3]);
   	for(i=0;i<=M;i++){
   		accumulator+=(( buffer[i] )*(( float)*(coeff+i)));
   		//printf("buffer[i] =%d\n",buffer[i]);
   		//printf("coff[i] =%d\n",(float)*(coeff+i));
   		//printf("accumulator =%f\n",accumulator );
   	}

   	(*outPtr)=((float)accumulator);
}


int main(void){
	printf("M =%d\n",1);
	Rectangular();
	lowpass();
	int k = 0 ;
	for(k =0 ; k<=M; k++) printf("%f\n", h_n_low[k]);	
	
	coeff = &h_n_low[0]; 	//coefs is coefficient of impulse response defined in fdacoefs_int.h
	inPtr = &input;		//inPtr is a globally declared pointer to a Int16
	outPtr = &output;
	

	int itr = 0 ;
	for( itr = 0; itr <= M ; itr++) buffer[itr] = 0 ; 
	//int tmp_l=500+M;
	int temp_1 = in_len + (M+1) -1 ;  
	float array[temp_1];
	int j = 0 ; 
	printf("temp_1 =%d\n",temp_1);

	// FOR REAL TIME OPERATION JUST REMOVE j < temp_1 and replace it by while ( true ) 
	// and also remove if condition 
	Int16 LEFT_DATA,RIGHT_DATA;
    SYS_EXBUSSEL = 0x6100;
    USBSTK5515_init( );
    AIC3204_config(freq_48);

	//while(j<temp_1)
	while(true)
		    {
		    	/*if(j>=in_len)
		    	{
		    		input=0;
		    	}
		    	else input=data_in[j];
		    	*/
		     	//input=data_in[j];
		     	//printf("in main input =%f\n", input );
		    	while((Rcv & I2S0_IR) == 0);  // Wait for interrupt pending flag
	    		//LEFT_DATA = I2S0_W0_MSW_R;  // 16 bit left channel received audio data
	    		//RIGHT_DATA = I2S0_W1_MSW_R;  // 16 bit right channel received audio data
	    		input=  I2S0_W0_MSW_R ;

		    	linearbuff();

		    	while((Xmit & I2S0_IR) == 0);
	    		I2S0_W0_MSW_W = output;  // 16 bit left channel transmit audio data
	    		//I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data

		    	//array[j]=output;
		    	printf("output=%f\n" ,output);

		      j=j+1;
		    }

	 return 0 ;

}


main( void )
{
    
    
    while(1)
    {

		    
    }   
    
    I2S0_CR = 0x00;
    return 0;
}