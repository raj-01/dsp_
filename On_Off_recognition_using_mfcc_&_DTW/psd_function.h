#include <math.h>
#define pi 3.14
#define fs 48000 
#define frame_len 1024
#define log_in_len 10  // we are computing 1024 point dft
extern float haming_w_n[frame_len];
extern float hamming_filt_data[frame_len] ;
extern float each_frame_data[frame_len] ;
extern float filt_data_im[frame_len] ; // frame len is 1024
extern float psd_data[frame_len] ;
int  rev_bit( int n  ){
		int store_rev_bits = 0 ;
		int i ;
		for(i = 0 ; i < log_in_len ; i++){
			store_rev_bits <<= 1 ; 
			if( (int)(n & 1) == 1) {store_rev_bits ^= 1;};
			n >>= 1 ;  
		} 

		return store_rev_bits ;
}

float * two_point_dft(float a_re,float a_im ,float b_re ,float b_im , int k , int N){
		float static store_dft_value[4] ;
		float cos_theta = cos( (-2*pi*k)/N );
		float sin_theta = sin( (-2*pi*k)/N );
		
		store_dft_value[0] =  a_re + b_re*cos_theta -b_im*sin_theta;
		store_dft_value[1] =  a_im + b_re*sin_theta + b_im*cos_theta;    
		store_dft_value[2] = a_re - b_re*cos_theta + b_im*sin_theta;
		store_dft_value[3] =  a_im - b_re*sin_theta - b_im*cos_theta;  

		return store_dft_value ; 
}

void psd(float *filt_data_re){
		int num_stages = log_in_len ;   // 10 stages for 1024 data
		float * value ; 
		
		int i , j , k,len_sets , left_idx , right_idx;
		for( i =1 ; i <= num_stages; i++ ){             // iterate horizontally 
			for( j= 0 ; j< frame_len ;j += pow(2,i) ){     // iterate vertically 
				len_sets = pow(2 , i-1 );
				for( k =0; k < len_sets; k++ ){		//iterating in set 
					left_idx = j + k  ;
					right_idx = j + len_sets + k ; 
					//printf( "%f\n" ,x_re[rev_bit(1)]);
					value = two_point_dft( filt_data_re[rev_bit(left_idx)] , filt_data_im[rev_bit(left_idx)], filt_data_re[rev_bit(right_idx)] , filt_data_im[rev_bit(right_idx)] , k , frame_len/(pow(2,(num_stages-i))));
					filt_data_re[rev_bit(left_idx)] =  *( value ); 
					filt_data_im[rev_bit(left_idx)] = *( value +1);
					filt_data_re[rev_bit(right_idx)] =*( value + 2);
					filt_data_im[rev_bit(right_idx)] =*( value + 3);
				}
				
			} 
		}

		for (int i = 0; i <1024; i++)
		{
			psd_data[i] =  filt_data_re[rev_bit(i)]*filt_data_re[rev_bit(i)] + filt_data_im[rev_bit(i)]*filt_data_im[rev_bit(i)] ;		
		}
}
