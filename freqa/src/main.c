#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "freqa.h"
#include "stats.h"
#include "utils.h"


int main(int argc, char *argv[])
{
	//tfile tsf;

	/* Assigning ext arg to variables */
	if( argc != 3 ) {
      printf("Bad number of arguments supplied.\n");
	  return -1;
	}
	//tsf.dirname = argv[1]; 
	//tsf.filename = argv[2]; 

	/* Building histogram */
	float x[] = {173, 18, 20, 60, 99, 81, 40, 33, 27, 0};	
	
	/* Uniform distritution */
	pdfpar pdfpu = mle_uniform_pdf(x,10);
	double *pu = uniform_pdf(&pdfpu, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, p = %f\n", x[i], pu[i]);
	free(pu);
	double *cpu = uniform_cdf(&pdfpu, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, cp = %f\n", x[i], cpu[i]);
	free(cpu);
	printf("\n");

	/* Gaussian distribution */
	pdfpar pdfpg = mom_normal_pdf(x,10);
	double *pg = normal_pdf(&pdfpg, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, p = %f\n", x[i], pg[i]);
	free(pg);
	double *cpg = normal_cdf(&pdfpg, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, cp = %f\n", x[i], cpg[i]);
	free(cpg);
	printf("\n");

	/* Log Gaussian distribution */
	pdfpar pdfpl = mom_lnormal_pdf(x,10);
	printf("miu = %f, sigma = %f\n", pdfpl.p1, pdfpl.p2);
	double *pl = lnormal_pdf(&pdfpl, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, p = %f\n", x[i], pl[i]);
	free(pl);
	double *cpl = lnormal_cdf(&pdfpl, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, cp = %f\n", x[i], cpl[i]);
	free(cpl);
	printf("\n");

	/* Exponential distribution */
	pdfpar pdfpe = mom_exponential_pdf(x, 10);
	printf("lambda = %f\n", pdfpe.p1);
	double *pe = exponential_pdf(&pdfpe, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, p = %f\n", x[i], pe[i]);
	free(pe);
	double *cpe = exponential_cdf(&pdfpe, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, cp = %f\n", x[i], cpe[i]);
	free(cpe);
	printf("\n");

	/* GEV distribution */
	//pdfpar pdfpv = mle_gev_pdf(x,10);
	//pdfpar pdfpv = quasiNewMet(x,10);
	pdfpar pdfpv = levenbergMarquardtMet(x, 10);
	printf("miu = %f, sigma = %f, xi = %f\n", pdfpv.p1, pdfpv.p2, pdfpv.p3);
	double *pv = gev_pdf(&pdfpv, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, p = %f\n", x[i], pv[i]);
	free(pv);
	double *cpv = gev_cdf(&pdfpv, x, 10);
	for(int i = 0; i < 10; i++) printf("x = %f, cp = %f\n", x[i], cpv[i]);
	free(cpv);
	printf("\n");

	/* Probability function fitting */
	//pdpar pdp;
	//pdp = methodOfMoments(x, 10);
	//exponential_pdf(&pdp, x[0]);
	//printf("loc =%f, scale = %f, shape = %f", pdp.loc, pdp.scale, pdp.shape);
	//
	//float *y = rangen(minval(x,10), maxval(x,10), 20);
	//float *p = uniform_pdf(x, 10);
	//for(int i = 0; i < 10; i++)
	//	printf("x = %f, p = %f\n", x[i], p[i]);
	//free(x);
	//free(p);
	
	//float *y = range(10, 20, 0.6);
	//size_t n = round((20-10)/0.6) + 1;
	//for(int i = 0; i<n;  i++)
	//	printf("y = %f\n",y[i]);
	//free(y);
	//printf("\n");
	//float *yy = rangen(1, 10, 300000);
	//for(int i = 0; i<300000;  i++)
	//	printf("y = %f\n",yy[i]);
	//free(yy);



	return 0;
}

