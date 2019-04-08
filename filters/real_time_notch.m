f0=input('Input the resonant frequency in Hz:');
w0=(2*pi*f0)/(fs/2);
r=0.95;
b0=(1-r)*sqrt(1+r^2-(2*r*cos(2*w0)));
b=[b0 -2*b0*cos(w0) b0];
a=[1 -2*r*cos(w0) r^2];
sin_ = sin(2*pi*f1*(0:1/fs:1))+sin(2*pi*f2*(0:1/fs:1));
out1=filter(b,a,sin_);
fvtool(b,a);

[num,den]=iirnotch(w0,500/(fs/2));
out2=filter(num,den,sin_);