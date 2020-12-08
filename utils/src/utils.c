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

//*
//Read the time series from a csv file
//*/
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
//	/*char *token;*/
//	int i = 0;
//	char line[MAXCHAR];
//	char *token;
//	char seps[]=",";
//	
//	/* Read the header's file*/
//	fgets(line, MAXCHAR, fp); 
//	/*printf("%s",line);*/
//	char *newline;
//	while (fgets(line, MAXCHAR, fp) != NULL){
//		/*printf("%s",line);*/
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
//		/*if (i == 80){
//			break;
//		}*/
//
//		i++;
//	}
//	fclose(fp);
//	return 0;
//}


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
		//printf("strlen=%d\n",l); 
	}
	str = (char *)malloc(l*sizeof(char) + strlen(s)*sizeof(char));

	sprintf(in, "%d", x[0]);
	strcpy(str, in);
	for(i = 1; i < n; i++){
		strcat(str, s);
		sprintf(in, "%d", x[i]);
		strcat(str, in);
	}
	//printf("%s\n",str);
	
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
	int j = 0;
	for (int i = 0; i < n; i++){
		if (!isnan(x[i]))
			j++;	
	}
	return j;
}

/*
Count the number of values = NAN in x array
*/
int countNaNs(float *x, int n){
	int j = 0;
	for (int i = 0; i < n; i++){
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

//valpos minval(arr1d *x){
//	valpos min;
//	int i;
//	min.val = x->a[0];
//	min.pos = 0;
//	for(i = 1; i < x->n; i++) {
//		if ( x->a[i] < min.val){
//			min.val = x->a[i];
//			min.pos = i;
//		}
//	} 
//	return min;	
//}

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
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n - 1; i++) { 
  
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j] < arr[min_idx]) 
                min_idx = j; 
  
        // Swap the found minimum element 
        // with the first element 
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
  
//		printf("%d,%d,%d,%g\n", ts[i].dt.tm_year, ts[i].dt.tm_mon, ts[i].dt.tm_mday, ts[i].var);
//	
//		/*if (i == 80){
//			break;
//		}*/
//
//		i++;
//	}
//	fclose(fp);
//	return 0;
//}


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
		//printf("strlen=%d\n",l); 
	}
	str = (char *)malloc(l*sizeof(char) + strlen(s)*sizeof(char));

	sprintf(in, "%d", x[0]);
	strcpy(str, in);
	for(i = 1; i < n; i++){
		strcat(str, s);
		sprintf(in, "%d", x[i]);
		strcat(str, in);
	}
	//printf("%s\n",str);
	
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
	int j = 0;
	for (int i = 0; i < n; i++){
		if (!isnan(x[i]))
			j++;	
	}
	return j;
}

/*
Count the number of values = NAN in x array
*/
int countNaNs(float *x, int n){
	int j = 0;
	for (int i = 0; i < n; i++){
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

//valpos minval(arr1d *x){
//	valpos min;
//	int i;
//	min.val = x->a[0];
//	min.pos = 0;
//	for(i = 1; i < x->n; i++) {
//		if ( x->a[i] < min.val){
//			min.val = x->a[i];
//			min.pos = i;
//		}
//	} 
//	return min;	
//}

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
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n - 1; i++) { 
  
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j] < arr[min_idx]) 
                min_idx = j; 
  
        // Swap the found minimum element 
        // with the first element 
        swapInt(&arr[min_idx], &arr[i]); 
    } 
}

/*
Swat values in a memory position
*/
//		/*if (i == 80){
//			break;
//		}*/
//
//		i++;
//	}
//	fclose(fp);
//	return 0;
//}


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
		//printf("strlen=%d\n",l); 
	}
	str = (char *)malloc(l*sizeof(char) + strlen(s)*sizeof(char));

	sprintf(in, "%d", x[0]);
	strcpy(str, in);
	for(i = 1; i < n; i++){
		strcat(str, s);
		sprintf(in, "%d", x[i]);
		strcat(str, in);
	}
	//printf("%s\n",str);
	
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
	int j = 0;
	for (int i = 0; i < n; i++){
		if (!isnan(x[i]))
			j++;	
	}
	return j;
}

/*
Count the number of values = NAN in x array
*/
int countNaNs(float *x, int n){
	int j = 0;
	for (int i = 0; i < n; i++){
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

//valpos minval(arr1d *x){
//	valpos min;
//	int i;
//	min.val = x->a[0];
//	min.pos = 0;
//	for(i = 1; i < x->n; i++) {
//		if ( x->a[i] < min.val){
//			min.val = x->a[i];
//			min.pos = i;
//		}
//	} 
//	return min;	
//}

/*
Swat values in a memory position
*/
static void swapInt(int *xp, int *yp){ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
//	}
//	fclose(fp);
//	return 0;
//}


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
		//printf("strlen=%d\n",l); 
	}
	str = (char *)malloc(l*sizeof(char) + strlen(s)*sizeof(char));

	sprintf(in, "%d", x[0]);
	strcpy(str, in);
	for(i = 1; i < n; i++){
		strcat(str, s);
		sprintf(in, "%d", x[i]);
		strcat(str, in);
	}
	//printf("%s\n",str);
	
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
	int j = 0;
	for (int i = 0; i < n; i++){
		if (!isnan(x[i]))
			j++;	
	}
	return j;
}

/*
Count the number of values = NAN in x array
*/
int countNaNs(float *x, int n){
