%%  dft X(n*2*pi/N) = sum from {0} to {n} {exp(-j*(2*pi/N)*n)}*x[n] 
%% input matlab an array 
 x = input('Provide matlab array format x[n] :' );
%% taking samling in frequecy domain = sampling in time domain = N
N = length( x ) ; 
n = 0:(N-1);
%%difinig symbolic k 
k = sym('k') ;
%% using matrix multiplication insted of for loop
%X_k = (exp((-j*2*pi*k/N)*n)).*x ;

X_k =(exp((-1j*k*2*pi.*n)./(N)))*transpose(x) ; 
k = 0:(N-1) ;
%%
fprintf('number of comlex multiplicatin: %d  and addition : %d' ,N^2,N^2);
%% ploting magnitude and phase spectrum 

subplot(1,2,1);
stem( k , eval(abs(X_k)),'filled' ) , grid on  ;
title("magnitude spectrum");

subplot(1,2,2);
stem( k , eval(angle(X_k)) , 'filled') , grid on  ; 
title("phase spectrum");
xlabel("index");
ylabel("Phase angel in radians");



