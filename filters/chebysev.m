%% butterworth specification cutoff frequency and order 
[b,a] = butter(n,wn);
%[b,a] = butter(n,wn,'high');

%% chebyshev filters 
%[b,a]=cheby1(n,rp,wp);
%[b,a]=cheby1(n,rp,wp,'high');
freqz(b,a);

%% MAKING SIN INPUT ARRAY OF TWO FREQUENCIES

sin_ = sin(2*pi*f1*(0:1/fs:1))+sin(2*pi*f2*(0:1/fs:1));

%%
output = filter(b,a,sin_);
%%
figure(1)
plot(fs*(1:1:numel(sin_))/numel(fft(sin_)),abs(fft(sin_)));
figure(2)
plot(output);
figure(3)
plot(abs(fft(output)));