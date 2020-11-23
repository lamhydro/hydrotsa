#include "stats.h"
#include "lireg.h"

/*
Return the least-square point stimators alpha (intercep), beta (slope) 
and sigma (std dev of y given x) of a linear model (y = a^ + b^x).
See Bemjamin and Cornell 1970.
*/
void lSqrtMlM(float *x, float *y, int n, float *alpha, float *beta, float *sigma){
	float Sx = variance(x, n);
	float Sxy = covariance(x, y, n);

	float mx = mean(x,n);
	float my = mean(y,n);

	*beta = Sxy/pow(Sx,2);
	*alpha = my - (*beta)*mx;
	*sigma = sqrt((n/(n-2)) * ((1 - pow(correlation(x,y,n),2))*variance(y,n)));
}


