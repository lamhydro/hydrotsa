#include "stats.h"
#include "utils.h"

float mean(float *x, int n){
	float sum = 0.0;
	int i;
	for(i = 0; i < n; i++)
		sum += x[i];
	return sum/n;
}

float median(float *x, int n){
	int i;
	double *y;
	double m;

	y = (double *)malloc(n*sizeof(double));

	for(i = 0; i < n; i++)
		y[i] = x[i];

	ascenSort(y,n);
	if(n%2==0){ /* even number */
		i = n/2;
		m = (y[i]+y[i-1])*0.5;
	}else{ /* odd number */
		i = (n+1)/2;
		m = y[i-1];
	}
	free(y);
	return m; 
}

float variance(float *x, int n){
	float sum = 0.0;
	float ave = mean(x, n);
	int i;
	for(i = 0; i < n; i++)
		sum += pow((x[i] - ave),2.);
	return sum/n;
}

float stdDev(float *x, int n){
	return sqrt(variance(x, n));
}

float coeffVari(float *x, int n){
	return stdDev(x,n)/mean(x,n);
}

float skewness(float *x, int n){
	float sum = 0.0;
	float ave = mean(x, n);
	int i;
	for(i = 0; i < n; i++)
		sum += pow((x[i] - ave),3);
	return sum/n/pow(stdDev(x, n),3);
}

float covariance(float *x, float *y, int n){
	int i;
	float sum = 0;
	float mx, my;

	mx = mean(x, n);
	my = mean(y, n);
	for(i = 0; i < n; i++)
		sum += (x[i] - mx) * (y[i] - my);
	
	return sum/n;
}

float correlation(float *x, float *y, int n){
	float Sx = variance(x, n);
	float Sy = variance(y, n);
	float Sxy = covariance(x, y, n);

	return Sxy/(Sx*Sy);
}

/*
Filter a 1d array according to the wei array of weights.
Return a filtered 1d array of length n.
*/
int filter(float *x, int n, float *wei, int nwei, float *xs){
	int i, j, k;
	int d;
	float sum;

	if (nwei%2==0){ /* even number */
		d = nwei/2;
		k = 0;
	}else{ /* odd number */
		d = (nwei+1)/2;
		k = 1;
	}

	for(j = 0; j < n; j++){
		if ((j < d-k) || (j > n-d)){
			xs[j]=NAN;
		}else{
			sum = 0;
			for(i = j; i < nwei+j; i++)
				sum += x[i-d+k]*wei[i-j];
			sum /= nwei;
			xs[j] = sum;
		}
	}
	return 0;
}


/*
Baseflow index defined as 7-day minimun flow/mean flow for year
*/
float baseFlowIndex(tserie *ts){

	float *xs, *xsn;
	int nnonan;
	float wei[] = {1,1,1,1,1,1,1};
	float bfix;

	xs = (float *)malloc(ts->n * sizeof(float));

	/* 7-day flow */
	filter(ts->var, ts->n, wei, 7,xs);

	/* Remove nan's */
	nnonan = countNoNaNs(xs, ts->n);
	xsn = (float *)malloc(nnonan * sizeof(float));
	removeNaNFromArr(xs, ts->n, xsn);

	/* Min 7-day flow */
	bfix = minval(xsn, nnonan)/mean(ts->var, ts->n);
	free(xsn);
	free(xs);

	return bfix; 

}


/*
Median of all positive differences between consecutive daily values
*/
float riseRate(float *x, int n){
	int i, j;
	float d, rate;
	float *r;
	
	j = 0;
	for(i = 0; i < n-1; i++){
		d = x[i+1]-x[i];
		if(d > 0){
			j++;
		}
	}
	r = (float *)malloc(j * sizeof(float));
	
	j = 0;
	for(i = 0; i < n-1; i++){
		d = x[i+1]-x[i];
		if(d > 0){
			r[j] = d;
			j++;
		}
	}
	rate = median(r, j);
	free(r);
	
	return rate;
}

/*
Median of all negative differences between consecutive daily values
*/
float fallRate(float *x, int n){
	int i, j;
	float d, rate;
	float *r;
	
	j = 0;
	for(i = 0; i < n-1; i++){
		d = x[i+1]-x[i];
		if(d < 0){
			j++;
		}
	}
	r = (float *)malloc(j * sizeof(float));
	
	j = 0;
	for(i = 0; i < n-1; i++){
		d = x[i+1]-x[i];
		if(d < 0){
			r[j] = d;
			j++;
		}
	}
	rate = median(r, j);
	free(r);
	
	return rate;
}

