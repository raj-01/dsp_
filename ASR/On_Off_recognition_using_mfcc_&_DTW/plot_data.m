clear;
fbank_data = fopen('fbank.txt','r');
fbank_data = fscanf(fbank_data,'%f');
%%
k = 1 ;
for i=1:40
    for j=1:512
        H(i,j) = fbank_data(k);
        k = k + 1 ; 
    end
end

%%
for i=1:40
    hold on
    plot(H(i,:))
end