#ifndef __NORMAL_H__
#define __NORMAL_H__

#include "freqa.h"

/*
Fit Gaussian distribution to x data based on the Method of Moments
Estimation by MLE and Method of Moments are equal.
*/
pdfpar mom_normal_pdf(float *x, int n);

/*
Return the probability p of x based on a Gaussian distribution with parameters mi and sigma.
*/
double *normal_pdf(pdfpar *pdfp, float *x, int n);

/*
Return the cumulative probability p of x based on a Gaussian distribution with parameters mi and sigma.
using  Handbook of Mathematical Functions by Abramowitz and Stegun.
*/
double *normal_cdf(pdfpar *pdfp, float *x, int n);

#endif