/*
Estimate the percentiles y in p of the 1d array x
*/
int perctl(float *x, int nx, float *y, int ny, float *p){
	int i, j;
	double *xs;
	float id;
	float dummy;

	xs = (double *)malloc(nx*sizeof(double));	

	for(i = 0; i < nx; i++)
		xs[i] = x[i];

	ascenSort(xs,nx);
	dummy = (nx-1)*0.01;
	for(i = 0; i < ny; i++){
		id = dummy*y[i];
		for(j = 1; j < nx; j++){
			if(id >= j-1 && id <= j){
				/*p[i] = xs[j-1] + (xs[j]-xs[j-1])*(id-j+1);*/
				p[i] = xs[j-1] + (xs[j]-xs[j-1])*(id-j+1);
				break;
			}
		}
	}
	free(xs);
	return 0;
}

/*
Autocorrelation function of 1d complete time series
*/
float autocorre(float *x, int n, int k){
	int i;
	float sum = 0;
	float mx;

	mx = mean(x, n);
	for(i = 0; i < n-k; i++)
		sum += (x[i] - mx) * (x[i+k] - mx);
	
	sum /= n;
	/*sum /= n-k; // Definition with less bias */
	return sum/variance(x,n);
}

/*
Autocorrelation function of 1d complete time series
*/
int	acf(float *x, int n, float *r){
	int i;

	for(i = 0; i < n; i++)
		r[i] = autocorre(x, n, i);

	return 0;
}

/*
Partial autocorrelation
*/
float partialautocorre(float *x, int n, int k){
	int i, j;
	double **ma;
	double *vx;
	double *vy;
	double *r;
	double nma[NROWS][NCOLS];
	float d;
	double **inverse;
	float pr;

	ma = allocdmat(k-1, k-1);
	vy = (double *)malloc((k-1) * sizeof(double));
	r = (double *)malloc(k * sizeof(double));

	for(i = 0; i < k; i++)
		r[i] =autocorre(x,n,i); 

	if (k-1 == 1){
		pr = r[1];
		return pr;
	}

	for(i = 0; i < k-1; i++)
		vy[i] = r[i+1];
		
	for(i = 0; i < k-1; i++){
		for(j = i; j < k-1; j++){
			ma[i][j] = r[j-i];
			/*if (i != j)*/
			ma[j][i] = r[j-i];
		} 
	}

/*	for(i = 0; i < k-1; i++){
//		for(j = 0; j < k-1; j++){
//			printf("%f ",ma[i][j]);
//		}
//		printf(" x %f = %f\n", vx[i], vy[i]);
	}*/
	for(i = 0; i < k-1; i++){
		for(j = 0; j < k-1; j++){
			nma[i][j] = ma[i][j];
		}
	}

	inverse = allocdmat(k-1, k-1);
	d = determinant(nma, k-1);
	if (d == 0)
		printf("\nInverse of Entered Matrix is not possible\n");
	else{
		cofactor(nma, k-1, inverse);
	}
/*	printf("\n\n\nThe inverse of matrix is : \n");
//
//   for (i = 0;i < k-1; i++)
//    {
//     for (j = 0;j < k-1; j++)
//       {
//         printf("\t%f", inverse[i][j]);
//        }
//    printf("\n");
     }*/
	
	vx = dMatVecMult(inverse, vy, k-1, k-1);
	/*printf("k = %d\n",k);
	//for(i = 0; i < k-1; i++)
	//	printf("vx = %f\n", vx[i]);
	printf("\n");*/

	pr = vx[k-1-1];	
	free(vx);
	free(vy);
	free(r);
	deallocdmat(inverse,k-1);
	deallocdmat(ma,k-1);
	return pr;
}

/*
Partial autocorrelation function of 1d complete time series
*/
int	pacf(float *x, int n, int nk, float *r){
	int i;

	r[0] = 1;
	/*r[1] = autocorre(x,n,1);*/
	for(i = 2; i < nk; i++)
		r[i-1] = partialautocorre(x, n, i);
	return 0;
}

/*
Partial autocorrelation function of 1d complete time series
using hte Durbin-levinson recursive algorithm
*/
int	pacf1(float *x, int n, int ha, int h, float **p){
	int j;
	float *r;
	float num, den;
	

	/*printf("%d\n", h);*/
	if (h <= ha){
		r = (float *)malloc((h+1)*sizeof(float));
		for(j = 0; j < h+1; j++){
			r[j] = autocorre(x,n,j); 
			/*printf("%f\n",r[j]);*/
		}

		num  = 0;
		den = 0;
		for (j = 0; j < h-1; j++){
			num += p[h-1-1][j]*r[h-1-j];
			den += p[h-1-1][j]*r[j+1];
		}
		p[h-1][h-1] = (r[h] - num)/(1 - den);
		for (j = 0; j < h-1; j++)
			p[h-1][j] = p[h-1-1][j]-p[h-1][h-1]*p[h-1-1][h-1-1-j];

		free(r);
		return pacf1(x, n, ha, ++h, p);
	}else{
		return -1;
	}

}

int bpacf1(float *x, int n, int ha, float *pi){

	int i;
	float **p;

	p = allocfmat(ha, ha);
	
	pacf1(x, n, ha,1, p);
	for(i = 0; i < ha; i++)
		pi[i] = p[i][i];
	
	deallocfmat(p, ha);
	return 0;
}
