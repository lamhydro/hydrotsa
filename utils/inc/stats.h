#ifndef __STATS_H__
#define __STATS_H__

#include <math.h>
#include "tspre.h"

float mean(float *x, int n);
float median(float *x, int n);
float variance(float *x, int n);
float stdDev(float *x, int n);
float coeffVari(float *x, int n);
float skewness(float *x, int n);
float covariance(float *x, float *y, int n);
float correlation(float *x, float *y, int n);

/*
Filter a 1d array according to the wei array of weights.
Return a filtered 1d array of length n.
*/
int filter(float *x, int n, float *wei, int nwei, float *xs);

/*
Baseflow index defined as 7-day minimun flow/mean flow for year
*/
float baseFlowIndex(tserie *ts);

/*
Median of all positive differences between consecutive daily values
*/
float riseRate(float *x, int n);
	
/*
Median of all negative differences between consecutive daily values
*/
float fallRate(float *x, int n);

/*
Estimate the percentiles y in p of the 1d array x
*/
int perctl(float *x, int nx, float *y, int ny, float *p);

/*
Partial autocorrelation
*/
float partialautocorre(float *x, int n, int k);

/*
Autocorrelation function of 1d complete time series
*/
int	acf(float *x, int n, float *r);
	
/*
Partial autocorrelation function of 1d complete time series
*/
int	pacf(float *x, int n, int nk, float *r);
	
int bpacf1(float *x, int n, int ha, float *pi);
#endif
