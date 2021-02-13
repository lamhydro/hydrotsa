#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define MAXCHAR 1000
#define MAX(i, j) (((i) > (j)) ? i : j)
#define MIN(i, j) (((i) < (j)) ? i : j)

#define NROWS 100
#define NCOLS 100

#define M_PI 3.14159265358979323846

#define ERR 1.e8
/*#define NAN (0.0/0.0)*/

#define A 12 /* constant for Gamma function */

#define NSECDAY 86400 /* # of seconds in a day */
#define IYEAR 1900	 /* # starting year for time lib. For previous year use julian day */

#define true 1
#define false 0

typedef struct typicalFile{
	char *dirname;
	char *filename;
	int nrows; 
} tfile;

typedef struct array1d{
	unsigned n;
	float *a; 

} arr1d;

typedef struct array1dInt{
	unsigned n;
	int *a; 

} arr1di;

typedef struct valAndPosInArray{
	int pos;
	float val;
} valpos;

typedef struct csvData{
	int year, month, day;
	float var;
	char var_symbol[5];
} csvd;


/* 
Allocate memory for a float  m[nrows][ncols] matrix
*/
float **allocfmat(int nrows, int ncols);

/* 
Deallocate memory for a float  m[nrows][ncols] matrix
*/
int deallocfmat(float **m, int nrows);

/* 
Allocate memory for a double m[nrows][ncols] matrix
*/
double **allocdmat(int nrows, int ncols);

/* 
Deallocate memory for a double m[nrows][ncols] matrix
*/
int deallocdmat(double **m, int nrows);


/* 
Allocate memory for a int  m[nrows][ncols] matrix
*/
int **allocimat(int nrows, int ncols);

/* 
Deallocate memory for a int m[nrows][ncols] matrix
*/
int deallocimat(int **m, int nrows);
	
/*
Multiplication of float 2d array and 1d array
*/
float *fMatVecMult(float **m, float *v, int nrows, int ncols);

/*
Multiplication of float 2d array and 1d array
*/
double *dMatVecMult(double **m, double *v, int nrows, int ncols);

/*
Count number of rows in a file
*/
int nrowsFile(char *filename);

/*
Count number of rows in a bin file
*/
int nrowsBinFile(char *filename);

/*
Return the max. val of the x array.
*/
float maxval(float *x, int n);

/*
Return the position of max. val in the x array.
*/
int imaxval(float *x, int n);

/*
Return the min. val of the x array.
*/
float minval(float *x, int n);

/*
Return the position of min. val in the x array.
*/
int iminval(float *x, int n);

/*
Count the number of values != NAN in x array
*/
int countNoNaNs(float *x, int n);

/*
Count the number of values = NAN in x array
*/
int countNaNs(float *x, int n);

/*
Remove  NAN values from a 1d array.
*/
int removeNaNFromArr(float *x, int n, float *xn);
	
/*
Function to perform ascending sort 
*/
void ascenSort(double *arr, int n); 
void ascenSortInt(int *arr, int n); 

/*
Check if a array is sorted ascending
*/
int isAscenSort(double *arr, int n);

/*
Function to perform descending sort 
*/
void descenSort(double *arr, int n); 

/*
Return the number of unique values in x array.
*/
int nUniqueInt(int *x, int n);

/* 
Return an array x2 with unique values from array x1
*/
int uniqueInt(int *x1, int n, int *x2);

/* 
Fill in NaN values in an array using linear interpolation
*/
float *fillInNaNLinInt(float *x, float *y, unsigned int n);
	
/* 
Return a 1d array y2 with x2 values interpolated based on
x1 and y1 1d arrays.
*/
int linInterp(double *x1, double *y1, int n1, double *x2, double *y2, int n2);

/*
Return the day of the year [1-365/366] given year, month and day
*/
int dayOfYear(struct tm da);

/*
Create a datetime array [YYYY MM DD HH MM SS] based on a starting and end struct tm
*/
struct tm *dateTime(struct tm *d1, struct tm *d2, int *ndays);

/* 
Get the min struct tm of a vector of struc tm's
*/
struct tm minDateTime(struct tm *dt, int n);

/* 
Get the max struct tm of a vector of struc tm's
*/
struct tm maxDateTime(struct tm *dt, int n);
	
/*For calculating Determinant of the Matrix */
double determinant(double a[NROWS][NCOLS], float k);

/*Finding transpose of matrix*/ 
void transpose(double num[NROWS][NCOLS], double fac[NROWS][NCOLS], float r, double **inverse);

void cofactor(double num[NROWS][NCOLS], float f, double **inverse);

/*
Evaluation of the Gamma function using the Lanczos approximation
*/
double lacz_gamma(double a);

/*
Evaluation of the Gamma function using the Spouge approximation
*/
double sp_gamma(double z);

/*
Build a sequence of numbers x from x1 to x2 every dx
*/
float *range(float x1, float x2, float dx);

/*
Build a sequence of n numbers in x from x1 to x2 equally spaced
*/
float *rangen(float x1, float x2, int n);

/*
Factorial of a number n
*/
long fact(int n);

/*
Standarization of random variable.
*/
float standarization(float x, float mi, float sigma);

/*
Absolute value of a double
*/
double signl(double x);

/*
Concatenate an array of int into a string separated by a char
*/
char *concInts2string(int *x, int n, char *s);
#endif
