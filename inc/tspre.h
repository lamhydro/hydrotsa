#ifndef __TSPRE_H__
#define __TSPRE_H__

#include <time.h>
#include <stdbool.h>

#include "utils.h"

#define RATIODAYS 0.67

typedef enum weekdays{Sunday, Monday, Tuesday, Wednesday, Thursday, 
		  Friday, Saturday} ewdays;

typedef enum months{January, February, March, April, May, June, July, 
			August, September, October, Novermber, December} emonths;

typedef struct timeSeries{
	int n;
	int *year, *month, *day;
	float *var;
	/*char **var_symbol;*/
} tserie;

typedef struct completeTimeSeries{
	struct tm dt;
	float var;
} ctserie;

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
Read and display bin file with tserie
*/
int readBinFile(char *filename, tserie *ts);


/*
Read time series from csv file
*/
/*int readTSfromFile(tfile *tsf, tserie *ts);*/
tserie *readTSfromFile(tfile *tsf);

/*
Save a tserie struct into a csv file
*/
int writeTserie2csv(const tfile *tsf, const ctserie *ts, const int n); 

/*
Save a tserie struct into a binary file
*/
int writeTserie2bin(tfile *tsf, tserie *ts); 

/*
Allocate heap memory of an struct tserie
*/
tserie *allocMemTs(int n);

/*
Free heap memory taken by tserie struct
*/
void freeMemTs(tserie *ts);

/*
Pre-treatment of ts
*/
/*tserie *preTreatTS(tserie *ts1);*/
ctserie *preTreatTS(tserie *ts1, int *nn);

/* 
Get most complete years in a ts 
*/
ctserie *getCompleteYearTS(ctserie *ts, int n, int *nc);

/* 
Fill in nan values in a ts
*/
ctserie *fillInNanInTS(ctserie *ts1, int n);


/*
Remove rows that containt NAN values from  ts struct.
*/
int removeNaNFromTs1(tserie *ts1, tserie *ts2);
/*
Remove rows that containt NAN values from  ts1 struct. Return an array of ts2 ctserie struct and its length 
n2.
*/
ctserie *removeNaNFromTs2(ctserie *ts1, int n1, int *n2);

/*
Extract one year of a time series and store in a tserie struct.
*/
tserie *extractOneYearTSFromTS(tserie *ts1, int year);

/*
Extract one month of a time series and store in a tserie struct.
*/
int extractOneMonthTSFromTS(tserie *ts1, int year, int month, tserie *ts2);
	
/*
void ndaysBetweenDates();
*/

#endif
