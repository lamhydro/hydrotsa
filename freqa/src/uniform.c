#include "uniform.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"

/*
Fit uniform distribution to x data based on the Method of Moments
*/
pdfpar mom_uniform_pdf(float *x, int n){
	double m, s;
	pdfpar pdfp;

	m = mean(x,n);
	s = stdDev(x,n);
	pdfp.p1 = m - s*sqrt(3); //a
	pdfp.p2 = m + s*sqrt(3); //b
	return pdfp;
}

/*
Fit uniform distribution to x data based on the MLE 
*/
pdfpar mle_uniform_pdf(float *x, int n){
	double a1, b1, a2, b2, dummy, da, db;
	pdfpar pdfp;
	double x1[n];
	int i;
	
	for(i = 0; i < n; i++) x1[i] = x[i];

	ascenSort(x1, n); 
	a1 = x1[0];
	b1 = x1[n-1];
	dummy = (n+1)/n;
	while(1){
		a2 = b1 + dummy*(x1[0]-b1);
		b2 = a1 + dummy*(x1[n-1]-a1);
		da = a2-a1;
		db = b2-b1;
		if (da < 0) da *= -1.;
		if (db < 0) db *= -1.;
	
		if(da < ERR && db < ERR)
			break;
		a1 = a2;
		b1 = b2;
	}
	pdfp.p1 = a2; //a
	pdfp.p2 = b2; //b
	
	return pdfp;
}

/*
Return the probability p of x based on a uniform distribution with parameters a and b.
*/
double *uniform_pdf(pdfpar *pdfp, float *x, int n){
	int i;
	double a, b;
	double *p;

	p = (double *)malloc(n * sizeof(double));
	
	//pdfp = mom_uniform_pdf(x, n);
	//pdfp = mle_uniform_pdf(x, n);

	a = pdfp->p1; 
	b = pdfp->p2;

	printf("a = %f, b = %f\n", a,b);
	for(i = 0; i < n; i++){
		if (x[i] >= a  && x[i] <= b)
			p[i] = 1/(b-a);
		else
			p[i] = 0;
	}
	return p;
}

/*
Return the cumulative probability p of x based on a uniform distribution with parameters a and b.
*/
double *uniform_cdf(pdfpar *pdfp, float *x, int n){
	int i;
	double a, b;
	double *p;

	p = (double *)malloc(n * sizeof(double));

	a = pdfp->p1; 
	b = pdfp->p2;
	for(i = 0; i < n; i++){
		if (x[i] < a)
			p[i] = 0;
		else if (x[i] >= a  && x[i] <= b)
			p[i] = (x[i]-a)/(b-a);
		else
			p[i] = 1;
	}
	return p;
}
