#include "utils.h"

/* 
Allocate memory for a float  m[nrows][ncols] matrix
*/
float **allocfmat(int nrows, int ncols){
	int i;

	float **m = (float **)malloc(nrows * sizeof(float*));
	for(i = 0; i < nrows; i++){
		m[i] = (float *)malloc(ncols * sizeof(float));
	}
	return m;
}

/* 
Deallocate memory for a float  m[nrows][ncols] matrix
*/
int deallocfmat(float **m, int nrows){
	int i;

	for(i = 0; i < nrows; i++){
		free(m[i]);
	}
	free(m);
	return 0;
}

/* 
Allocate memory for a double m[nrows][ncols] matrix
*/
double **allocdmat(int nrows, int ncols){
	int i;

	double **m = (double **)malloc(nrows * sizeof(double*));
	for(i = 0; i < nrows; i++){
		m[i] = (double *)malloc(ncols * sizeof(double));
	}
	return m;
}

/* 
Deallocate memory for a double m[nrows][ncols] matrix
*/
int deallocdmat(double **m, int nrows){
	int i;

	for(i = 0; i < nrows; i++){
		free(m[i]);
	}
	free(m);
	return 0;
}

/* 
Allocate memory for a int  m[nrows][ncols] matrix
*/
int **allocimat(int nrows, int ncols){
	int i;

	int **m = (int **)malloc(nrows * sizeof(int*));
	for(i = 0; i < nrows; i++){
		m[i] = (int *)malloc(ncols * sizeof(int));
	}
	return m;
}

/* 
Deallocate memory for a int m[nrows][ncols] matrix
*/
int deallocimat(int **m, int nrows){
	int i;

	for(i = 0; i < nrows; i++){
		free(m[i]);
	}
	free(m);
	return 0;
}

/*
Multiplication of float 2d array and 1d array
*/
float *fMatVecMult(float **m, float *v, int nrows, int ncols){
	int i, j;

	float *r = (float *)malloc(nrows*sizeof(float));
	for (i = 0; i < nrows; i++){
		r[i] = 0;
		for (j = 0; j < ncols; j++){
			r[i] += m[i][j] * v[j];
		}
	}
	return r;
}

/*
Multiplication of double 2d array and 1d array
*/
double *dMatVecMult(double **m, double *v, int nrows, int ncols){
	int i, j;

	double *r = (double *)malloc(nrows*sizeof(double));
	for (i = 0; i < nrows; i++){
		r[i] = 0;
		for (j = 0; j < ncols; j++){
			r[i] += m[i][j] * v[j];
		}
	}
	return r;
}

/*
Count number of rows in a file
*/
int nrowsFile(char *filename)
{
	FILE *fp;
	char ch;
	int nlines = 0;

	if ((fp = fopen(filename,"r")) == NULL){
		printf("Error!");
		return -1;
	
	}

	while((ch=fgetc(fp))!=EOF) {
		if(ch=='\n')
			nlines++;
	}
	fclose(fp);
	return nlines;
}

/*
Count number of rows in a bin file
*/
int nrowsBinFile(char *filename)
{
	FILE *fp;
	csvd row;
	int nlines = 0;

	/* Open bin file */
	if ((fp = fopen(filename,"rb")) == NULL){
		printf("%s\n",filename);
		printf("Error opening %s stopping... \n", filename);
		return -1;
	}

	/* Counting lines */
	while (true){
    	fread(&row, sizeof(csvd), 1, fp);

		if (feof(fp))
			break;

		nlines++;
	}
	
	fclose(fp);
	return nlines;
}


