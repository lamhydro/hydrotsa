#include "gamma.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"


float gamma_pdf(pdpar *pdp, float x){	
	float alpha, beta, gamma;
	
	alpha = pdp->shape;
	beta = 1/pdp->scale;
	gamma = pdp->loc;

	x = x-gamma;
	if( x > 0 && alpha > 0 && beta > 0)
		return pow(beta,alpha)*pow(x,alpha-1)*exp(-beta*x)/lacz_gamma(alpha);
	else
		return 0.;
}
