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
	ctserie *ts;
	agg *tsm;
	yrsum *tsy;
	regi *dre, *mre;
	int n, nm, ny, nred;

	/* Assigning ext arg to variables */
	if( argc != 6 ) {
	  printf("argc = %d\n", argc);
      printf("Bad number of arguments supplied.\n");
	  return -1;
	}
	tsfi.dirname = argv[2]; 
	tsfi.filename = argv[3]; 
	tsfo.dirname = argv[4]; 
	tsfo.filename = argv[5]; 

	/* Reading time series files */ 
	ts = readPreCsvTs(&tsfi, &n);

	/* Aggregate to monthly time series */
	if (!strcmp(argv[1], "month")){
		printf("%s aggregation for %s\n", argv[1], tsfi.filename);
		tsm = aggDayToMonth1(ts, n, &nm);

		/* Write the monthly ts */
		if (writeMonthlyTs2csv(&tsfo, tsm, nm)){
			free(tsm);
			return -1;
		}
		free(tsm);

	/* Aggregate to yearly time series */
	}else if(!strcmp(argv[1], "year")){
		printf("%s aggregation for %s\n", argv[1], tsfi.filename);
		tsy = tsYearlySummary(ts, n, &ny);

		/* Write the yearly ts */
		if (writeTsYearlySummary(&tsfo, tsy, ny)){
			free(tsy);
			return -1;
		}
		free(tsy);

	/* Aggregate to daily flow regime */
	}else if(!strcmp(argv[1], "dregi")){
		printf("%s aggregation for %s\n", argv[1], tsfi.filename);
		dre = dailyRegime(ts, n, &nred);

		/* Write the yearly ts */
		if (writeRegi2csv(&tsfo, dre, nred)){
			free(dre);
			return -1;
		}
		free(dre);

	}else if(!strcmp(argv[1], "mregi")){
		printf("%s aggregation for %s\n", argv[1], tsfi.filename);
		mre = monthlyRegime(ts, n);

		/* Write the yearly ts */
		if (writeRegi2csv(&tsfo, mre, NYMON)){
			free(mre);
			return -1;
		}
		free(mre);


	}else{
		printf("Error! agg option is not correct\n");
	}


	
	/* Write pre-treated time series */
	/*if (writeTserie2csv(&tsfo, ts2, n))
		return -1;*/
    /*nameout = strtok(tsf.filename, ".");	
	while( nameout != NULL ) {
      printf( " %s\n", nameout); 
      nameout = strtok(NULL, " ");
	}*/

	/* Free heap mem */
	free(ts);
	return 0;
}

