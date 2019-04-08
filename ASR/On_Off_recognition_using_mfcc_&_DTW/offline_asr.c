#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<string.h>
#include "psd_function.h"
#include "mfcc_filter_bank.c"
#include "dtw.c"
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
float mfcc_on_data[300 +50][40] ={0};
int row_len_of_ref_mfcc = 0 ; 

////////////////////////////////////////////////////////////////////// 
#define num_files_for_matching 1
float final_decision_ranks[num_files_for_matching] = {0} ;
//////////////////////////////////////////////////////////////////////

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
	// storing the test/reference data into an array for comparision
	FILE *read_mfcc ;
	float mfcc_data = 0 ;
	read_mfcc = fopen("mfcc_coef_off3.txt", "r");
	if (read_mfcc==NULL){
		perror("file mfcc_coef_off3.txt not found");
		exit(1);
	}
	
	fscanf(read_mfcc , "%f",&mfcc_data);
	row_len_of_ref_mfcc = (int)mfcc_data ; 
	int mfcc_row_itr = 0 ;
	int mfcc_column_itr = 0 ;
	for (int mfcc_row_itr = 0; mfcc_row_itr < row_len_of_ref_mfcc; mfcc_row_itr++){
		for (int mfcc_column_itr = 0; mfcc_column_itr <40 ; mfcc_column_itr++){
			fscanf(read_mfcc , "%f",&mfcc_data);
			mfcc_on_data[mfcc_row_itr][mfcc_column_itr] =  mfcc_data ; 
		}
	}

	// creating file discriptor for reading file data.txt
	FILE *fd_read ; FILE *fd_write;  float data ; 
	fd_write = fopen("psd_data.txt","w");
	fd_read = fopen("on_1_data.txt","r");
	if(fd_read == NULL || fd_write == NULL){
		perror(" on_1_data.txt - file not found");
		exit(1);
	}	

	int num_data_collected = 0 ; float past_data = 0 ;float y = 0 , nth_row = 0; 
	//fscanf (fd_read, "%f", &data);
	//row_len_of_ref_mfcc = data ; // because i stored the first element in mfcc data as its row length

	while( !feof(fd_read) ){
		fscanf (fd_read, "%f", &data);
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
			for (int m = 0; m <frame_len; m++){
				hamming_filt_data[m] = 0 ; 
				filt_data_im[m] = 0;
			}
			// calling mfcc function for creating cepstral coefficient
			mffc_coef();  // value will be stored in cepstral[] , 40  
			// dist_matrix(int nth_row , int row_len_of_ref_mfcc)
			dist_matrix(nth_row ) ;   /*computed 40 dist is stored in Dist[300][num_filters] defined in dtw.c*/
			nth_row++ ;     /* because we want to know how many rows got created in real time input */
			//exit(0);
			/*int k = 0 ; 
			for (int k = 0; k <40 ; k++){	
				//fprintf(fd_write, "%f\n", psd_data[k]);
				//printf("dist data :%f\n", dist_matrix[nth_row-1][k]);
			}*/
		
		}     
		//fscanf (fd_read, "%f", &data);	
	}
	int file_num = 1; 
	cal_globdist(file_num ,nth_row);
	printf("%f\n",final_decision_ranks[file_num] );
	fclose(fd_read);
	fclose(fd_write);
	return 0;
}