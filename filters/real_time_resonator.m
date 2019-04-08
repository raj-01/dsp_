%% 
f0=20 ;% input('Input the resonant frequency in Hz:');
fs = 100 ;
f1 = 10 ;
f2 = 20 ; 
w0=(2*pi*f0)/fs;
r=0.95;
b=(1-r)*sqrt(1+r^2-(2*r*cos(2*w0)));
a=[1 -2*r*cos(w0) r^2];

%% multitone data input 
sin_ = sin(2*pi*f1*(0:1/fs:1))+sin(2*pi*f2*(0:1/fs:1));


%% matlab functions for filter 
out1=filter(b,a,sin_);
plot( abs(fft(out1)));
%fvtool(b,a);
%[num,den]=iirpeak(w0,500/(fs/2));
%out2=filter(num,den,sin_);
%% 
 M = length(b) ; % denotes input coff length 
 N = length(a) ; % denotes past output coff length  

buffer_pastValues =  zeros(1, N ) ;
buffer_input = zeros( 1, M ) ; 
 
 %% real time input 
 data = [] ; 
 for idx = 1:length(y) 
      for i_x = M:-1:2 
            buffer_input(i_x) = buffer_input(i_x-1);
        end 
        buffer_input(1) = y(idx) ;
     value = myfilter(a,b , buffer_pastValues,buffer_input );
     for i_x = N:-1:3 
            buffer_pastValues(i_x) = buffer_pastValues(i_x-1);
     end 
     buffer_pastValues(2) = value ;
     %display(value ) ; 
     %stem(idx, value);
     data(idx) = value ; 
     %pause(0.05);
     %hold on ;
 end 
     
 %% function for real time filter  
 
   function output = myfilter(a , b , buffer_pastValues,buffer_input)
        N = length( a ) ;
        M = length( b ) ; 
        value = 0 ; 
%         for i_x = M:-1:2 
%             buffer_input(i_x) = buffer_input(i_x-1);
%         end 
%         buffer_input(1) = input ; 
        
        for i = 1:M 
            % b is coff of x 
            value = value + b(i)*buffer_input(i); 
        end 
        
        for j = 1:N 
            % a is codd of y 
            value = value - a(j)*buffer_pastValues(j) ;
        end 
     
        output = value ; 
            
end               
        
        