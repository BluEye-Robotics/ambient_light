load 'buiteveld94'
load 'pope97'
load 'smith81'

data = buiteveld94;
data2 = pope97;
data3 = smith81;

%hold on;
%plot(data(:,1), data(:,2))
%plot(data2(:,1), data2(:,2), 'r')
%plot(data3(:,1), data3(:,2), 'g')

doubleD = [data3(1,2)];

for i = 1:length(data3(:,2))-1
    avg = (data3(i,2) + data3(i+1,2)) / 2;
    doubleD = [doubleD; avg; data3(i+1, 2)];
end

%doubleD = [data(1,2)];
%
%for i = 1:length(data(:,2))-1
%    avg = (data(i,2) + data(i+1,2)) / 2;
%    doubleD = [doubleD; avg; data(i+1, 2)];
%end