/*
//Read the time series from a csv file
//
//int readFile(char *filename, tser *ts)
//{
//	FILE *fp;
//
//
//	if ((fp = fopen(filename,"r")) == NULL){
//		printf("Error reading %s stopping... \n", filename);
//		exit(1);
//	
//	}
//	printf("Reading file\n");
//	int i = 0;
//	char line[MAXCHAR];
//	char *token;
//	char seps[]=",";
//	
//	fgets(line, MAXCHAR, fp); 
//	char *newline;
//	while (fgets(line, MAXCHAR, fp) != NULL){
//		newline = strchr(line, '\n' );
//		if ( newline )
//			*newline = 0;
//		token = strtok(line,seps);
//		ts[i].dt.tm_year = atoi(token);
//		token = strtok(NULL,",");
//		ts[i].dt.tm_mon = atoi(token);
//		token = strtok(NULL,",");
//		ts[i].dt.tm_mday = atoi(token);
//		token = strtok(NULL,",");
//		if (strcmp(token, " ")==0){
//			ts[i].var = NAN;
//		}else{
//			ts[i].var = atof(token);
//		}
//			
//		token = strtok(NULL,",");
//		//strcpy(ts->var_symbol[i], token);
//		token = strtok(NULL,",");
//
//		printf("%d,%d,%d,%g\n", ts[i].dt.tm_year, ts[i].dt.tm_mon, ts[i].dt.tm_mday, ts[i].var);
//	
//		//if (i == 80){
//			break;
//		}//
//
//		i++;
//	}
//	fclose(fp);
//	return 0;
}*/


/*
Concatenate an array of int into a string separated by a char
*/
char *concInts2string(int *x, int n, char *s){
	int i, l;
	char *str;
	char in[10];

	l = 0;
	for(i = 0; i < n; i++){
		sprintf(in, "%d", x[i]);
		l += strlen(in);
	}
	str = (char *)malloc(l*sizeof(char) + strlen(s)*sizeof(char));

	sprintf(in, "%d", x[0]);
	strcpy(str, in);
	for(i = 1; i < n; i++){
		strcat(str, s);
		sprintf(in, "%d", x[i]);
		strcat(str, in);
	}
	
	return str;
}

/*
Return the max. val of the x array.
*/
float maxval(float *x, int n){
 	int i;
 	float max = x[0];
	for(i = 1; i < n; i++) {
 		if ( x[i] > max ){
			max = x[i]; 
		}
	} 
	return max;	
}

/*
Return the position of max. val in the x array.
*/
int imaxval(float *x, int n){
 	int i;
 	float max = x[0];
	int imax = 0;
	for(i = 1; i < n; i++) {
 		if ( x[i] > max ){
			max = x[i]; 
			imax = i;
		}
	} 
	return imax;	
}

/*
Return the min. val of the x array.
*/
float minval(float *x, int n){
 	int i;
 	float min = x[0];
	for(i = 1; i < n; i++) {
 		if ( x[i] < min ){
			min = x[i]; 
		}
	} 
	return min;	
}

/*
Return the position of min. val in the x array.
*/
int iminval(float *x, int n){
 	int i;
 	float min = x[0];
	int imin = 0;
	for(i = 1; i < n; i++) {
 		if ( x[i] > min ){
			min = x[i]; 
			imin = i;
		}
	} 
	return imin;	
}

/*
Count the number of values != NAN in x array
*/
int countNoNaNs(float *x, int n){
	int i, j;

	j = 0;
	for (i = 0; i < n; i++){
		if (!isnan(x[i]))
			j++;	
	}
	return j;
}

/*
Count the number of values = NAN in x array
*/
int countNaNs(float *x, int n){
	int i, j;
	
	j = 0;
	for (i = 0; i < n; i++){
		if (isnan(x[i]))
			j++;	
	}
	return j;
}

/*
Remove  NAN values from a 1d array.
*/
int removeNaNFromArr(float *x, int n, float *xn){
	int i, j;

	j = 0;
	for (i = 0; i < n; i++){
		if (!isnan(x[i])){
			xn[j] = x[i];	
			j++;
		}
	}
	return 0;
}

/*valpos minval(arr1d *x){
	valpos min;
	int i;
	min.val = x->a[0];
	min.pos = 0;
	for(i = 1; i < x->n; i++) {
		if ( x->a[i] < min.val){
			min.val = x->a[i];
			min.pos = i;
		}
	} 
	return min;	
}*/

