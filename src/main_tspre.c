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
	tfile tsfi, tsfo;
	tserie *ts1;
	ctserie *ts2;
	int n;

	/* Assigning ext arg to variables */
	if( argc != 5 ) {
	  printf("argc = %d\n", argc);
      printf("Bad number of arguments supplied.\n");
	  return -1;
	}
	tsfi.dirname = argv[1]; /*"/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/";*/
	tsfi.filename = argv[2]; /*"01BJ001_DLY_FLOWS.csv";*/
	tsfo.dirname = argv[3]; /*"/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/";*/
	tsfo.filename = argv[4]; /*"01BJ001_DLY_FLOWS.csv";*/

	/* Reading time series files */ 
	ts1 = readTSfromFile(&tsfi);

	/* Pre-treatment of time series */
	ts2 = preTreatTS(ts1, &n);

	/* Write pre-treated time series */
	if (writeTserie2csv(&tsfo, ts2, n))
		return -1;
    /*nameout = strtok(tsf.filename, ".");	
	while( nameout != NULL ) {
      printf( " %s\n", nameout); 
      nameout = strtok(NULL, " ");
	}*/

	/* Free heap mem */
	freeMemTs(ts1);
	free(ts2);
	return 0;
}

