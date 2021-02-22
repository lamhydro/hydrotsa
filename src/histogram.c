#include "histogram.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"


/*
Allocate memory for histogram struct.
*/
void allocMemHist(hist *h, int nclass){
	h->nc = nclass;
	h->lc = (float *)malloc(h->nc * sizeof(float));
	h->uc = (float *)malloc(h->nc * sizeof(float));
	h->mc = (float *)malloc(h->nc * sizeof(float));
	h->p = (float *)malloc(h->nc * sizeof(float));
	h->pa = (float *)malloc(h->nc * sizeof(float));
	h->f = (int *)malloc(h->nc * sizeof(int));
	h->fa = (int *)malloc(h->nc * sizeof(int));
}

/*
Free memory for histogram struct.
*/
void freeMemHist(hist *h){
	free(h->lc);
	free(h->uc);
	free(h->mc);
	free(h->p);
	free(h->pa);
	free(h->f);
	free(h->fa);
}

/*
Create a histogram of x float array. Return histogram
information within struct hist.
*/
int makeHistogram(float *x, int n, hist *h){
	float min, max;
	float delta, wclass;
	int i, j, k; 
	int ka = 0;
	float pa = 0;

	min = minval(x, n);
	max = maxval(x, n);

	min = min*0.99;
	max = max*1.01;
	delta = max - min;
	wclass = delta/h->nc;

	h->lc[0] = min;
	h->uc[0] = h->lc[0]+wclass;
	h->mc[0] = h->lc[0]+wclass*0.5;

	for(i = 1; i < h->nc ; i++){
		h->lc[i] = h->uc[i-1];
		h->uc[i] = h->lc[i] + wclass;
		h->mc[i] = h->lc[i] + wclass*0.5;
	}
	
	for(i = 0; i < h->nc; i++){
		k = 0;
		for(j = 0; j < n; j++){
			if(i == h->nc-1){
				if((x[j] >= h->lc[i]) && (x[j] <= h->uc[i]))
					k++;
			}else{
				if((x[j] >= h->lc[i]) && (x[j] < h->uc[i]))
					k++;
			}
		}
		h->f[i] = k;
		h->p[i] = (float) k/n;
		ka += k;
		pa += (float) k/n;
		h->fa[i] = ka;
		h->pa[i] = pa;
	}
	return 0;
}


/* 
Compute the number of histogram classes based on 
array len n ustin Sturges and Rice methods.
*/
int nclassHist(int n, char *method){
	if (strcmp(method, "sturges")){
		return (int) log10(n)/log10(2) + 1;
	}else if (strcmp(method,"rice")){
		return (int) 2*cbrt(n);
	}else{
		printf("Error: incorrect method");
		return -1;
	}
}

