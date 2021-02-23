#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "freqa.h"

typedef struct histogram{
	int nc;
	float *lc, *uc, *mc, *p, *pa;
	int *f, *fa;
} hist;

/*
Allocate memory for histogram struct.
*/
void allocMemHist(hist *h, int nclass);

/*
Free memory for histogram struct.
*/
void freeMemHist(hist *h);

/*
Create a histogram of x float array. Return histogram
information within struct hist.
*/
int makeHistogram(float *x, int n, hist *h);

/* 
Compute the number of histogram classes based on 
array len n ustin Sturges and Rice methods.
*/
int nclassHist(int n, char *method);


#endif
