#include "lognormal.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"

/*
Fit Log Gaussian distribution to x data based on the Method of Moments
Estimation by MLE and Method of Moments are equal.
*/
pdfpar mom_lnormal_pdf(float *x, int n){
	int i;
	pdfpar pdfp;
	float xl[n];
	
	for(i = 0; i < n; i++){
		if (x[i]<=0){
			printf("x <= 0 STOPPING...\n");
		//	exit(0);
			pdfp.p1 = NAN;
			pdfp.p2 = NAN; 
			return pdfp;
		}
		xl[i] = log(x[i]);
	}
	pdfp.p1 = mean(xl,n);
	pdfp.p2 = stdDev(xl,n);
	return pdfp;
}
/*
//Fit Log Gaussian distribution to x data based on the Method of Moments
*/
//pdfpar mle_lgauss_pdf(float *x, int n){
//	pdfpar pdfp;
//	double xb, s;
//
//	xb = mean(x,n);
//	s = stdDev(x,n);
//	pdfp.p1 = log(xb/sqrt(s*s/(xb*xb) + 1)); //miu
//	pdfp.p2 = sqrt(log(s*s/(xb*xb) + 1)); //sigma
//	
//	return pdfp;
//}
/*
Return the probability p of x based on a Log Gaussian distribution with parameters mi and sigma.
*/
double *lnormal_pdf(pdfpar *pdfp, float *x, int n){
	int i;
	double mi, sigma;
	double a, b;
	double *p;

	p = (double *)malloc(n * sizeof(double));

	mi = pdfp->p1;
	sigma = pdfp->p2;

	b = -1/(2*sigma*sigma);
	for(i = 0; i < n; i++){
		//if (x[i] <= 0){
		//	p[i] = 0;
		//}else{
		a = 1/(x[i]*sigma*sqrt(2*M_PI));
		p[i] = a*exp(b*(log(x[i])-mi)*(log(x[i])-mi));
		//}
	}
	return p;
}
/*
Return the cumulative probability p of x based on a Gaussian distribution with parameters mi and sigma.
using  Handbook of Mathematical Functions by Abramowitz and Stegun.
*/
double *lnormal_cdf(pdfpar *pdfp, float *x, int n)
{
	int i;
	float mi, sigma;
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
		//if (log(x[i]) < 0){
		//	p[i] = 0;
		//}else{
		// Save the sign of x
    	sign = 1;
		xs = (log(x[i])-mi)/sigma;	
    	if (xs < 0){
    		sign = -1;
			xs *=-1;
		}
    	xs = xs/sqrt(2.0);

    	// A&S formula 7.1.26
    	t = 1.0/(1.0 + p1*xs);
    	y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-xs*xs);

    	p[i] = 0.5*(1.0 + sign*y);
		//}
	}
	return p;
}


