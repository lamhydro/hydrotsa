#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aggts.h"
#include "freqa.h"
#include "lireg.h"
#include "stats.h"
#include "tspre.h"
#include "utils.h"

int main(int argc, char *argv[])
{

	//tserie ts, ts1, ts2, ts3;
	//char *filename;
	tfile tsf, tsfo, ysf;
	tserie ts1, ts2;
	//valpos minn, maxx;
	//arr1d t1, t2; 
	//arr1di x1, x2;
	//hist h1;
	//int i, j;

	/* Assigning ext arg to variables */
	if( argc != 3 ) {
      printf("Bad number of arguments supplied.\n");
	  return -1;
	}
	tsf.dirname = argv[1]; //"/home/luis/Documents/cpp/HYDATsfana/dayUnregLongFLOWSandCatch/";
	tsf.filename = argv[2]; //"01BJ001_DLY_FLOWS.csv";

	/* Reading time series files */ 
	readTSfromCSV(&tsf, &ts1);
	//printf("ts1.n %d\n",ts1.n);

	/* Pre-treatment of time series */
	if (preTreatTS(&ts1, &ts2))
		return -1;
	
	//printf("ts2.n %d\n",ts2.n);
	int nnans = countNaNs(ts2.var, ts2.n);
	if(nnans>0)
		printf("NANs=%d\n", nnans);
	
	int ny;	
	yrsum *ys = tsYearlySummary(&ts2, &ny);
	ysf.dirname = "./";
	ysf.filename = "yrsum.csv";
	if (writeTsYearlySummary(&ysf, ys, ny))
		return -1;
		
	//float x[]={1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6};

	float x[22] = {16272.01, 16472.37, 16776.43, 16790.19, 16912.29, 17050.75, 17084.49, 17131.86, 17081.89,\
				16924.75, 17141.75, 17215.97, 17230.54, 17217.11, 17168.61, 17489.16, 17646.7,\
				17623.05, 17581.43, 17779.52, 17755.8, 17663.54};

	//float r[22];
	//acf(x, 22, r);
	//partialautocorre(x, 16, 5);

	float r[20];

	bpacf1(x, 22, 20, r);
	//pacf(x, 22, 13, r);
	for (int i = 0; i < 20; i++) printf("pr = %f\n",r[i]);
	//regi *mre = monthlyRegime(&ts2);
	//for (int i = 0; i < 12; i++){
	//	printf("%d, mean = %f\n", mre[i].x, mre[i].mean);
	//}
	//free(mre);
	
	printf("gamma lacz = %15.8lf, gamma sp = %15.9lf\n", lacz_gamma(1/3.0), sp_gamma(1/3.0));
	//int dyear[ts1.n];
	//dayOfYearTs(&ts1, dyear);
	//for(int i = 0; i < ts1.n; i++)
	//	printf("%d,%d,%d,%d\n",ts1.year[i],ts1.month[i], ts1.day[i], dyear[i]);
	//struct tm da = { 0, 0, 0, 1, 2, 2000};
	//printf("dayaaaa %d\n",dayOfYear(da));
	//regi *dre = NULL;
	//regi *dre = (regi *)malloc(366 * sizeof(regi));
	//dailyRegime(&ts2, dre);
	//int nydays;
	//regi *dre = dailyRegime(&ts1, &nydays);
	//for (int i = 0; i < nydays; i++){
	//	printf("%d, mean = %f\n", dre[i].x, dre[i].mean);
	//}
	//free(dre);

	//tserie ts2m;
	//extractOneMonthTSFromTS(&ts1, ts1.year[0], ts1.month[0], &ts2m);
	//for(int i = 0; i < ts2m.n; i++)
	//	printf("%d, %d, %d, %f\n", ts2m.year[i], ts2m.month[i], ts2m.day[i], ts2m.var[i]);
	
	
	//int nmon;
	//agg *tsm = aggDayToMonth1(&ts1, &nmon);
	//for (int i = 0; i < nmon; i++){
	//	printf("%d, %d, mean = %f\n", tsm[i].dt.tm_year, tsm[i].dt.tm_mon, tsm[i].mean);
	//}
	//free(tsm);

	//int ny;
	//agg *tsy = aggDayToYear1(&ts2, &ny);
	//for (int i = 0; i < ny; i++){
	//	printf("%d, mean = %f\n", tsy[i].dt.tm_year, tsy[i].mean);
	//}
	//free(tsy);


//	float x[] = {1,3,4,5,6,6,5,2,3,8,1};
//	float wei[] = {1., 1., 1,1,1};
//	printf("%ld, %ld\n", sizeof(wei), sizeof(wei[0]));
//	float xs[11];
//	filter(x, 11, wei, sizeof(wei)/sizeof(wei[0]), xs);
//	for(int i = 0; i < 11; i++)
//		printf("%f, %f\n",x[i], xs[i]);

	/* write time series to csv file */
	tsfo.dirname = "./";
	tsfo.filename = "test.csv";
	if (writeTserie2csv(&tsfo, &ts2)) 
		return -1;
	//printf("size ts1:%d, size ts2:%d\n", ts1.n, ts2.n);
	/* Allocate heap memory for time series */


	/* Reading ts file */
	//ts = malloc(tsf.nrows * sizeof(tser));
		/* Freeing heap memory */
	//free(filename);
	freeMemTs(&ts1);
	freeMemTs(&ts2);
	//free(ts);
	
	/*printf("Hello world!\n");*/
	//tsf.nrows = nrowsFile(filename);	
	//printf("LINES: %d\n",tsf.nrows);
	//
	//allocMemTs(&ts, tsf.nrows-1);	
	//readFile(filename, &ts);

	//removeNaNFromTs(&ts, &ts1);

	//x1.n = ts1.n;
	//x1.a = (int *)malloc(x1.n * sizeof(int));
	//for(i = 0; i<ts1.n; i++)
	//	x1.a[i] = ts1.year[i];

	//*uniqueInt(&x1, &x2);*/

	//aggDayToYear(&ts1, &ts2);

	//printf("N months of the TS: %d\n", nMonthsInYearDayTs(&ts1));
	//
	//aggDayToMonth(&ts1, &ts3);
	//for(i = 0; i < ts3.n; i++)
	//	printf("Year %d Month %d Val %f\n", ts3.year[i], ts3.month[i], ts3.var[i]);
	//
	//printf("rrrrrrrrrrrrrrrrrr %d\n",ts1.n);
	//t2.n = ts1.n;
	//t2.a = (float *)malloc(t2.n * sizeof(float));
	//for (i = 0; i < ts1.n; i++)
	//	t2.a[i] = ts1.var[i];

	//*t1.n = ts.n;*/
	//t1.n = 0;
	//for(i = 0; i<ts.n; i++){
	//	if(!isnan(ts.var[i]))
	//		t1.n++;
	//}
	//printf("%d\n",t1.n);
	//t1.a = (float *)malloc(t1.n * sizeof(float));
	//j = 0;
	//for(i = 0; i<ts.n; i++){
	//	if(!isnan(ts.var[i])){
	//		t1.a[j] = ts.var[i];
	//		j++;	
	//	}
	//}
	//float t3[] = {0,45, 23, 1, 3, 5, 78};
	//ascenSort(t3, 7); 
	//for(i = 0; i<7; i++)
	//	printf("t3 %f\n", t3[i]);
	//descenSort(t3, 7); 
	//for(i = 0; i<7; i++)
	//	printf("t3 %f\n", t3[i]);
	//
	//printf("Median %f\n", median(t3, 7));
	//
	//ndaysBetweenDates();
	//*t1.a[i] = ts.var[i];
	//t1.a = &ts.var;*/
	//printf("dddd %d\n", t1.n );
	//printf("Variance = %f\n",variance(&t1));
	//printf("Mean = %f\n",mean(&t1));
	//printf("Mean rrrrrrrrrrrrr = %f\n",mean(&t2));
	//*printf("Max = %f\n",maxval(ts.var,ts.n));*/
	//maxx = maxval(&t1);
	//printf("Max = %f and Pos = %d\n",maxx.val, maxx.pos);
	//minn = minval(&t1);
	//printf("Min = %f and Pos = %d\n",minn.val, minn.pos);


	//
	//allocMemHist(&h1, nclassHist(t1.n, "sturges"));
	//makeHistogram(&t1, &h1);
	//float sump=0;
	//*for(i = 0; i<h1.nc; i++){
	//	printf("%f\n",h1.pa[i]);
	//	printf("[%f, %f]\n", h1.lc[i], h1.uc[i]);	
	//	sump+=h1.p[i];
	//}
	//printf("sump = %f\n",sump);*/

	//free(t1.a);	
	//freeMemTs(&ts);
	//freeMemTs(&ts1);
	//freeMemHist(&h1);
	//free(filename);
	//
	return 0;
}

