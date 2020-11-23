#ifndef __TSPRE_H__
#define __TSPRE_H__

#include <time.h>
#include <stdbool.h>

#include "utils.h"

//#define BUF_LEN 256

typedef enum weekdays{Sunday, Monday, Tuesday, Wednesday, Thursday, 
		  Friday, Saturday} ewdays;

typedef enum months{January, February, March, April, May, June, July, 
			August, September, October, Novermber, December} emonths;

typedef struct timeSeries{
	unsigned n;
	int *year, *month, *day;
	float *var;
	//char **var_symbol;
} tserie;

typedef struct timeSer{
	unsigned n;
	struct tm dt;
	float var;
} tser;

typedef struct dayTSinventory{
	int year, nmonths, ndayspy;
	int month[12];
	int dayspm[12];
} dtsinv;

/*
Read the time series from a csv file
*/
int readFile(char *filename, tserie *ts);

/*
Read time series from csv file
*/
int readTSfromCSV(tfile *tsf, tserie *ts);

/*
Save a tserie struct into a csv file
*/
int writeTserie2csv(tfile *tsf, tserie *ts); 
	
/*
Allocate heap memory of an struct tserie
*/
void allocMemTs(tserie *ts, int n);

/*
Free heap memory taken by tserie struct
*/
void freeMemTs(tserie *ts);

/*
Pre-treatment of ts
*/
int preTreatTS(tserie *ts1, tserie *ts2);

/*
Remove rows that containt NAN values from  ts struct.
*/
int removeNaNFromTs(tserie *ts1, tserie *ts2);

/*
Extract one year of a time series and store in a tserie struct.
*/
int extractOneYearTSFromTS(tserie *ts1, int year, tserie *ts2);

/*
Extract one month of a time series and store in a tserie struct.
*/
int extractOneMonthTSFromTS(tserie *ts1, int year, int month, tserie *ts2);
	
//int ndaysBetweenDates(struct tm startdate, struct tm enddate){
void ndaysBetweenDates();


#endif
