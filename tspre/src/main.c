#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aggts.h"
#include "stats.h"
#include "tspre.h"
#include "utils.h"

int main(int argc, char *argv[])
{

	tfile tsf, regif;
	tserie *ts1;
	tserie *ts2;
	regi *dre;
	int nnans;
	int nydays;

	/* Assigning ext arg to variables */
	if( argc != 3 ) {
      printf("Bad number of arguments supplied.\n");
	  return -1;
	}
	tsf.dirname = argv[1]; /*"/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/";*/
	tsf.filename = argv[2]; /*"01BJ001_DLY_FLOWS.csv";*/

	/* Reading time series files */ 
	/*readTSfromFile(&tsf, &ts1);*/
	ts1 = readTSfromFile(&tsf);

	/* Pre-treatment of time series */
	ts2 = preTreatTS(ts1);
	/*if (preTreatTS(ts1, &ts2))
		return -1;*/
	
	/*printf("ts2.n %d\n",ts2.n);*/
	/*nnans = countNaNs(ts2.var, ts2.n);
	if(nnans>0)
		printf("NANs=%d\n", nnans);*/
	
	/* Get annual daily regime */ 
	/*regi *dre = NULL;*/
	/*regi *dre = (regi *)malloc(366 * sizeof(regi));*/
	/*dailyRegime(&ts2, dre);*/
	/*dre = dailyRegime(&ts2, &nydays);*/
	/*for (int i = 0; i < nydays; i++){
	//	printf("%d, mean = %f\n", dre[i].x, dre[i].mean);
	}*/
	/*regif.dirname = argv[1];*/
	/*regif.filename = "dre.csv";
	if (nydays == 367 || nydays == 364){
		printf("-------------------------------------------nydays = %d\n",nydays);
	}
	writeRegi2csv(&regif, dre, &nydays);
	free(dre);*/
	
	freeMemTs(ts1);
	freeMemTs(ts2);
	return 0;
}

