clear;
close all
clc;


fid1 = fopen('AAPLbidHist.csv');
%bidSym = textscan(fid1, 'Ticker is %s');
bid = textscan(fid1, 'Time: %d, Price: %.2f, Count: %d,');

fid2 = fopen('AAPLofrHist.csv');
%ofrSym = textscan(fid2, 'Ticker is %s');
ofr = textscan(fid2, 'Time: %d, Price: %.2f, Count: %d,');

fid3 = fopen('AAPLtotalHist.csv');
%ofrSym = textscan(fid2, 'Ticker is %s');
total = textscan(fid3, 'Time: %d, Price: %.2f, Count: %d,');

fid4 = fopen('AAPLdata.csv','w');

i = 1;
iStart = 1;
iTimeStep = 1;
totalData = zeros(size(total{1},1), 5); % Store 4 elements per time step
nTotal = size(total{1},1); % Bid dim
while i < nTotal
    
    while total{1}(iStart)==total{1}(i)
       i = i + 1;
       if i > nTotal
           break;
       end
    end
    iEnd = i - 1; % 6
    
    % Bid Data
    totalPriceVect = total{2}(iStart:iEnd);
    totalFreqVect = total{3}(iStart:iEnd);
    
    % Find index of higest freq element
    [maxFreq, fIdx] = max(totalFreqVect); % Find max freq
    lBnd = 0.95*totalPriceVect(fIdx);
    uBnd = 1.05*totalPriceVect(fIdx);
    idxVect = find(totalPriceVect>lBnd & totalPriceVect<uBnd);
    
    % Find the truncated vector
    totalVect = rude(double(totalFreqVect(idxVect)), totalPriceVect(idxVect)); % Run length coding
    
    totalTime = total{1}(iStart);
    m_total = mean(totalVect);
    v_total = var(totalVect);
    y_total = skewness(totalVect);
    k_total = kurtosis(totalVect); % Centered

    totalData(iTimeStep,1) = totalTime;
    totalData(iTimeStep,2) = m_total;
    totalData(iTimeStep,3) = v_total;
    totalData(iTimeStep,4) = y_total;
    totalData(iTimeStep,5) = k_total;
        
    fprintf(fid4,'%d, %.2f, %.2f, %.2f, %.2f \n',...
        totalTime, m_total, v_total, y_total, k_total);
    
    iStart = iEnd + 1;
    iTimeStep = iTimeStep + 1;
    
end 



j = 1;
jStart = 1;
jTimeStep = 1;
ofrData = zeros(size(ofr{1}), 5); %
nOfr = size(ofr{1},1); % Ofr dim
while j < nOfr
    
    while ofr{1}(jStart)==ofr{1}(j)
       j = j + 1;
       if j > nOfr
           break;
       end
    end
    jEnd = j - 1; % 6

    % Ofr Data
    ofrPriceVect = ofr{2}(jStart:jEnd);
    ofrFreqVect = ofr{3}(jStart:jEnd);
    ofrVect = rude(double(ofrFreqVect), ofrPriceVect); % Run length coding
    
    ofrTime = ofr{1}(jStart);
    m_ofr = mean(ofrVect);
    v_ofr = var(ofrVect);
    k_ofr = kurtosis(ofrVect); % Centered
    y_ofr = skewness(ofrVect);

    ofrData(jTimeStep,1) = ofrTime;
    ofrData(jTimeStep,2) = m_ofr;
    ofrData(jTimeStep,3) = v_ofr;
    ofrData(jTimeStep,4) = k_ofr;
    ofrData(jTimeStep,5) = y_ofr;    
    
    jStart = jEnd + 1;
    jTimeStep = jTimeStep + 1;
    
end 


% for i = 1:size(ofr{1},1)
%    if(bidData(i,1) ~= ofrData(i,1))
%       fprintf('TimeStep at bid/ofrData at %d not match \n', i);
%    end
% end



%figure
%plot(bid{1}(bidIdx),bid{2}(bidIdx), 'r.', ofr{1}(ofrIdx), ofr{2}(ofrIdx),'b.');
%title('Bid volume against bid price');
%legend('bid','ask');

fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);