#ifndef __EXPONENTIAL_H__
#define __EXPONENTIAL_H__

#include "freqa.h"

/*
Fit exponential distribution to x data based on the Method of Moments
Estimation by MLE and Method of Moments are equal.
*/
pdfpar mom_exponential_pdf(float *x, int n);

/*
Return the probability p of x based on a exponential distribution with parameters lambda.
*/
double *exponential_pdf(pdfpar *pdfp, float *x, int n);

/*
Return the cumulative probability p of x based on a exponential distribution with parameters lambda.
*/
double *exponential_cdf(pdfpar *pdfp, float *x, int n);

#endif
