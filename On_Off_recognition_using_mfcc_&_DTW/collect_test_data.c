  #include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<string.h>
#include "psd_function.h"
#include "mfcc_filter_bank.c"
#define pi 3.14
#define fs 48000 
#define num_filters 40 
#define frame_len 1024
#define log_in_len 10  // we are computing 1024 point dft 
float haming_w_n[frame_len] = {0};
float hamming_filt_data[frame_len] = {0};
float each_frame_data[frame_len] = {0};
float filt_data_im[frame_len] = {0} ; // frame len is 1024
float psd_data[frame_len] = {0};
float cepstral[num_filters] = {0};


// getting hamming window arry  
void hamming_window(int M ){
	int i = 0 ;
	for (int i = 0; i < M; i++)
	{
		haming_w_n[i] = 0.54 - 0.46*cos(2*pi*i/(M-1));
	}
}

// filtering the input data using hammin window 
void hamming_filter(int M , float *each_frame_data){
	int i = 0 ;
	for (int i = 0; i < M ; i++)
	{
		hamming_filt_data[i] = each_frame_data[i]*haming_w_n[i];
	}
	
}




int main()
{
	for(int a=0;a<40;a++){	
		for(int b=0;b<512;b++){
			 H[a][b]= 0 ;
		} 
	}

	// takeing 15 ms interval 
	int len_each_frame = 0.015*fs   ; // this many data values 
	// overlap in frame is of 0.005 s 
	int len_overlaped_data = 0.010*fs ;  
	float overlaped_data[frame_len] = {0};
	// M is the size of each frame 
	if( len_each_frame >= frame_len){
		printf("%s\n","size limit for M reached" );
		exit(0);
	}
	
	hamming_window( len_each_frame ) ; // this will genrate w_n 
	triangular_filter();

	// creating file discriptor for reading file data.txt
	FILE *fd_read ; FILE *fd_write;  float data ; 
	fd_write = fopen("mfcc_coef_off3.txt","w");
	fd_read = fopen("off_3_data.txt","r");
	if(fd_read == NULL || fd_write == NULL){
		perror(" file not found");
		exit(0);
	}	

	int num_data_collected = 0 ; float past_data = 0 ;float y = 0 ;
	int itr = 0 , row_len_mfcc = 0; 
	
	fscanf (fd_read, "%f", &data);

	while( !feof(fd_read) ){
		//printf("data %f\n",data );
		// pre-enphasis of data --> y = x[n] - 0.95*x[n-1] , implementing high pass filter 
		y =  data - 0.95*past_data ;
		past_data = data ; 

		// now we have to collect the data until we won't get len_each_frame data val to proces
		each_frame_data[num_data_collected] = y;
		num_data_collected++ ;
		if(num_data_collected > len_each_frame ) {
			int i = 0;
			for (int i = 0; i < len_overlaped_data; i++){
				overlaped_data[i] = each_frame_data[(len_each_frame - len_overlaped_data+1) +i];
			}
				// sending the frame for processing
			hamming_filter(len_each_frame , &each_frame_data[0]);

			// storing overlaped data to each_frame_data array and setting num_data to len_overlaped_data
			int n = 0;
			for (int n = 0; n < len_overlaped_data; n++){
				each_frame_data[n] = overlaped_data[n];
			}
				num_data_collected = len_overlaped_data ; 

			// calling pdf fucntion 
			psd(&hamming_filt_data[0]);  
			for (int m = 0; m <frame_len; ++m){
				hamming_filt_data[m] = 0 ; 
				filt_data_im[m] = 0;
			}
			// calling mfcc function for creating cepstral coefficient
			mffc_coef();  // value will be stored in cepstral[] , 40  
			row_len_mfcc++ ; 
			//exit(0);

			for (int itr = 0; itr < (40-1); itr++){
				fprintf(fd_write, "%f", cepstral[itr] );
				//printf("%f\n", cepstral[itr] );
			}
			fprintf(fd_write, "%f\n", cepstral[40-1] );
		
		}     
		
		fscanf (fd_read, "%f", &data);
	
	}
	fseek(fd_write, 0, SEEK_SET);
	fprintf(fd_write, "%i\n", row_len_mfcc );
	printf("num of rows in collected mfcc data :%i\n", row_len_mfcc);
	fclose(fd_read);
	fclose(fd_write);
	return 0;
}