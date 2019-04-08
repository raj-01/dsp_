%% collect data from on / of files 
clear;
[y,Fs] = audioread("off_2.m4a");
data = y(: ,1); 
%%
fbank_data = fopen('off_2_data.txt','w');
fprintf(fbank_data,'%f\n' ,data );
fclose(fbank_data);