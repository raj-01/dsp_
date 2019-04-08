%% triangulr filter 
clear;
Fs = 48000; 
highest_freq = Fs/2;
lowest_freq = 1;
% converting into mel scale 
mel_highest = 2595*log10(1+highest_freq/700);
mel_lowest = 2595*log10(1+lowest_freq/700);
%% 40 equal linespace  // mel(2) = mel(1) + 1*(mel_highest-mel_lowest)/39
%mel = linspace(mel_lowest,mel_highest,40); 
num_filters = 40;
mel = [];
mel(1) = mel_lowest ;
for i=2:(num_filters+2)
    mel(i) = mel(i-1) + ((mel_highest-mel_lowest)/(num_filters+1));
end 

%% mel to herz 
mel_to_hz =double(700*((10.^(mel/2595) - 1)));

%%  triangular filter values each value of k i.e 1:1024 
H = [] ;
m=42;
 for k=1:Fs/2
        if( k< mel_to_hz(m-1))
            H(k)=0 ; 
        elseif( mel_to_hz(m-1)<=k && k <=mel_to_hz(m))
            H(k) = (k - mel_to_hz(m-1))/( mel_to_hz(m)-mel_to_hz(m-1));
        elseif ( mel_to_hz(m)<=k  && k<= mel_to_hz(m+1))
            H(k) = (mel_to_hz(m+1)-k)/(mel_to_hz(m+1)-mel_to_hz(m)); 
        else
            H(k) = 0;
        end 
end



%% 
plot(H);  
