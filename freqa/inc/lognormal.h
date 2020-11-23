#ifndef __LOGNORMAL_H__
#define __LOGNORMAL_H__

#include "freqa.h"

/*
Fit Log Gaussian distribution to x data based on the Method of Moments
Estimation by MLE and Method of Moments are equal.
*/
pdfpar mom_lnormal_pdf(float *x, int n);

/*
Return the probability p of x based on a Log Gaussian distribution with parameters mi and sigma.
*/
double *lnormal_pdf(pdfpar *pdfp, float *x, int n);

/*
Return the cumulative probability p of x based on a Gaussian distribution with parameters mi and sigma.
using  Handbook of Mathematical Functions by Abramowitz and Stegun.
*/
double *lnormal_cdf(pdfpar *pdfp, float *x, int n);


#endif
