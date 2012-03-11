clear;
close all
clc;


fid1 = fopen('IBMbidHist.csv');
%bidSym = textscan(fid1, 'Ticker is %s');
bid = textscan(fid1, 'Time: %d, Price: %.2f, Count: %d,');

fid2 = fopen('IBMofrHist.csv');
%ofrSym = textscan(fid2, 'Ticker is %s');
ofr = textscan(fid2, 'Time: %d, Price: %.2f, Count: %d,');


bidTimeCurr = bid{1}();
bidTimePrev = bid{1}();

i = 1;
iStart = 1;
timeStep = 1;
iStart = 1;
bidData = zeros(size(bid{1}), 4); % Store 4 elements per time step
ofrData = zeros(size(ofr{1}), 4); %
n = size(bid{1},1);

while i < n
    
    while bid{1}(iStart)==bid{1}(i)
       i = i + 1;
       if i > n
           break;
       end
    end
    iEnd = i - 1; % 6
    
    % Bid Data
    bidPriceVect = bid{2}(iStart:iEnd);
    bidFreqVect = bid{3}(iStart:iEnd);
    bidVect = rude(double(bidFreqVect), bidPriceVect); % Run length coding
    
    m_bid = mean(bidVect);
    v_bid = var(bidVect);
    k_bid = kurtosis(bidVect); % Centered
    y_bid = skewness(bidVect);

    bidData(timeStep,1) = m_bid;
    bidData(timeStep,2) = v_bid;
    bidData(timeStep,3) = k_bid;
    bidData(timeStep,4) = y_bid;

    
    % Ofr Data
    ofrPriceVect = ofr{2}(iStart:iEnd);
    ofrFreqVect = ofr{3}(iStart:iEnd);
    ofrVect = rude(double(ofrFreqVect), ofrPriceVect); % Run length coding
    
    m_ofr = mean(ofrVect);
    v_ofr = var(ofrVect);
    k_ofr = kurtosis(ofrVect); % Centered
    y_ofr = skewness(ofrVect);

    ofrData(timeStep,1) = m_ofr;
    ofrData(timeStep,2) = v_ofr;
    ofrData(timeStep,3) = k_ofr;
    ofrData(timeStep,4) = y_ofr;
    
    iStart = iEnd + 1;
    timeStep = timeStep + 1;
    
end 


% bidIdx : Index of higest freq element
% [maxBidFreq, bidIdx] = max(bid{2}); % Find max freq
% [maxOfrFreq, ofrIdx] = max(ofr{2});


% lBndBid = 0.95*bid{1}(bidIdx);
% uBndBid = 1.05*bid{1}(bidIdx);
% lBndOfr = 0.95*ofr{1}(ofrIdx);
% uBndOfr = 1.05*ofr{1}(ofrIdx);

% bidIdx = find(bid{1}>lBndBid & bid{1}<uBndBid);
% ofrIdx = find(bid{1}>lBndOfr & bid{1}<uBndOfr);

%figure
%plot(bid{1}(bidIdx),bid{2}(bidIdx), 'r.', ofr{1}(ofrIdx), ofr{2}(ofrIdx),'b.');
%title('Bid volume against bid price');
%legend('bid','ask');

fclose(fid1);
fclose(fid2);