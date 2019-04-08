        /*Distance is calculated as: 
     
       Dist[x1][y1] = (X1a - Y1a)^2 + (X1b - Y1b)^2 + ... + (X1n - Y1n)^2, etc. 
     
    */  
      
    /* OUTPUTS: */  
    /* output file: best alignment of the 2 parameter files */  
    /* glob: sum of global distances, useful as a similarity measure */ 

#include <math.h>  
#include <stdio.h>  
#include <stdlib.h>  
      
#define VERY_BIG  (1e30)  
#define num_filters 40 

////////////////////////////////////////////////////////////////////// 
#define num_files_for_matching 1
extern float final_decision_ranks[num_files_for_matching] ;
//////////////////////////////////////////////////////////////////////
extern float mfcc_on_data[300 + 50][40] ; 
//int static row_len_of_input_data =  0 ;   // input computed mfcc vector size (row_len_of_input_data)X(40)
extern float cepstral[num_filters];
float static Dist[300+50][num_filters] = {0};    /* so , 500 is the limit of 25 ms frame */
float static globdist[300+50][300+50] = {0};
extern int row_len_of_ref_mfcc ; 
// size_of_reference_mfcc is the total num of rows with 40 elements in each reference/test files 
void dist_matrix(int nth_row ){
    /*Compute distance matrix*/  
    if( nth_row >=  300+50 ){
        printf("%s\n","In  {dtw.c} dist_matrix() -->Size of real time input reached" );
        exit(1);
    }      
    float total = 0 ; 
   // printf("nth row :%i\n",nth_row );
    //printf("row len of ref. mfcc data :%i\n", row_len_of_ref_mfcc); 
    for(int j=0;j<row_len_of_ref_mfcc;j++) {  
        total = 0;  
        for (int k=0;k<num_filters;k++) {  
          total = total + ((cepstral[k] - mfcc_on_data[j][k]) * (cepstral[k] - mfcc_on_data[j][k]));  
        }       
        Dist[nth_row][j] = total; 
       /* if (j == row_len_of_ref_mfcc-1 ){
             printf("  %f\n", Dist[nth_row][row_len_of_ref_mfcc-1]);
        }
        else printf("  %f", Dist[nth_row][j]); */
         
    }  
    
}

    
void cal_globdist(int file_num , int row_len_of_input_data){

     /*% for first frame, only possible match is at (0,0)*/ 
     double top = 0 , mid = 0, bot = 0, cheapest = 0; 
     int I = 0; 
      
    globdist[0][0] = Dist[0][0];  
    for (int j=1; j<row_len_of_input_data; j++)  
        globdist[j][0] = VERY_BIG;  
      
    globdist[0][1] = VERY_BIG;  
    globdist[1][1] = globdist[0][0] + Dist[1][1];    
      
    for(int j=2;j<row_len_of_input_data;j++)  
        globdist[j][1] = VERY_BIG;  
      
    for(int i=2;i<row_len_of_ref_mfcc;i++) {  
        globdist[0][i] = VERY_BIG;  
        globdist[1][i] = globdist[0][i-1] + Dist[1][i];   
       /* globdist_jMinus1_iMinus2 = globdist[1][0];
        globdist_j_iMinus1 =  Dist[j][i-1];
        globdist_j_i = Dist[j][i];
        globdist_jMinus1_iMinus1 = globdist[1][1];
        globdist_jMinus2_iMinus1 = globdist[0][1];
        globdist_jMinus1_i = globdist[1][2];*/
        for(int j=2;j<row_len_of_input_data;j++) {  
            top = globdist[j-1][i-2] + Dist[j][i-1] + Dist[j][i];  
            mid = globdist[j-1][i-1] + Dist[j][i];  
            bot = globdist[j-2][i-1] + Dist[j-1][i] + Dist[j][i]; 
            if( (top < mid) && (top < bot))  cheapest = top;    
             
            else if (mid < bot)  cheapest = mid;   
        
            else cheapest = bot;     
      
        globdist[j][i] = cheapest;      
          }  
    }    
          
    /* print global distance to globfile*/         
      
    final_decision_ranks[file_num] = globdist[row_len_of_input_data-1][row_len_of_ref_mfcc-1];    

} 


   