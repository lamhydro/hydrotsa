#ifndef __FREQA_H__
#define __FREQA_H__


typedef struct probabilityDistri{
	int n;
	char *name;
	float p1,p2,p3,p4;
	float *pdf;
	float *cdf;
} prodis;

typedef struct probabilityDistributionParameters{
	float loc, scale, shape;
} pdpar;

typedef struct probabilityDistributionMoments{
	float mean, median, varianza, skewness, kurtosis;
} pdmon;

typedef struct probabilityDistributionParam{
	double p1,p2,p3;
} pdfpar;

#include "exponential.h"
#include "gev.h"
#include "histogram.h"
#include "gamma.h"
#include "lognormal.h"
#include "normal.h"
#include "uniform.h"


#endif
