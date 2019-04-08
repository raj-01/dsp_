#include<stdio.h>
#include<math.h>
#define in_len 16 
#define log_in_len 4 
#define pi 3.14 

// takking array as unput 
float static x_re[in_len] = {1, 2, 3, 4, 5, 6, 7, 8 , 1, 2, 3, 4, 5, 6, 7, 8} ; 
float static x_im[in_len] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


//float static x_re[in_len] = { 1,2};
//float static x_im[in_len] = {0,0};
// bit reversal , we have to reverse 0 to (in_len-1)
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


int  main(){
		//printf( "%f\n" , sin( (-2*pi*1)/in_len ) ) ;
		//printf("%d\n", sizeof(x)/sizeof(double) );
		//printf("%d\n", rev_bit(6) );
		// numer of stages is log_in_len 
		int num_stages = log_in_len ;
		float * value ; 
		 
		int i , j , k,len_sets , left_idx , right_idx;
		for( i =1 ; i <= num_stages; i++ ){             // iterate horizontally 
			for( j= 0 ; j< in_len ;j += pow(2,i) ){     // iterate vertically 
				len_sets = pow(2 , i-1 );
				for( k =0; k < len_sets; k++ ){		//iterating in set 
					left_idx = j + k  ;
					right_idx = j + len_sets + k ; 
					//printf( "%f\n" ,x_re[rev_bit(1)]);
					value = two_point_dft( x_re[rev_bit(left_idx)] , x_im[rev_bit(left_idx)], x_re[rev_bit(right_idx)] , x_im[rev_bit(right_idx)] , k , in_len/(pow(2,(num_stages-i))));
					x_re[rev_bit(left_idx)] =  *( value ); 
					x_im[rev_bit(left_idx)] = *( value +1);
					x_re[rev_bit(right_idx)] =*( value + 2);
					x_im[rev_bit(right_idx)] =*( value + 3);
				}
				
			} 
		}
		int itr ;
		for(itr = 0 ; itr < in_len ; itr++ ){
		printf("fourier transform %f %s %f %s\n" , x_re[rev_bit(itr)] , "  +   ",x_im[rev_bit(itr)],"j");
	}
	//printf( "%f\n" , sin( (-2*pi*1)/in_len ) ) ; 
	return 0 ; 
}
