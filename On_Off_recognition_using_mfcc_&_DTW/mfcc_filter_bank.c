
#include<math.h>
#include<stdio.h>
#define pi 3.14
#define fs 48000 
#define frame_len 1024
#define log_in_len 10

#define highest_freq  fs/2
#define lowest_freq  1
#define num_filters 40 
float static H[num_filters][frame_len/2] = {0} ;  
extern float psd_data[frame_len] ; 
extern float cepstral[num_filters];

float static S[num_filters] = {0} ;
int static mel_to_hz[(num_filters+2)] = {0};
float static mel[num_filters] = {0};
float static mel_highest = 2595*log10(1+highest_freq/700);
float static mel_lowest = 2595*log10(1+lowest_freq/700);
void triangular_filter(){
	/* 1 to  compresed scal , than forming triagle ,say 40, in number */
	// converting frequency to mel scale 
	
	mel[0] = mel_lowest;
	float mel_temp = 0;
	int i = 0 ;
	for( int i=1; i<num_filters+2;i++) {
		mel[i] = mel[i-1] + ( mel_highest-mel_lowest)/(num_filters+1);
	}
	// mel to hertz conversion 
	int j = 0 ; 
	for( int j=0; j<num_filters+2;j++) {
		 mel_temp = (((mel[j])/2595)  );
		mel_to_hz[j] =  (int)((frame_len/2+52)*(700*((pow(10,mel_temp)) - 1))/(fs/2));		
	}
	

	for(int m=1 ; m<=40;m++){
		for( int k = 0 ; k<frame_len/2;k++){ 
			if( k < mel_to_hz[m-1]) H[m-1][k] = 0 ;
			else if( mel_to_hz[m-1]<= k && k <= mel_to_hz[m]){
				H[m-1][k] =   (float)(k - mel_to_hz[m-1])/( mel_to_hz[m]-mel_to_hz[m-1 ] );
			}
			else if ( mel_to_hz[m]<= k && k <= mel_to_hz[m+1]){
				H[m-1][k] = (float)(mel_to_hz[m+1]-k)/(mel_to_hz[m+1]-mel_to_hz[m] );
			}
			else H[m-1][k] = 0 ;
		}
	}
		
	
}


void mffc_coef(){
	   	/*			N–1		  2 
	   		S(m) =   E  |S(k)|  X Hm(k), 0<= m <= M– 1
					 0	
		S(k)^2 is nothing but psd_data , we already did square */
	int m = 0 ,j = 0;
	for (int m = 0; m < num_filters; m++){
		for (int j = 0; j<frame_len/2 ; j++){
			S[m] = (S[m] + (psd_data[j])*H[m][j]);
		} 
	}
	// calculating log and inverse DCT
	/* 	 M-1
		E amLog10(S(m))cos( pi*n(m-0.5)) , 0<=n<=C-1  ; am = 1/M for m =0  
		m=0					----------						and 2/M for m 0
								M	
				*/
	m = 1 ;
	int n = 0 , M = num_filters;
	for (int n = 0; n < M; n++){
			cepstral[n] =  (float)((log10(S[0]))*cos((pi*n*(0-0.5))/M))/M;
		for (int m = 1; m < num_filters; m++){
			cepstral[n] = cepstral[n] + (float)(2*(log10(S[m]))*cos((pi*n*(m-0.5))/M))/M;
		}
	}

	/*for (int itr = 0; itr < 40; itr++)
	{
			printf("%f\n", cepstral[itr] );
	}*/

}