/*
Swat values in a memory position
*/
static void swapInt(int *xp, int *yp){ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
/*
Function to perform ascending sort 
*/
void ascenSortInt(int *arr, int n){ 
    int i, j, min_idx; 
  
    /* One by one move boundary of unsorted subarray */
    for (i = 0; i < n - 1; i++) { 
  
        /* Find the minimum element in unsorted array */
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j] < arr[min_idx]) 
                min_idx = j; 
  
        /* Swap the found minimum element 
         with the first element */
        swapInt(&arr[min_idx], &arr[i]); 
    } 
}

/*
Swat values in a memory position
*/
static void swap(double *xp, double *yp){ 
    double temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
/*
Function to perform ascending sort 
*/
void ascenSort(double *arr, int n){ 
    int i, j, min_idx; 
  
    /* One by one move boundary of unsorted subarray */
    for (i = 0; i < n - 1; i++) { 
  
        /* Find the minimum element in unsorted array */ 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j] < arr[min_idx]) 
                min_idx = j; 
  
        /* Swap the found minimum element 
         with the first element */
        swap(&arr[min_idx], &arr[i]); 
    } 
}

/*
Check if a array is sorted ascending
*/
int isAscenSort(double *arr, int n){

	int i;
	for(i = 0; i < n-1; i++){
		if (arr[i+1]<arr[i]){
			return 0;
		}
	}
	return 1;
}

/*
Function to perform descending sort 
*/
void descenSort(double *arr, int n) 
{ 
    int i, j, max_idx; 
  
    /* One by one move boundary of unsorted subarray */
    for (i = 0; i < n - 1; i++) { 
  
        /* Find the minimum element in unsorted array */
        max_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j] > arr[max_idx]) 
                max_idx = j; 
  
        /* Swap the found minimum element 
         with the first element */
        swap(&arr[max_idx], &arr[i]); 
    } 
}

/*
Return the number of unique values in x array.
*/
int nUniqueInt(int *x, int n){

	int i, j, k; 

	/*for (i = 0; i < x->n; i++){
		if (!isnan(x->a[i])){
			dummy = x->a[i];
			j++;
			break;
		}
	}
	if (j == 0)
		return j;*/

	k = 0;
	for (i = 0; i < n; i++){
		for (j = 0; j < i; j++){
			if (x[i] == x[j]){
				break;
			}
		}
		if (i == j)
			k++;
	}
	return k;
}

/* 
Return an array x2 with unique values from array x1
*/
int uniqueInt(int *x1, int n, int *x2){

	int i, j, k;
	
	/* *n2 = nUniqueInt(x1,n1);
	x2 = (int *)malloc(*n2 * sizeof(int));*/
	/*for (i = 0; i < x->n; i++){
		if(isnan(x->a[i])){
			y->a[j] = x->a[i];
			j++;
			break;
		}
	}*/
	/*x2[0] = x1[0];
	//j = 1; 
	//for (i = 1; i < n; i++){
	//	if (x2[j-1] != x1[i]){
	//		x2[j] = x1[i];
	//		j++;
	//	}
	}*/
	k = 0;
	for (i = 0; i < n; i++){
		for (j = 0; j < i; j++){
			if (x1[i] == x1[j]){
				break;
			}
		}
		if (i == j){
			x2[k] = x1[i];
			k++;
		}
	}

	return 0;
}

