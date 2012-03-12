#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
// 4 moments per second
// Compute matrix


int main () {
   FILE *quoteFile;
   quoteFile = fopen("aaplquotestraining.txt", "r"); // Open file for read
   if (NULL == quoteFile) printf("ERROR opening file \n");

   FILE *tradeFile;
   tradeFile = fopen("aaplquotestraining.txt", "r"); // Open file for read
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


   // Allocate memory for labels
   char** label = new char* [10];
   for (int i=0; i<10; ++i) {
         label[i] = new char [10];
         memset(label[i], NULL, 10*sizeof(char));
   }

   // Allocate memory for time
   char symbol[5];
   int date;
   char time[8];
   float bid;
   float ofr;
   int bidSize;
   int ofrSize;
   int mode;
   char ex;
  
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
   int idx=0;


   fscanf(quoteFile, "%s %s %s %s %s %s  %s %s %s %s", label[0], label[1], label[2], label[3], label[4], label[5], label[6], label[7], label[8], label[9]);

   while(!feof(quoteFile)) {
	 
         // for (int i=0; i<10; ++i) printf("The label is %s \n", label[i]);

         fscanf(quoteFile, "%s %d %s %f %f %d %d %d %c", symbol, &date, time, &bid, &ofr, &bidSize, &ofrSize, &mode, &ex);
         //printf("%s %d %s %f %f %d %d %d %c \n", symbol, date, time, bid, ofr, bidSize, ofrSize, mode, ex);


	int hr = atoi(&time[0]); // Runs from 00 hrs to 24hrs
	int min = atoi(&time[2]); // 
	int sec = atoi(&time[5]);
        int timeCurr = 10000*hr + 100*min + sec; // Current time 
	int timePrev;
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

   fclose(totalFile);
   fclose(ofrFile);
   fclose(quoteFile);
   fclose(bidFile);
}
