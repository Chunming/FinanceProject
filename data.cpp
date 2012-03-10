#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
// 4 moments per second
// Compute matrix


int main () {
   FILE *inFile;
   inFile = fopen("AAPL", "r"); // Open file for read
   if (NULL == inFile) printf("ERROR opening file \n");

   FILE *bidFile;
   bidFile = fopen("bidHist.csv", "w"); // Open file for write
   if (NULL == bidFile) printf("ERROR opening file \n");

   FILE *ofrFile;
   ofrFile = fopen("ofrHist.csv", "w"); // Open file for write
   if (NULL == ofrFile) printf("ERROR opening file \n");

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
   memset(bidCount, 0, 1000000*sizeof(int));
   memset(ofrCount, 0, 1000000*sizeof(int));

   int bidIdx;
   int ofrIdx;
   int idx=0;


   fscanf(inFile, "%s %s %s %s %s %s  %s %s %s %s", label[0], label[1], label[2], label[3], label[4], label[5], label[6], label[7], label[8], label[9]);

   while(!feof(inFile)) {
	 
         // for (int i=0; i<10; ++i) printf("The label is %s \n", label[i]);

         fscanf(inFile, "%s %d %s %f %f %d %d %d %c", symbol, &date, time, &bid, &ofr, &bidSize, &ofrSize, &mode, &ex);
         printf("%s %d %s %f %f %d %d %d %c \n", symbol, date, time, bid, ofr, bidSize, ofrSize, mode, ex);


	int hr = atoi(&time[0]); // Runs from 00 hrs to 24hrs
	int min = atoi(&time[2]); // 
	int sec = atoi(&time[5]);
        int timeCurr = 10000*hr + 100*min + sec; // Current time 
	int timePrev;
	if (idx == 0) {
           timePrev = 10000*hr + 100*min + sec; // Current time 
	}

        printf("The number is %d \n", timeCurr);	 

	
	if (timeCurr == timePrev) {
            bidIdx = bid*100; // The hash function
            bidCount[bidIdx]+= bidSize;
	    ofrIdx = ofr*100;
            ofrCount[ofrIdx]+= ofrSize;

	    
	}

	if (timeCurr != timePrev || feof(inFile) ) { // timeCurr is incremented 

           fprintf(bidFile, "Time Stamp is %d \n", timePrev);
           fprintf(ofrFile, "Time Stamp is %d \n", timePrev);

	   timePrev = timeCurr;

           for (int i=0; i<1000000; ++i) {
      	      if (0 != bidCount[i])  {
                 fprintf(bidFile, "%.2f, %d, \n", (float)i/100, bidCount[i]);
      	      }
      	      if (0 != ofrCount[i])  {
                 fprintf(ofrFile, "%.2f, %d, \n", (float)i/100, ofrCount[i]);
      	      }
   	   }

	   // Reset all the bins
   	   memset(bidCount, 0, 1000000*sizeof(int));
   	   memset(ofrCount, 0, 1000000*sizeof(int));

            bidIdx = bid*100; // The hash function
            bidCount[bidIdx]+= bidSize;
	    ofrIdx = ofr*100;
            ofrCount[ofrIdx]+= ofrSize;


	}







	idx++;
   }







   fclose(ofrFile);
   fclose(inFile);
   fclose(bidFile);
}