int linInterp(double *x1, double *y1, int n1, double *x2, double *y2, int n2){

	int i, j;
	float dx, m;

/*	for(i = 0; i < n2; i++){
//		for(j = 0; j < n1; j++)
//			if (x2[i] < x1[j] || x2[i] == x1[j] || x2[i] > x1[j]){
//				y2[i] = y1[j];
//				break;
//			}else{
//				m = (y1[j+1]-y1[j])/(x1[j+1]-x1[j]);
//				dx = x2[i]-x2[i-1]; 
//				y2[i] = y2[i-1] + m*dx; 
//				break;
//			}
	}*/
			
	
/*	dx = x2[1]-x2[0]; */

	for(i = 0; i < n2; i++){
		if (x2[i] < x1[0]){
			y2[i] = y1[0];
		}else if (x2[i] > x1[n1-1]){
			y2[i] = y1[n1-1];
		}else{
			for(j = 0; j < n1; j++){
				if (x2[i] == x1[j]){
					y2[i] = y1[j];
					break;
				}
				if (x2[i]>x1[j] && x2[i]<x1[j+1]){
					m = (y1[j+1]-y1[j])/(x1[j+1]-x1[j]);
					dx = x2[i]-x2[i-1]; 
					y2[i] = y2[i-1] + m*dx; 
					break;
				}
			}
		}
	}
	
	return 0;
}

/*
Return the day of the year [1-365/366] given year, month and day
*/
int dayOfYear(struct tm da){
	int i;
	struct tm di = { 0 };
	struct tm de = { 0 };
	time_t si, sa, si0;	
		
	da.tm_year -= 1900;
	sa = mktime(&da);

	di.tm_year = da.tm_year;
	di.tm_mon = 0;
	di.tm_mday = 1;
	/*di.tm_hour = 0;*/
	/*di.tm_min = 0;*/
	/*di.tm_sec = 0;*/
	si0 = mktime(&di);
	i = 0;
	while(i < 366){
		si = si0 + 86400*i;
		de = *gmtime(&si);
		de.tm_hour = 0;
		de.tm_min = 0;
		de.tm_sec = 0;
		/*if((da.tm_year+1900 == de.tm_year) && (da.tm_mon == de.tm_mon) && (da.tm_mday == de.tm_mday))*/
		if(sa == mktime(&de))
			return ++i;
		i++;	
	}
	return 0;
}

/*For calculating Determinant of the Matrix */
double determinant(double a[NROWS][NCOLS], float k)
{
  double s = 1, det = 0, b[NROWS][NCOLS];
  int i, j, m, n, c;

  if (k == 1)
    {
     return (a[0][0]);
    }
  else
    {
     det = 0;
     for (c = 0; c < k; c++)
       {
        m = 0;
        n = 0;
        for (i = 0;i < k; i++)
          {
            for (j = 0 ;j < k; j++)
              {
                b[i][j] = 0;
                if (i != 0 && j != c)
                 {
                   b[m][n] = a[i][j];
                   if (n < (k - 2))
                    n++;
                   else
                    {
                     n = 0;
                     m++;
                     }
                   }
               }
             }
          det = det + s * (a[0][c] * determinant(b, k - 1));
          s = -1 * s;
          }
    }

    return (det);
}

/*Finding transpose of matrix*/ 
void transpose(double num[NROWS][NCOLS], double fac[NROWS][NCOLS], float r, double **inverse)
{
  int i, j;
  double b[NROWS][NCOLS],  d;

  for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
         b[i][j] = fac[j][i];
        }
    }
  d = determinant(num, r);
  for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
        inverse[i][j] = b[i][j] / d;
        }
    }
}

void cofactor(double num[NROWS][NCOLS], float f, double **inverse)
{

 double b[NROWS][NCOLS], fac[NROWS][NCOLS];
 int p, q, m, n, i, j;

 for (q = 0;q < f; q++)
 {
   for (p = 0;p < f; p++)
    {
     m = 0;
     n = 0;
     for (i = 0;i < f; i++)
     {
       for (j = 0;j < f; j++)
        {
          if (i != q && j != p)
          {
            b[m][n] = num[i][j];
            if (n < (f - 2))
             n++;
            else
             {
               n = 0;
               m++;
               }
            }
        }
      }
      fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
    }
  }
  transpose(num, fac, f, inverse);
}


