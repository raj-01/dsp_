% wp=input('Pass band:');
% ws=input('Stop band:');
fs = 500;
f2 = 100;
f1 = 30;

wp = (pi*2*f1)/fs ;
ws = (pi*2*(f1+30))/fs
%ws = 500;
%sin_ = sin(2*pi*f1*(0:1/fs:1))+sin(2*pi*f2*(0:1/fs:1));
sin_ = [ 1, 2, 2, 4, 5, 6, 8, 4, 5, 6]; 
%%
wc=(wp+ws)/2;
%% Rectangular
 ml=0.92*pi/(ws-wp);
 M=ceil(ml)+1;
 for n=1:M
     w_n(n)=1;
 end
 
 %% Hamming
 ml=3.32*pi/(ws-wp); 
 M=ceil(ml)+1;
 for n=1:M
     w_n(n)=0.54-0.46*cos(2*pi*(n-1)/(M-1));
 end
 
 %% Hanning
 ml=3.11*pi/(ws-wp);
 M=ceil(ml)+1;
 for n=1:1:M
     w_n=0.5-0.5*cos(2*pi*(n-1)/(M-1));
 end
 %%
 hd_lowpass=zeros(1,M);
 h_n_lowpass=zeros(1,M);
 hd_highpass=zeros(1,M);
 h_n_highpass=zeros(1,M);
 for n=1:1:M
     hd_lowpass(n)=(wc/pi)*sinc((wc/pi)*(n-((M-1)/2)));
%      hd_highpass(n)=sinc(n-1)-hd_lowpass(n);
     h_n_lowpass(n)=hd_lowpass(n)*w_n(n);
%      h_n_highpass(n)=hd_highpass(n)*w_n(n);
 end
%%
%plot(abs(fft(h_n_lowpass)));
out_low=conv(sin_,h_n_lowpass);
% out_high=conv(sin_,h_n_highpass);
%h_n_lowpass 
out_low
% figure(1)
% plot(abs(fft(sin_)));
% figure(2)
% plot(abs(fft(out_low)));
% % plot(abs(fft(out_high)));

