#include "normal.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"

/*
Fit Gaussian distribution to x data based on the Method of Moments
Estimation by MLE and Method of Moments are equal.
*/
pdfpar mom_normal_pdf(float *x, int n){
	pdfpar pdfp;

	pdfp.p1 = mean(x,n);
	pdfp.p2 = stdDev(x,n);
	return pdfp;
}

/*
Return the probability p of x based on a Gaussian distribution with parameters mi and sigma.
*/
double *normal_pdf(pdfpar *pdfp, float *x, int n){
	int i;
	double mi, sigma;
	double a, b;
	double *p;

	p = (double *)malloc(n * sizeof(double));

	mi = pdfp->p1;
	sigma = pdfp->p2;

	a = 1/(sigma*sqrt(2*M_PI));
	b = -1/(2*sigma*sigma);
	
	for(i = 0; i < n; i++)
		p[i] = a*exp(b*pow(x[i]-mi,2));
	
	return p;
}

/*
Return the cumulative probability p of x based on a Gaussian distribution with parameters mi and sigma.
using  Handbook of Mathematical Functions by Abramowitz and Stegun.
*/
double *normal_cdf(pdfpar *pdfp, float *x, int n)
{
	int i;
	double mi, sigma;
	double *p;
	int sign;
	double t, y, xs;

    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p1  =  0.3275911;

	p = (double *)malloc(n * sizeof(double));

	mi = pdfp->p1;
	sigma = pdfp->p2;

	for(i = 0; i < n; i++){
		// Save the sign of x
    	sign = 1;
		xs = (x[i]-mi)/sigma;	
    	if (xs < 0){
    	    sign = -1;
			xs *=-1;
		}
    	xs = xs/sqrt(2.0);

    	// A&S formula 7.1.26
    	t = 1.0/(1.0 + p1*xs);
    	y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-xs*xs);

    	p[i] = 0.5*(1.0 + sign*y);
	}
	return p;
}

/*
Gaussian pdf and cdf of random variable
*/
//probdi *normalianDist(float *x, int n){
//	int i;	
//	float a, b; 
//	float xs[n];
//	probdi *gaudi;
//
//	gaudi->n = n;
//	gaudi->pdf = (prodis *)malloc(n * sizeof(prodis));
//	gaudi->cdf = (prodis *)malloc(n * sizeof(prodis));
//
//	gaudi->p1 = mean(x,n);
//	gaudi->p2 = stdDev(x,n);
//
//	for(i = 0; i < n; i++)
//		xs[i] = x[i];
//
//	ascenSort(xs, n);
//
//	a = 1/(gaudi->p2*sqrt(2*M_PI));
//	b = -1/(2*guadi->p2*gaudi->p2)
//	for(i = 0; i < n; i++){
//		gaudi->pdf[i] = a*exp(b*pow((xs[i]-gaudi->p1),2));
//	}
//
//	
//
//	return gaudi;
//}


