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
	unsigned int i;
	tfile tsf, regif;
	tserie *ts1;
	ctserie *ts2;
	regi *dre;
	int nnans, n;
	int nyday;
	float *x;

	/* Assigning ext arg to variables */
	if( argc != 3 ) {
      printf("Bad number of arguments supplied.\n");
	  return -1;
	}
	tsf.dirname = argv[1]; /*"/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/";*/
	tsf.filename = argv[2]; /*"01BJ001_DLY_FLOWS.csv";*/

	/* Reading time series files */ 
	ts1 = readTSfromFile(&tsf);

	/* Pre-treatment of time series */
	ts2 = preTreatTS(ts1, &n);
	/*if (preTreatTS(ts1, &ts2))
		return -1;*/
	
	/*printf("ts2.n %d\n",ts2.n);*/
	x = malloc(n * sizeof(float));
	for(i = 0; i < n; i++) x[i] = ts2[i].var;
	nnans = countNaNs(x, n);
	free(x);
	if(nnans>0){
		printf("file: %s, NANs=%d\n",tsf.filename, nnans);
		exit(0);
	}
	

	/* Get annual daily regime */ 
	/*regi *dre = NULL;*/
	/*regi *dre = malloc(366 * sizeof(regi));*/
	/*dailyRegime(&ts2, dre);*/
	/*dre = dailyRegime(ts2, &nydays);*/
	dre = dailyRegime(ts2, n, &nyday);
	/*for (i = 0; i < nyday; i++){
		printf("%d, mean = %f\n", dre[i].x, dre[i].mean);
	}*/
	regif.dirname = argv[1];
	regif.filename = "dre.csv";
	if (nyday == 367 || nyday == 364){
		printf("-------------------------------------------nydays = %d\n",nyday);
	}
	writeRegi2csv(&regif, dre, &nyday);

	free(dre);
	freeMemTs(ts1);
	free(ts2);
	return 0;
}

