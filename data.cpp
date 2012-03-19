#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
// 4 moments per second
// Compute matrix


int main () {
   FILE *quoteFile;
   quoteFile = fopen("input/cvsquotestraining.txt", "r"); // Open file for read
   if (NULL == quoteFile) printf("ERROR opening file \n");

   FILE *tradeFile;
   tradeFile = fopen("input/cvstradestraining.txt", "r"); // Open file for read
   if (NULL == tradeFile) printf("ERROR opening file \n");

   FILE *bidFile;
   bidFile = fopen("AAPLbidHist.csv", "w"); // Open file for write
   if (NULL == bidFile) printf("ERROR opening file \n");

   FILE *ofrFile;
   ofrFile = fopen("AAPLofrHist.csv", "w"); // Open file for write
   if (NULL == ofrFile) printf("ERROR opening file \n");

   FILE *totalFile;
   totalFile = fopen("AAPLtotalHist.csv", "w"); // Open file for write
   if (NULL == totalFile) printf("ERROR opening file \n");

   FILE *priceFile;
   priceFile = fopen("AAPLpriceHist.csv", "w"); // Open file for write
   if (NULL == priceFile) printf("ERROR opening file \n");


   // =====================================================
   // Scan Bid/Ask Quote Data
   // =====================================================

   // Allocate memory for labels
   char** label = new char* [10];
   for (int i=0; i<10; ++i) {
         label[i] = new char [10];
         memset(label[i], NULL, 10*sizeof(char));
   }

   // Allocate memory for time
   char symbol[5];
   int date;
   char time[10];
   float bid;
   float ofr;
   int bidSize;
   int ofrSize;
   int mode;
   char ex;
   unsigned int idx = 0;


  
   int* bidCount = new int [1000000];
   if (NULL == bidCount) {
      printf("ERROR alloc \n");
      return -1;
   }
   int* ofrCount = new int [1000000];
   if (NULL == ofrCount) {
      printf("ERROR alloc \n");
      return -1;
   }
   int* totalCount = new int [1000000];
   if (NULL == totalCount) {
      printf("ERROR alloc \n");
      return -1;
   }
   memset(bidCount, 0, 1000000*sizeof(int));
   memset(ofrCount, 0, 1000000*sizeof(int));
   memset(totalCount, 0, 1000000*sizeof(int));
   
   int bidIdx;
   int ofrIdx;


   fscanf(quoteFile, "%s %s %s %s %s %s  %s %s %s %s", label[0], label[1], label[2], label[3], label[4], label[5], label[6], label[7], label[8], label[9]);

   while (idx < 500000) { //while(!feof(quoteFile))

         fscanf(quoteFile, "%s %d %s %f %f %d %d %d %c", symbol, &date, time, &bid, &ofr, &bidSize, &ofrSize, &mode, &ex);
         //printf("%s %d %s %f %f %d %d %d %c \n", symbol, date, time, bid, ofr, bidSize, ofrSize, mode, ex);

	int hr, min, sec, timeCurr, timePrev; // Time length can be 7 or 8
	if (strlen(time)==7) {
	   hr = atoi(&time[0]); // Runs from 00 hrs to 24hrs
           min = atoi(&time[2]); // 
	   sec = atoi(&time[5]);
           timeCurr = 10000*hr + 100*min + sec; // Current time 
	}
	else { // strlen == 8
	   hr = atoi(&time[0]);
           min = atoi(&time[3]); 
	   sec = atoi(&time[6]);
           timeCurr = 10000*hr + 100*min + sec;
        }
	if (idx == 0) {timePrev = 10000*hr + 100*min + sec;}

	if (timeCurr == timePrev) {
            bidIdx = bid*100; // The hash function
            bidCount[bidIdx]+= bidSize;
	    totalCount[bidIdx]+= bidSize; 

	    ofrIdx = ofr*100;
            ofrCount[ofrIdx]+= ofrSize;
	    totalCount[ofrIdx]+= ofrSize;	     
	}

	if (timeCurr != timePrev || feof(quoteFile) ) { // timeCurr is incremented 

           for (int i=0; i<1000000; ++i) {
      	      if (0 != bidCount[i])  {
                 fprintf(bidFile, "Time: %d, Price: %.2f, Count:  %d, \n", timePrev, (float)i/100, bidCount[i]);
      	      }
      	      if (0 != ofrCount[i])  {
                 fprintf(ofrFile, "Time: %d, Price: %.2f, Count:  %d, \n", timePrev, (float)i/100, ofrCount[i]);
      	      }
      	      if (0 != totalCount[i])  {
                 fprintf(totalFile, "Time: %d, Price: %.2f, Count:  %d, \n", timePrev, (float)i/100, totalCount[i]);
      	      }
   	   }

	   timePrev = timeCurr;

	   // Reset all the bins
   	   memset(bidCount, 0, 1000000*sizeof(int));
   	   memset(ofrCount, 0, 1000000*sizeof(int));
   	   memset(totalCount, 0, 1000000*sizeof(int));

            bidIdx = bid*100; // The hash function
            bidCount[bidIdx]+= bidSize;
	    totalCount[bidIdx] += bidSize;
	
	    ofrIdx = ofr*100;
            ofrCount[ofrIdx]+= ofrSize;
	    totalCount[bidIdx] += ofrSize;
	}
	idx++;
   }


   // =====================================================
   // Scan Trade Price Data
   // =====================================================
   
   // Allocate memory for labels
   char** tLabel = new char* [9];
   for (int i=0; i<9; ++i) {
         tLabel[i] = new char [10];
         memset(tLabel[i], NULL, 10*sizeof(char));
   }
   
   float price;
   int g127;
   int corr;
   char cond[3];
   int size;
   // char ex
   // int date
   // char* time

   float cumSum = 0;
   int cumWeight = 0;
   float avePrice;
   fscanf(tradeFile, "%s %s %s %s %s %s  %s %s %s", tLabel[0], tLabel[1], tLabel[2], tLabel[3], tLabel[4], tLabel[5], tLabel[6], tLabel[7], tLabel[8]);

   idx = 0;
   while(!feof(tradeFile)) {
         fscanf(tradeFile, "%s %d %s %f %d %d %s %c  %d", symbol, &date, time, &price, &g127, &corr, cond, &ex, &size);

	int hr, min, sec, timeCurr, timePrev; // Time length can be 7 or 8
	if (strlen(time)==7) {
	   hr = atoi(&time[0]); // Runs from 00 hrs to 24hrs
           min = atoi(&time[2]); // 
	   sec = atoi(&time[5]);
           timeCurr = 10000*hr + 100*min + sec; // Current time 
	}
	else { // strlen == 8
	   hr = atoi(&time[0]);
           min = atoi(&time[3]); 
	   sec = atoi(&time[6]);
           timeCurr = 10000*hr + 100*min + sec;
        }

	// Initialize timePrev
	if (idx == 0) {timePrev = 10000*hr + 100*min + sec;}


	if (timeCurr == timePrev) {
	   cumSum = cumSum + price*size;
	   cumWeight = cumWeight + size;
	   timePrev = timeCurr;
	}

	if (timeCurr != timePrev || feof(tradeFile)) { // timeCurr is incremented 

	   avePrice = cumSum/cumWeight;
           fprintf(priceFile, "Time: %d, Price: %.2f, \n", timePrev, avePrice);
	   timePrev = timeCurr;
	   cumSum = 0; // Reset all the bins
	   cumWeight = 0;
	   cumSum = cumSum + price*size;
	   cumWeight = cumWeight + size;
	}
	
	idx++;
   }

   fclose(priceFile);
   fclose(totalFile);
   fclose(ofrFile);
   fclose(quoteFile);
   fclose(tradeFile);
   fclose(bidFile);
}
