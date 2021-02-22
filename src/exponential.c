#include "exponential.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"

/*
Fit exponential distribution to x data based on the Method of Moments
Estimation by MLE and Method of Moments are equal.
*/
pdfpar mom_exponential_pdf(float *x, int n){
	int i;
	pdfpar pdfp;

	for (i = 0; i < n; i++){
		if (x[i] < 0){
			printf("x < 0 STOPPING...\n");
		//	exit(0);
			pdfp.p1 = NAN;
			return pdfp;
		}
	}

	pdfp.p1 = 1/mean(x,n);
	//pdfp.p2 = stdDev(xl,n);
	return pdfp;
}
/*
Return the probability p of x based on a exponential distribution with parameters lambda.
*/
double *exponential_pdf(pdfpar *pdfp, float *x, int n){
	int i;
	double lambda; //, gamma;
	double *p;

	p = (double *)malloc(n * sizeof(double));
	lambda = pdfp->p1;
	//gamma = pdp->shape;
	//x = x-gamma;
	for(i = 0; i < n; i++){
		if(x[i] >= 0){
			p[i] = lambda*exp(-lambda*x[i]);
		}else{
			p[i] = 0;
		}
	}
	return p;
}
/*
Return the cumulative probability p of x based on a exponential distribution with parameters lambda.
*/
double *exponential_cdf(pdfpar *pdfp, float *x, int n){
	int i;
	double lambda; //, gamma;
	double *p;

	p = (double *)malloc(n * sizeof(double));
	lambda = pdfp->p1;
	//gamma = pdp->shape;
	//x = x-gamma;
	for(i = 0; i < n; i++){
		if(x[i] >= 0){
			p[i] = 1 - exp(-lambda*x[i]);
		}else{
			p[i] = 0;
		}
	}
	return p;
}

//int func1(float *x, int n, int nb, void (*f)(int)){
//	float m, s;
//
//	gaudi->p1 = mean(x,n);
//	gaudi->p2 = stdDev(x,n);
// //	(*f)( //}
