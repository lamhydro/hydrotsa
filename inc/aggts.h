#ifndef __AGGTS_H__
#define __AGGTS_H__

#include <time.h>

#include "tspre.h"
#include "utils.h"

#define NYMON 12

typedef struct regime{
	int x;
	float mean, median, std, max, min;
} regi;

typedef struct aggregation{
	struct tm dt;
	float mean, median, std, max, min;
} agg;

typedef struct yearlySummary{
	int year;
	float mean, q25, q50, q75, std, covar, skew, range, min, max, bfix, riser, fallr;
	int maxday, minday;
} yrsum;

/* 
Count the number of months per year in at ts struct.
*/
int nMonthsInYearDayTs(const ctserie *ts, const int n);

/*
Aggreate daily ts into monthly ts.
*/
int aggDayToMonth(tserie *ts1, tserie *ts2);
agg *aggDayToMonth1(const ctserie *ts, const int n, int *nm);
	
/*
Aggregate daily ts into yearly ts.
*/
int aggDayToYear(tserie *ts1, tserie *ts2);
agg *aggDayToYear1(tserie *ts, int *n);

/*
Summary of statistic per year for a time series. This suppose to be
used for pre-treated time series
*/
yrsum *tsYearlySummary(const ctserie *ts, const int n, int *nyears);

/*
Write yearly summary of a ts into a csv file
*/
int writeTsYearlySummary(const tfile *ysf, const yrsum *ys, const int n);
	
/*
Aggregation to every month of the year
*/
regi *monthlyRegime(const ctserie *ts, const int n);

/*
Aggregation to every day of the year
*/
regi *dailyRegime(ctserie *ts, int n, int *ny);

/*
Save a regime str (regi) into a cvs file
*/
int writeRegi2csv(const tfile *regif, const regi *dre, const int n);

/*
Write the monthly time series (struct agg)
*/
int writeMonthlyTs2csv(const tfile *fi, const agg *ts, const int n);
	
/*
Return an 1d array with the day of the year of the tserie struct ts. 
*/	
int *dayOfYearTs(const ctserie *ts, const int n);
#endif
