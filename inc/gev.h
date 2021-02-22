#ifndef __GEV_H__
#define __GEV_H__

#define ERR_GEV 1.e-5
#define NMAXITER 2000
#define SRF 0.25
#define MAXSR 30
#define ACCE 0.00001 
#define ACCA 0.00001 
#define ACCK 0.00001 
#define LAMB 1e-2
#define NPAR 3
#define ALPHA0 1e-2
#define C1 1e-4 
#define C2 0.9

#include "freqa.h"

/*
Fit Generalize Extreme Value (GEV) distribution to x data based on the L Moments
See 'Generalized maximum-likelihood generalized extreme-valuequantile estimators 
for hydrologic data' by Martins and Stinger, 2000.
It is a very good approximation for k(=pdfp.p3) in the range (0.5,  0.5).  
*/
pdfpar pwme_gev_pdf(float *x, int n);
pdfpar mle_gev_pdf(float *x, int n);
pdfpar quasiNewMet(float *data, int n);
pdfpar levenbergMarquardtMet(float *data, int n);

/*
Return the probability p of x based on a GEV distribution with parameters mi, sigma and xi.
*/
double *gev_pdf(pdfpar *pdfp, float *x, int n);

/*
Return the cumulative probability p of x based on a GEV distribution with parameters mi, sigma and xi.
*/
double *gev_cdf(pdfpar *pdfp, float *x, int n);
	

#endif