/*
Evaluation of the Gamma function using the Lanczos approximation
*/
double lacz_gamma(double a){
 
    const int cg = 7;
	double t, w, x;
    int i;
	/* These precomputed values are taken by the sample code in Wikipedia,
     and the sample itself takes them from the GNU Scientific Library */
    const double p[] = {0.99999999999980993, 676.5203681218851, -1259.1392167224028, \
         771.32342877765313, -176.61502916214059, 12.507343278686905, \
         -0.13857109526572012, 9.9843695780195716e-6, 1.5056327351493116e-7};
 
    x = a;
 
    if (x < 0.5){
		return M_PI/(sin(M_PI*x) * lacz_gamma(1.0-x));
    }else{
		x = x - 1.0;
		t = p[0];
		for(i = 1; i < cg+2; i++)
			t += p[i]/(x+(float)i);
		w = x + (float)cg + 0.5;
		return sqrt(2.0*M_PI) * pow(w,x+0.5) * exp(-w) * t;
	}
}

/*
Evaluation of the Gamma function using the Spouge approximation
*/
double sp_gamma(double z)
{
  const int a = A;
  static double c_space[A];
  static double *c = NULL;
  int k;
  double accm;
 
  if ( c == NULL ) {
    double k1_factrl = 1.0; /* (k - 1)!*(-1)^k with 0!==1*/
    c = c_space;
    c[0] = sqrt(2.0*M_PI);
    for(k=1; k < a; k++) {
      c[k] = exp(a-k) * pow(a-k, k-0.5) / k1_factrl;
	  k1_factrl *= -k;
    }
  }
  accm = c[0];
  for(k=1; k < a; k++) {
    accm += c[k] / ( z + k );
  }
  accm *= exp(-(z+a)) * pow(z+a, z+0.5); /* Gamma(z+1) */
  return accm/z;
}

/*
Build a sequence of numbers x from x1 to x2 every dx
*/
float *range(float x1, float x2, float dx){
	int i;
	float *x;
	int n; 

	n = (int )round((x2 - x1)/dx) + 1;
	printf("n = %d\n",n);

	x = (float *)malloc(n*sizeof(float));
	x[0] = x1;
	for(i = 1; i < n; i++)
		x[i] = x[i-1] + dx;

	return x;
}

/*
Build a sequence of n numbers in x from x1 to x2 equally spaced
*/
float *rangen(float x1, float x2, int n){
	int i;
	float *x;
	float dx = (x2 - x1)/(n-1);
	printf("dx = %f\n",dx);

	x = (float *)malloc(n*sizeof(float));
	x[0] = x1;
	for(i = 1; i < n; i++)
		x[i] = x[i-1] + dx;

	return x;
}
/*
Factorial of a number n
*/
long fact(int n){
	if (n == 0)
		return 1;
	else
		return n*fact(n-1);
}

/*
Standarization of random variable.
*/
float standarization(float x, float mi, float sigma){
	return (x-mi)/sigma;
}

/*
Absolute value of a double
*/
double signl(double x){
	if (x>=0.) 
		return x;
	else
		return -1*x;
}

/*
Estimate of locatization, scale and shape parameters based on the moments.
*/
/*pdpar methodOfMoments(float *x, int n){
	pdpar pdp;
	pdp.loc = mean(x,n);
	pdp.scale = stdDev(x,n);
	pdp.shape = 0;
	return pdp;
}*/

/*
Fit data in x to a distribution
*/
/*int fitData(float *x, int n, int nn, float (*pdf)(pdpar *, float), float (*cdf)(pdpar *,float)){
	pdpar pdp;
	float minx, maxx, dx;
	float xn[nn];

	methodOfMoments(x, n, &pdp);
	
	minx = minval(x,n);
	maxx = maxval(x,n);
	dx = (maxval - minval)/(nn-1);
	xn[0] = minx;
	for(i = 1; i < nn; i++)
		xn[i] += dx
	
	for(i = 0; i < nn; i++){
		pdf[i] = pdf(&pdp, xn[i]);
		cdf[i] = cdf(&pdp, xn[i]);
	}
	return 0;
}*/


