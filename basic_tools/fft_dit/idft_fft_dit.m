%% taking user input signal into an array
data = input('Provide input signal in format [ 1 5 6 8 ...] \n');

%% making len of data power of 2
data_len = length(data) ;
range_base2 = log2(data_len);
if( ~isempty(data) && isinteger(range_base2) == false  && range_base2 ~=0 ) 
    data = horzcat(data ,zeros(1, 2^ceil(range_base2) -data_len));
   
end

%% bit reversal array
temp_arr = (0:length(data)-1);
rev_bit = bitrevorder(temp_arr);
rev_bit
%log2(length(data))

%% rearanging the data array
re_arr = zeros(1 ,length(rev_bit));
for itr = 1:length(rev_bit)
    re_arr(itr) = data( rev_bit(itr) + 1) ;
end

%%
num_stages = log2(length(re_arr)) ;
disp( 'num of stages')
disp(num_stages);
for i = 1:num_stages 
    %2^i
    for j = 1:2^i:length(rev_bit)
        len_sets = 2^(i-1);
        % for each set iterate
        for k = 1:len_sets 
            %left_idx = rev_bit(j+k-1) +1   ;
            %right_idx = rev_bit(j+len_sets-1+k) +1;
            left_idx = j + k -1;
            right_idx = j+ len_sets - 1+ k ; 
            
            [re_arr(left_idx),re_arr(right_idx)]= my_dft(re_arr(left_idx),re_arr(right_idx),k-1,data_len/(2^(num_stages-i)));
            %disp(data(left_idx));
            %disp(data(right_idx));
        end  
    end
   
end 

re_arr 
%%
data_ = [j 1 3 2j];
ifft(data_)


 %% fucntion for two point dft 
function [x,y] = my_dft(a,b,k ,data_len)
    x = (a) + ((exp((2*pi*j*k)/data_len))*b)/4 ;
    y = (a) - ((exp((2*pi*j*k)/data_len))*b)/4  ; 
end


















