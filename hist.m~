
fid1 = fopen('IBMbidHist.csv');
bid = textscan(fid1, 'Time: %d, Price: %.2f, Count: %d,');

fid2 = fopen('IBMofrHist.csv');
ofr = textscan(fid2, 'Time: %d, Price: %.2f, Count: %d, ');

% bidIdx : Index of higest freq element
[maxBidFreq, bidIdx] = max(bid{2}); % Find max freq
[maxOfrFreq, ofrIdx] = max(ofr{2});

lBndBid = 0.95*bid{1}(bidIdx);
uBndBid = 1.05*bid{1}(bidIdx);
lBndOfr = 0.95*ofr{1}(ofrIdx);
uBndOfr = 1.05*ofr{1}(ofrIdx);

bidIdx = find(bid{1}>lBndBid & bid{1}<uBndBid);
ofrIdx = find(bid{1}>lBndOfr & bid{1}<uBndOfr);

figure
plot(bid{1}(bidIdx),bid{2}(bidIdx), 'r.', ofr{1}(ofrIdx), ofr{2}(ofrIdx),'b.');
title('Bid volume against bid price');
legend('bid','ask');

fclose(fid1);
fclose(fid2);