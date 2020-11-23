#ifndef __UNIFORM_H__
#define __UNIFORM_H__

#include "freqa.h"

/*
Fit uniform distribution to x data based on the Method of Moments
*/
pdfpar mom_uniform_pdf(float *x, int n);

/*
Fit uniform distribution to x data based on the MLE 
*/
pdfpar mle_uniform_pdf(float *x, int n);
	
/*
Return the probability p of x based on a uniform distribution with parameters a and b.
*/
double *uniform_pdf(pdfpar *pdfp, float *x, int n);

/*
Return the cumulative probability p of x based on a uniform distribution with parameters a and b.
*/
double *uniform_cdf(pdfpar *pdfp, float *x, int n);

#endif
