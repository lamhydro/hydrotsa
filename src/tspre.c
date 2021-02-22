#include "utils.h"
#include "tspre.h"

/*
/Read the time series from a csv file
*/
int readFile(char *filename, tserie *ts)
{
	FILE *fp;
	int i = 0;
	char line[MAXCHAR];
	char *token;
	char seps[]=",";
	char *newline;

	if ((fp = fopen(filename,"r")) == NULL){
		printf("Error reading %s stopping... \n", filename);
		exit(1);
	}
	
	/* Read the header's file*/
	fgets(line, MAXCHAR, fp); 
	
	while (fgets(line, MAXCHAR, fp) != NULL){
		/*printf("%s",line);*/
		newline = strchr(line, '\n' );
		if ( newline )
			*newline = 0;
		token = strtok(line,seps);
		ts->year[i] = atoi(token);
		token = strtok(NULL,",");
		ts->month[i] = atoi(token);
		token = strtok(NULL,",");
		ts->day[i] = atoi(token);
		token = strtok(NULL,",");
		if (strcmp(token, " ")==0){
			ts->var[i] = NAN;
		}else{
			ts->var[i] = atof(token);
		}
			
		token = strtok(NULL,",");
		/*strcpy(ts->var_symbol[i], token);*/
		token = strtok(NULL,",");

		/*printf("%d,%d,%d,%g,%s\n", ts->year[i], ts->month[i], ts->day[i], ts->var[i], ts->var_symbol[i]);*/
		/*printf("%d,%d,%d,%g\n", ts->year[i], ts->month[i], ts->day[i], ts->var[i]);*/
	
		/*if (i == 80){
			break;
		}*/

		i++;
	}
	fclose(fp);
	return 0;
}

/*
Read and display bin file with tserie
*/
int readBinFile(char *filename, tserie *ts){

	FILE *fp;
	unsigned i;
	csvd row;

	/* Open bin file */
	if ((fp = fopen(filename,"rb")) == NULL){
		printf("Error opening %s stopping... \n", filename);
		return -1;
	}

	/* Read main table */
	i = 0;
	while (true){
    	fread(&row, sizeof(csvd), 1, fp);

		ts->year[i] = row.year;
		ts->month[i] = row.month;
		ts->day[i] = row.day;
		ts->var[i] = row.var;

		if (feof(fp))
			break;

		/* Print each element of the object */
    	/*printf("%d, %d, %d, %f\n", ts->year[i], ts->month[i], ts->day[i], ts->var[i]);*/
		i++;
	}

    /* Close the file */
    fclose(fp);
    return 0;
}

/*
Read time series from csv file
*/
/*int readTSfromFile(tfile *tsf, tserie *ts){*/
tserie *readTSfromFile(tfile *tsf){
	tserie *ts;
	char *filename;

	/* Make up filename */
	if ((filename = malloc(strlen(tsf->dirname) + strlen(tsf->filename) + 1)) == NULL){
		exit(0);
	}
	strcpy(filename, tsf->dirname);
	strcat(filename, tsf->filename);

	/* Number of file rows */
	tsf->nrows = nrowsBinFile(filename); 

	/*printf("nrows = %d\n", tsf->nrows);*/	
	/* Reading ts from csv file */ 
	/*allocMemTs(ts, tsf->nrows);*/  /* -1 to remove header in 1rs row */ 
	ts = allocMemTs(tsf->nrows);  /* -1 to remove header in 1rs row */ 
	printf("Reading %s ts file with %d rows.\n", filename, tsf->nrows);
	
	/* Read time series from ascii file */
	readBinFile(filename, ts);
	/*printf("ts->n = %d\n", ts->n);*/
	/*readFile(filename, ts);*/

	free(filename);

	return ts;
}


/*
Save a tserie struct into a csv file
*/
int writeTserie2csv(tfile *tsf, tserie *ts){ 
	int i;
	FILE *fp;
	char *filename;
	char *str;
	int x[3];  

	/* Make up filename */
	if ((filename = malloc(strlen(tsf->dirname) + strlen(tsf->filename) + 1)) == NULL){
		return -1;
	}
	strcpy(filename, tsf->dirname);
	strcat(filename, tsf->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"w")) == NULL){
		printf("Error writing %s stopping... \n", filename);
		return -1;
	}
	
	/*printf("%s\n",concInts2string(x, 3, "-"));*/
	/*fprintf(fp,"%s,%s,%s,%s\n","YEAR", "MONTH", "DAY", "VAR");*/
	for(i = 0; i < ts->n; i++){
		x[0]=ts->year[i];
		x[1]=ts->month[i];
		x[2]=ts->day[i];
		str = concInts2string(x, 3, "-");
		fprintf(fp,"%s,%f\n",str, ts->var[i]);
		free(str);
		/*fprintf(fp,"%d,%d,%d,%f\n",ts->year[i], ts->month[i], ts->day[i], ts->var[i]);*/
	}	

	free(filename);

	return 0;
}

/*
Save a tserie struct into a binary file
*/
int writeTserie2bin(tfile *tsf, tserie *ts){ 
	unsigned i;
	FILE *fp;
	char *filename;
	int tint;
	double tdoub;

	/* Make up filename */
	filename = (char *)malloc(strlen(tsf->dirname)*sizeof(char *) + strlen(tsf->filename)*sizeof(char *));
	strcpy(filename, tsf->dirname);
	strcat(filename, tsf->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"wb")) == NULL){
		printf("Error writing %s stopping... \n", filename);
		return -1;
	}
	
	/*printf("%s\n",concInts2string(x, 3, "-"));*/
	/*fprintf(fp,"%s,%s,%s,%s\n","YEAR", "MONTH", "DAY", "VAR");*/
	for(i = 0; i < ts->n; i++){
		printf("%d,%d,%d,%g\n", ts->year[i], ts->month[i], ts->day[i], ts->var[i]);
		tint = ts->year[i];
		fwrite( &tint, sizeof(int), 1, fp );
		tint = ts->month[i];
		fwrite( &tint, sizeof(int), 1, fp );
		tint = ts->day[i];
		fwrite( &tint, sizeof(int), 1, fp );
		tdoub = ts->var[i];
		fwrite( &tdoub, sizeof(double), 1, fp );
	}	

	fclose(fp);
	free(filename);

	return 0;
}

/*
Allocate heap memory of an struct tserie
*/
/*void allocMemTs2(tserie *ts, int n){
	ts = (tserie *)malloc(sizeof(tserie *));
	ts->n = n;
	ts->year = (int *)malloc(n * sizeof(int *));
	ts->month= (int *)malloc(n * sizeof(int *));
	ts->day = (int *)malloc(n * sizeof(int *));
	ts->var = (float *)malloc(n * sizeof(float *));*/
	/*(*ts)->n = n;*/
	/*ts->var_symbol = (char *)malloc(ts->n * sizeof(char));*/
	/*for (int i = 0; i < ts->n; i++)*/
	/*	ts->var_symbol[i] = malloc(5 * sizeof(char));*/
/*}*/

tserie *allocMemTs(int n){
	tserie *ts;

	ts = (tserie *)malloc(sizeof(tserie));
	if (ts == NULL)
		return NULL;

	ts->year = (int *)malloc(n * sizeof(int *));
	if (ts->year == NULL){
		free(ts->year);
		return NULL;
	}

	ts->month= (int *)malloc(n * sizeof(int *));
	if (ts->month == NULL){
		free(ts->month);
		return NULL;
	}

	ts->day = (int *)malloc(n * sizeof(int *));
	if (ts->day == NULL){
		free(ts->day);
		return NULL;
	}

	ts->var = (float *)malloc(n * sizeof(float *));
	if (ts->var == NULL){
		free(ts->var);
		return NULL;
	}

	ts->n = n;
	return ts;
}


/*
Free heap memory taken by tserie struct
*/
void freeMemTs(tserie *ts){
	if (ts != NULL){
		free(ts->year);
		free(ts->month);
		free(ts->day);
		free(ts->var);
		free(ts);
	}
	/*for (int i = 0; i < ts->n; i++){*/
	/*	free(ts->var_symbol[i]);*/
	/*}*/	
}

/*
Remove rows that containt NAN values from  ts struct.
*/
int removeNaNFromTs1(tserie *ts1, tserie *ts2){
	int j, i;
	/*ts2->n = countNoNaNs(ts1->var, ts1->n);*/
	/*allocMemTs(ts2, ts2->n);*/
	j = 0;
	for (i = 0; i < ts1->n; i++){
		/*if (!isnan(ts1->var[i])){*/
		if (ts1->var[i] != NAN){
			ts2->year[j] = ts1->year[i];
			ts2->month[j] = ts1->month[i];
			ts2->day[j] = ts1->day[i];
			ts2->var[j] = ts1->var[i];
			/*ts2->var_symbol[j] = ts1->var_symbol[j];*/	
			j++;
		}
	}
	return 0;
}

/*
Remove rows that containt NAN values from  ts1 struct. Return an array of ts2 ctserie struct and its length n2.
*/
ctserie *removeNaNFromTs2(ctserie *ts1, int n1, int *n2){
	int j, i;
	ctserie *ts2;
	float *var;
	
	/* Count the number of nonans */
	var = malloc(n1 * sizeof(float));
	for (i = 0; i < n1; i++){
		var[i] = ts1[i].var;
	}
	*n2 = countNoNaNs(var, n1);
	free(var);
	/*printf("nnonan = %d\n", nnonan);*/
	ts2 = malloc(*n2 * sizeof(ctserie));

	j = 0;
	for (i = 0; i < n1; i++){
		/*if (ts1[i].var != NAN){*/
		if (!isnan(ts1[i].var)){
			ts2[j].dt = ts1[i].dt;
			ts2[j].var = ts1[i].var; 
			j++;
		}
	}

	return ts2;
}

/*
Return a dtsinv structure with the inventory of a 
daily time series.
*/
int dayTSinventory(ctserie *ts, int n, dtsinv *dtsi){
	int i, j, k, nd, nm, ndy;
	
	i = 0;
	j = 0;
	k = 0;
	nd = 1;
	nm = 1;
	ndy = 1;
	dtsi[j].year = ts[i].dt.tm_year;
	dtsi[j].ndayspy = ndy;
	dtsi[j].month[k] = ts[i].dt.tm_mon;
	dtsi[j].nmonths = nm;
	dtsi[j].dayspm[k] = nd;
	for (i = 1; i < n; i++){
		if (dtsi[j].year == ts[i].dt.tm_year){
			if (dtsi[j].month[k] == ts[i].dt.tm_mon){
				nd++;
			}else{
				dtsi[j].dayspm[k] = nd;
				k++;
				nm++;
				dtsi[j].nmonths = nm;
				dtsi[j].month[k] = ts[i].dt.tm_mon;
				nd = 1;
			}
			ndy++;
		}else{
			dtsi[j].dayspm[k] = nd;
			dtsi[j].ndayspy = ndy;
			j++;
			k = 0;
			nd = 1;
			nm = 1;
			ndy = 1;
			dtsi[j].year = ts[i].dt.tm_year;
			dtsi[j].month[k] = ts[i].dt.tm_mon;
			dtsi[j].nmonths = nm;
		}
	}
	dtsi[j].dayspm[k] = nd;
	dtsi[j].ndayspy = ndy;

	return 0;
}

int makeUpSeqDayDates(struct tm *startdate,  int tdays, struct tm *daydates){

	time_t t2, t2i;
	int i;
	time_t one_day = 86400;
	/*struct tm *ptm;*/
	/*char buf[256] = {0};*/
	
	t2i = mktime(startdate);
	for(i = 0; i < tdays; i++){
		/*t2 += one_day;*/
		t2 = t2i + one_day*i;
		daydates[i] = *gmtime(&t2);  
		/* Set to hour = 0, min = 0 and sec = 0 as it usually 
		covert to UTC giving some hours e.g. 5hours ahead */
		daydates[i].tm_hour = 0;
		daydates[i].tm_min = 0;
		daydates[i].tm_sec = 0;
		/*daydates[i].tm_mon++;
		daydates[i] = *ptm;  = gmtime(&t2);  
		daydates[i].tm_year = ptm->tm_year;
		daydates[i].tm_mon = ptm->tm_mon;
		daydates[i].tm_mday = ptm->tm_mday;*/

		/*printf(" year: %d, month:%d, day:%d, hour:%d\n", daydates[i].tm_year, daydates[i].tm_mon, daydates[i].tm_mday, daydates[i].tm_hour);*/
		/*strftime(buf, 256, "%F", ptm);*/
		/*strftime(buf, 256, "%F", &daydates[i]);*/
		/*puts(buf);*/
	}
	return 0;	
}

/*
Switch negative values into nan in tserie struct
*/
int negativeToNanInTs(tserie *ts){
	int i;

	for(i = 0; i < ts->n; i++){
		if (ts->var[i] < 0)
			ts->var[i] = NAN;
	}
	return 0;
}

/*
Indicate if a year is a leap year.
*/
bool isLeapYear(int year) {

    if (year % 4 != 0) {
        
        return false;
    } else if (year % 400 == 0) {
        
        return true;
    } else if (year % 100 == 0) {
        
        return false;
    } else {
        
        return true;
    }
}

/*
Return the number of days in a year
*/
int nDaysYear(int year){
	if (isLeapYear(year)){ 
		return 366;		
	}else{
		return 365;		
	}
}

int sliceDayTimeSeries(struct tm *sdate, struct tm *edate, tserie *ts1, tserie *ts2){
/*tserie *sliceDayTimeSeries(struct tm *sdate, struct tm *edate, tserie *ts1){
	
//	int i, j, start;
//	struct tm *ta;
//	struct tm tb = {0};
//	float *var;
//
//	ta = (struct tm *)malloc(366 * sizeof(struct tm));
//	var = (float *)malloc(366 * sizeof(float));	
//	
//	j = 0;
//	start = 0;
//	for(i = 0; i < ts1->n; i++){
//		tb.tm_year = ts1->year[i]-1900;
//		tb.tm_mon = ts1->month[i]-1;
//		tb.tm_mday = ts1->day[i];
//		if (!start){
////			if (!strcmp(asctime(sdate),asctime(&tb))){
//			if (difftime(mktime(sdate),mktime(&tb))){
//				printf("%s", asctime(sdate));
//				printf("%s", asctime(&tb));
//				printf("strcmp %d\n",strcmp(asctime(sdate),asctime(&tb)));
//				start = 1;
//			}
//		}
//		if (start){
//			ta[j].tm_year = ts1->year[i];
//			ta[j].tm_mon = ts1->month[i];
//			ta[j].tm_mday = ts1->day[i];
//			var[j] = ts1->var[i];
//			j++;
//			//if ( !strcmp(asctime(edate),asctime(&tb)) ){
//			if (difftime(mktime(edate),mktime(&tb))){
//				printf("%s", asctime(edate));
//				printf("%s", asctime(&tb));
//				printf("strcmp %d\n",strcmp(asctime(edate),asctime(&tb)));
//				break;
//			}
//		}
//	}
//
//	printf("cccccccccccccccccccccccc %d\n",j);
//	allocMemTs(ts2,j);
//	for(i = 0; i < j; i++){
//		ts2->year[i] = ta[i].tm_year;
//		ts2->month[i] = ta[i].tm_mon;
//		ts2->day[i] = ta[i].tm_mday;
//		ts2->var[i] = var[i];
//	}	
//	free(ta);
//	free(var);
*/

	struct tm da ={0};
	int i, j;
	int ids = 0;
	int ide = 0;

	time_t st = mktime(sdate);
	time_t en = mktime(edate);
	time_t ta;
	
	j = 0;
	for(i = 0; i < ts1->n; i++){
		da.tm_year = ts1->year[i]-1900;
		da.tm_mon = ts1->month[i]-1;
		da.tm_mday = ts1->day[i];
		ta = mktime(&da);
		/*if (ta == st)*/
		if (difftime(ta,st)==0)	
			ids = i;
		/*if (ta == en)*/
		if (ta>=st && ta<=en)
			j++;
		if (difftime(ta, en)==0){
			ide = i;
			break;
		}
	}
	ts2 = allocMemTs(j);
	j = 0;
	for(i = ids; i <= ide; i++){
		ts2->year[j] = ts1->year[i];
		ts2->month[j] = ts1->month[i];
		ts2->day[j] = ts1->day[i];
		ts2->var[j] = ts1->var[i];
		j++;
	}
			
	return 0;
}

/*
Extract one year of a time series and store in a tserie struct.
*/
tserie *extractOneYearTSFromTS(tserie *ts1, int year){
	int i, j;
	tserie *ts2;
	int start, ids, ide;

	start = 0;
	ids = 0;
	ide = ts1->n-1;
	for(i = 0; i < ts1->n; i++){
		if ((ts1->year[i] == year) && !start){
			start = 1;
			ids = i;
		}
		if ((ts1->year[i] != year) && start){
			ide = i-1;
			break;
		}
	}
	ts2 = allocMemTs(ide-ids+1);
	j = 0;
	for(i = ids; i <= ide; i++){
		ts2->year[j] = ts1->year[i];
		ts2->month[j] = ts1->month[i];
		ts2->day[j] = ts1->day[i];
		ts2->var[j] = ts1->var[i];
		j++;
	}

	return ts2;	
}

/*
Extract one month of a time series and store in a tserie struct.
*/
int extractOneMonthTSFromTS(tserie *ts1, int year, int month, tserie *ts2){
	int i, j;
	int start, ids, ide;

	start = 0;
	ids = 0;
	ide = ts1->n-1;
	for(i = 0; i < ts1->n; i++){
		if ((ts1->year[i] == year) && (ts1->month[i] == month) && !start){
			start = 1;
			ids = i;
		}
		if ((ts1->year[i] != year) || (ts1->month[i] != month)){
			ide = i-1;
			break;
		}
	}
	ts2 = allocMemTs(ide-ids+1);
	j = 0;
	for(i = ids; i <= ide; i++){
		ts2->year[j] = ts1->year[i];
		ts2->month[j] = ts1->month[i];
		ts2->day[j] = ts1->day[i];
		ts2->var[j] = ts1->var[i];
		j++;
	}

	return 0;	
}

int sortTS(tserie *ts1, tserie *ts2){

	int i, j;
	time_t *x1, *x2;
	struct tm sd = { 0 };

	x1 = (time_t *)malloc(ts1->n * sizeof(time_t));
	x2 = (time_t *)malloc(ts1->n * sizeof(time_t));

	for(i = 0; i < ts1->n; i++){
		sd.tm_year = ts1->year[i];
		sd.tm_mon = ts1->month[i]-1;
		sd.tm_mday = ts1->day[i];
		x1[i] = mktime(&sd);
		x2[i] = x1[i];
	}
	
	if (!isAscenSort((double *)x2, ts1->n)){
		printf("no sorted!\n");
		ascenSort((double *)x2, ts1->n); 
		for(i = 0; i < ts1->n; i++){
			for(j = 0; j < ts1->n; j++){
				if(x2[i]==x1[j]){
					ts2->year[i] = ts1->year[j];
					ts2->month[i] = ts1->month[j];
					ts2->day[i] = ts1->day[j];
					ts2->var[i] = ts1->var[j];
					break;
				}
			}
		}
		
		free(x1);
		free(x2);
		return 0;
	}else{
		free(x1);
		free(x2);
		for(i = 0; i < ts1->n; i++){
			ts2->year[i] = ts1->year[i];
			ts2->month[i] = ts1->month[i];
			ts2->day[i] = ts1->day[i];
			ts2->var[i] = ts1->var[i];
		}		
		return 1;
	}
	
}


/* tserie *uniqueTS(tserie *ts){
	int i, j, k, l;
	tserie *ts1, *tsn;
*/	
	/* Allocate struct arrays */
	/*ts1 = allocMemTs(ts->n); */

	/* Check duplicate dates */
	/*l = 0;
	for (i = 0; i < ts->n; i++){
		accum = ts->var[i];
		k = 1;
		for (j = i+1; j < ts->n; j++){
			if(ts->year[i] == ts->year[j] &&
				ts->month[i] == ts->month[j] && 
				ts->day[i] == ts->day[j]){
				accum += ts->var[j]
				k++;
			}
		}
		if(k > 1){
		}else{
		}
		l = i-k-1
		ts1->year[l] = ts->year[i];
		ts1->month[l] = ts->month[i];
		ts1->day[l] = ts->day[i];
		ts1->var[l] = accum /= k;
	}*/
	
	/* Allocate struct arrays for unique ts */
/*	tsn = allocMemTs(ts->n-l);
	for (i = 0; i < ts->n; i++){
		tsn->year[i] = ts1->year[i];
		tsn->month[i] = ts1->month[i];
		tsn->day[i] = ts1->day[i];
		tsn->var[i] = ts1->var[i];
	}
	freeMemTs(ts1);

	return tsn;

}*/




/*
Map original ts onto a build time series using struct tm
*/
ctserie *matchingTs(tserie *ts1, int *n){

	ctserie *ts2;
	int i, j, k, n2;
	float accum, val;
	struct tm *dt1, *dt;
	struct tm mindt, maxdt;
	
	/* Set ts1 into struct tm */
	dt1 = calloc(ts1->n, sizeof(struct tm));
	for (i = 0; i < ts1->n; i++){
		/*dt1[i] = {0, 0, 0, ts1->day[i], ts1->month[i]-1, ts1->year[i]-IYEAR};*/
		dt1[i].tm_sec = 0;
		dt1[i].tm_min = 0;
		dt1[i].tm_hour = 0;
		dt1[i].tm_mday = ts1->day[i];
		dt1[i].tm_mon = ts1->month[i]-1;
		dt1[i].tm_year = ts1->year[i]-IYEAR;
	}

	/* Get the initial datetime */
	mindt = minDateTime(dt1, ts1->n);
	/*printf("UTC 1 time: %s", asctime(&mindt));*/

	/* Get the end datetime */
	maxdt = maxDateTime(dt1, ts1->n);
	/*printf("UTC 2 time: %s", asctime(&maxdt));*/
	free(dt1);

	/* Create a vector of struct tm */
	/*struct tm d1 = {0, 0, 0, ts1->day[0], ts1->month[0]-1, ts1->year[0]-IYEAR};
	printf("UTC 1 time: %s", asctime(&d1));
	struct tm d2 = {0, 0, 0, ts1->day[ts1->n-1], ts1->month[ts1->n-1]-1, ts1->year[ts1->n-1]-IYEAR};
	printf("UTC 2 time: %s", asctime(&d2));*/
	/*printf("ts1->n = %d\n",ts1->n);
	printf("ts2->n = %d\n",ts2->n);*/
	dt = dateTime(&mindt, &maxdt, &n2);
	*n = n2;
	/*printf("dt[0].year = %d\n", dt[0].tm_year);
	printf("UTC 1 time: %s", asctime(&d1));
	printf("UTC 1n time: %s", asctime(&dt[0]));
	printf("UTC 2 time: %s", asctime(&d2));
	printf("UTC 2n time: %s", asctime(&dt[6848]));*/ 
	ts2 = (ctserie *)malloc(n2 * sizeof(ctserie));
	/*tsn = allocMemTs(nda);*/ 
	for(i = 0; i < n2; i++){
		/*tsn->year[i] = dt[i].tm_year+IYEAR;
		tsn->month[i] = dt[i].tm_mon+1;
		tsn->day[i] = dt[i].tm_mday;
		tsn->var[i] = NAN;*/
		ts2[i].dt = dt[i];
		ts2[i].var = NAN;
	}
	free(dt);

	/* Match original time series with date & time array */
	for(i = 0; i < n2; i++){
		accum = 0.;
		k = 0;
		for(j = 0; j < ts1->n; j++){
			if ( ts2[i].dt.tm_year+IYEAR == ts1->year[j] &&
				 ts2[i].dt.tm_mon+1 == ts1->month[j] &&
				 ts2[i].dt.tm_mday == ts1->day[j]){
				
				if (ts1->var[j]<0){ /* Check for (-) values */
					val = NAN;
				}else{
					val = ts1->var[j];
				}
				/*break;*/
				k++;
				accum += val;
			}
		}
		if (k>0){ /* Estimate the average of k repeated dates */
			ts2[i].var = accum/k;
		}
	}

	return ts2;
}

/* 
Complete a ctserie struct by years
*/
ctserie *completeCtserie(ctserie *ts, int n, int *n1){
	unsigned int i, j, k, nyears;
	int n2;
	int *years, *uyears;
	struct tm *dt1;
	ctserie *ts1;
	struct tm d1 = {0};
	struct tm d2 = {0};
	struct tm d3 = {0};
	struct tm d4 = {0};

	/*Get years of ts*/
	years = malloc(n * sizeof(int));
	for (i = 0; i < n; i++){
		/*printf("%s", asctime(&ts[i].dt));*/
		years[i] = ts[i].dt.tm_year;
	}
	nyears = nUniqueInt(years, n);
	uyears = malloc(nyears * sizeof(int));
	uniqueInt(years, n, uyears);
	free(years);

	/* Number of days per year */
	*n1 = 0;
	for(i = 0; i < nyears; i++){
		if (i == 0){
			d1.tm_sec = 0;
			d1.tm_min = 0;
			d1.tm_hour = 0;
			d1.tm_mday = ts[0].dt.tm_mday;
			d1.tm_mon = ts[0].dt.tm_mon;
			d1.tm_year =ts[0].dt.tm_year;

			d2.tm_sec = 0;
			d2.tm_min = 0;
			d2.tm_hour = 0;
			d2.tm_mday = 31;
			d2.tm_mon = 11;
			d2.tm_year = ts[0].dt.tm_year;
		}else if(i == nyears-1){
			d1.tm_sec = 0;
			d1.tm_min = 0;
			d1.tm_hour = 0;
			d1.tm_mday = 1;
			d1.tm_mon = 0;
			d1.tm_year = ts[n-1].dt.tm_year;

			d2.tm_sec = 0;
			d2.tm_min = 0;
			d2.tm_hour = 0;
			d2.tm_mday = ts[n-1].dt.tm_mday;
			d2.tm_mon = ts[n-1].dt.tm_mon;;
			d2.tm_year = ts[n-1].dt.tm_year;
		}else{
			d1.tm_sec = 0;
			d1.tm_min = 0;
			d1.tm_hour = 0;
			d1.tm_mday = 1;
			d1.tm_mon = 0;
			d1.tm_year = uyears[i];

			d2.tm_sec = 0;
			d2.tm_min = 0;
			d2.tm_hour = 0;
			d2.tm_mday = 31;
			d2.tm_mon = 11;
			d2.tm_year = uyears[i];
		}

		/*printf("%s", asctime(&d1));
		printf("%s", asctime(&d2));*/
		dt1 = dateTime(&d1, &d2, &n2);
		/*printf("-1%s", asctime(&dt1[0]));
		printf("-2%s", asctime(&dt1[n2-1]));*/
		*n1 += n2;
		free(dt1);
	}
	/* *n1 = *n1 + 1;*/

	/* Complete by years */
	/*printf("n = %d , n1 = %d\n", n, *n1);*/
	ts1 = calloc(*n1, sizeof(ctserie));
	k = 0;
	for(i = 0; i < nyears; i++){
		if (i == 0){
			d3.tm_sec = 0;
			d3.tm_min = 0;
			d3.tm_hour = 0;
			d3.tm_mday = ts[0].dt.tm_mday;
			d3.tm_mon = ts[0].dt.tm_mon;
			d3.tm_year =ts[0].dt.tm_year;

			d4.tm_sec = 0;
			d4.tm_min = 0;
			d4.tm_hour = 0;
			d4.tm_mday = 31;
			d4.tm_mon = 11;
			d4.tm_year = ts[0].dt.tm_year;
		}else if(i == nyears-1){
			d3.tm_sec = 0;
			d3.tm_min = 0;
			d3.tm_hour = 0;
			d3.tm_mday = 1;
			d3.tm_mon = 0;
			d3.tm_year = ts[n-1].dt.tm_year;

			d4.tm_sec = 0;
			d4.tm_min = 0;
			d4.tm_hour = 0;
			d4.tm_mday = ts[n-1].dt.tm_mday;
			d4.tm_mon = ts[n-1].dt.tm_mon;;
			d4.tm_year = ts[n-1].dt.tm_year;
		}else{
			d3.tm_sec = 0;
			d3.tm_min = 0;
			d3.tm_hour = 0;
			d3.tm_mday = 1;
			d3.tm_mon = 0;
			d3.tm_year = uyears[i];

			d4.tm_sec = 0;
			d4.tm_min = 0;
			d4.tm_hour = 0;
			d4.tm_mday = 31;
			d4.tm_mon = 11;
			d4.tm_year = uyears[i];
		}
		/*printf("%s", asctime(&d1));
		printf("%s", asctime(&d2));*/
		dt1 = dateTime(&d3, &d4, &n2);
		/*printf("year = %d, n2 = %d\n", uyears[i], n2);*/
		/*printf("-1%s", asctime(&dt1[0]));
		printf("-2%s", asctime(&dt1[n2-1]));*/
		for (j = 0; j < n2; j++){
			ts1[k].dt = dt1[j];
			/*printf("year = %d\n", ts1[k].dt.tm_year);*/
			/*printf("->>>%s", asctime(&ts1[k].dt));*/
			ts1[k].var = NAN;
			k++;
		}
		free(dt1);
	}
	free(uyears);

	/*printf("n1 = %d, k = %d\n", *n1, k);*/
	/* Fill the var in the ts1 serie based on ts */
	for(i = 0; i < *n1; i++){
		for(j = 0; j < n; j++){
			/*time_t tt1 = mktime(&ts1[i].dt);
			time_t tt = mktime(&ts[j].dt);
			printf("%s------>%s", asctime(&ts[j].dt), asctime(&ts1[i].dt));
			printf("ts1 = %ld , ts = %ld\n",tt1, tt);*/
			/*printf("ts1 = %d , ts = %d\n",ts1[i].dt.tm_year, ts[j].dt.tm_year);*/
			/*if(tt1 == tt){*/
			if (ts1[i].dt.tm_mday == ts[j].dt.tm_mday &&  ts1[i].dt.tm_mon == ts[j].dt.tm_mon && ts1[i].dt.tm_year == ts[j].dt.tm_year){
				ts1[i].var = ts[j].var;
				/*printf("%s", asctime(&ts1[i].dt));
				printf("%s", asctime(&ts[j].dt));
				printf("tt = %d, i = %d, j = %d\n",tt, i, j);*/
				/*printf("i = %d, year = %d, ts1.var = %f\n", i, ts1[i].dt.tm_year, ts1[i].var);*/
				break;
			}
		}
	}
	return ts1;
}

/* 
Get most complete years in a ts 
*/
ctserie *getCompleteYearTS(ctserie *ts, int n, int *nc){
	
	unsigned int i, j, k;
	int *years, *cyears;
	ctserie *tsa, *tsb, *tsc;
	unsigned int nyears, ndaysy, ncyears;
	dtsinv *dtsi; 
	int n2, n1, n3;
	
	/* Remove nan from ctserie */
	tsa = removeNaNFromTs2(ts, n, &n2);
	/*printf("n = %d, n2 = %d\n", n, n2);*/

	/* Time series inventory */
	years = malloc(n2 * sizeof(int));
	for (i = 0; i < n2; i++){
		/*printf("tsa[i].var = %f\n", tsa[i].var);*/
		years[i] = tsa[i].dt.tm_year;
	}
	nyears = nUniqueInt(years, n2);
	/*printf("nyears = %d\n", nyears);*/
	dtsi = (dtsinv *)malloc(nyears * sizeof(dtsinv));
	dayTSinventory(tsa, n2, dtsi);
	/*for(i = 0; i < nyears; i++){
		printf("\n");
		for (j = 0; j<dtsi[i].nmonths; j++)
			printf("Year:%d, Month:%d, nmonths:%d, ndays:%d, ndayspy:%d\n",dtsi[i].year, 
			dtsi[i].month[j], dtsi[i].nmonths, dtsi[i].dayspm[j], dtsi[i].ndayspy);
	}*/

	/* Extract the portion of ts to be used. Extract 85% completed years */
	/* Count the number of completed years */
	n1  = 0;
	ncyears = 0;
	/*printf("nyears = %d\n", nyears);*/
	for(i = 0; i < nyears; i++){
		/*printf("i = %d, year = %d\n", i, dtsi[i].year);*/
		ndaysy = nDaysYear(dtsi[i].year + IYEAR);
		if ((float)dtsi[i].ndayspy/ndaysy > RATIODAYS){
			/*printf("Year %d ndays %d\n",dtsi[i].year, dtsi[i].ndayspy);*/
			n1 += dtsi[i].ndayspy;
			ncyears++;
		}
	}
	/*printf("ncyears = %d\n", ncyears);*/
	/* Test is there are completed years and allocate arrays */
	tsb = (ctserie *)malloc(n1 * sizeof(ctserie));	
	cyears = malloc(ncyears * sizeof(int));
	if (ncyears==0){
		printf("Very short time series, stopping...\n");
		free(dtsi);
		free(years);
		free(cyears);
		free(tsa);
		free(tsb);
		exit(0);
	}
	/* Save the complete years in the ts */
	j = 0;
	for(i = 0; i < nyears; i++){
		ndaysy = nDaysYear(dtsi[i].year + IYEAR);
		if ((float)dtsi[i].ndayspy/ndaysy > RATIODAYS){
			cyears[j] = (int)dtsi[i].year;
			/*printf("cyears = %d\n", cyears[j]);*/
			j++;
		}
	}
	/* Save the completed years in a new array of struct ctserie */
	k = 0;
	for(i = 0; i < ncyears; i++){
		for(j = 0; j < n2; j++){
			if (tsa[j].dt.tm_year == cyears[i]){
				tsb[k].dt = tsa[j].dt;	
				tsb[k].var = tsa[j].var;
				/*printf("year = %d, month = %d, day = %d, var = %f\n",
						tsb[k].dt.tm_year, tsb[k].dt.tm_mon, tsb[k].dt.tm_mday, tsb[k].var);*/
				k++;
			}
		}
	}

	free(tsa);
	free(dtsi);
	free(years);
	free(cyears);
	/*printf("ncyears = %d\n", ncyears);*/	
	
	tsc = completeCtserie(tsb, n1, &n3);
	*nc = n3;
	/*
	printf("year = %d, month = %d, day = %d, var = %f\n",
						tsb[0].dt.tm_year, tsb[0].dt.tm_mon, tsb[0].dt.tm_mday, tsb[0].var);
	printf("year = %d, month = %d, day = %d, var = %f\n",
						tsb[n1-1].dt.tm_year, tsb[n1-1].dt.tm_mon, tsb[n1-1].dt.tm_mday, tsb[n1-1].var);
	printf("n1 = %d, n3 = %d\n", n1, n3);
	printf("year = %d, month = %d, day = %d, var = %f\n",
						tsc[0].dt.tm_year, tsc[0].dt.tm_mon, tsc[0].dt.tm_mday, tsc[0].var);
	printf("year = %d, month = %d, day = %d, var = %f\n",
						tsc[n3-1].dt.tm_year, tsc[n3-1].dt.tm_mon, tsc[n3-1].dt.tm_mday, tsc[n3-1].var);
	*/

	free(tsb);
	return tsc;
}

/* 
Fill in nan values in a ts by years assuming there are data for some years
in the middle.
*/
ctserie *fillInNanInTS(ctserie *ts1, int n){

	unsigned int i, j, k, l, nyears;	
	float *y1, *y2, *x1;
	int *years, *uyears;
	ctserie *ts2;
	struct tm dt;

	/*Get years of ts*/
	years = malloc(n * sizeof(int));
	for (i = 0; i < n; i++){
		years[i] = ts1[i].dt.tm_year;
	}
	nyears = nUniqueInt(years, n);
	uyears = (int *)malloc(nyears * sizeof(int));
	uniqueInt(years, n, uyears);
	free(years);

	/* Fill in gaps for each years */
	l = 0;
	ts2 = (ctserie *)malloc(n * sizeof(ctserie));	
	for (i = 0; i < nyears; i++){ /* Loop for years in the ts */
		k = 0;
		for(j = 0; j < n; j++){
			if (uyears[i] == ts1[j].dt.tm_year){
				k++;
			}
		}
		x1 = malloc(k * sizeof(float));
		y1 = malloc(k * sizeof(float));
	
		k = 0;
		for (j = 0; j < n; j++){
			if (uyears[i] == ts1[j].dt.tm_year){
				dt = ts1[j].dt;
				x1[k] = (float)mktime(&dt);
				y1[k] = ts1[j].var;
				k++;
			}
		}
		/*printf("year = %d, k = %d\n",uyears[i], k);*/	
		y2 = fillInNaNLinInt(x1, y1, k);
		free(x1);
		free(y1);
	
		for (j = 0; j < k; j++){
			ts2[l].dt = ts1[l].dt;
			ts2[l].var = y2[j];
			l++;
			/*printf("ts1.var = %f, y2 = %f\n", ts1[i].var, y2[i]);*/
		}
		free(y2);
	}

	free(uyears);

	return ts2;
}


ctserie *preTreatTS(tserie *ts, int *nn){

	unsigned int i;
	ctserie *ts2, *ts3, *ts4;
	int n, nc, nnans;
	float *x;

	/* Matching original time series with create time serier based on struct tm */ 
	ts2 = matchingTs(ts, &n);

	/* Extract most completed years from the ts */
	ts3 = getCompleteYearTS(ts2, n, &nc);
	free(ts2);
	*nn = nc;

	/* Fill in NaN values in the ts */
	x = malloc(nc * sizeof(float));
	for (i = 0; i < nc; i++){
		x[i] = ts3[i].var;
	}
	nnans = countNaNs(x, nc);
	free(x);
	if(nnans > 0){ /* Check if there are NAN values in the ts */
		ts4 = fillInNanInTS(ts3, nc);
		free(ts3);
		return ts4;
	}else{
		return ts3;
	}
}

tserie *preTreatTS2(tserie *ts1){

	tserie *ts2, *tsa, *tsas, *tsb, *tsbs;
	dtsinv *dtsi; 
	int i, j, k, nyears, ncyears, ndaysy;
	int *years;
	struct tm sdate = { 0 };
	struct tm edate = { 0 };
	struct tm adate = { 0 };
	struct tm *ddates;	
	int tdays;
	double *y1, *y2, *x1, *x2;
	int nnonan;

	/* Switch negative values into NAN in tserie struct*/
	negativeToNanInTs(ts1);
	
	/* Remove rows with NAN values from time series */
	nnonan = countNoNaNs(ts1->var, ts1->n);
	tsas = allocMemTs(nnonan); 
	tsa = allocMemTs(nnonan); 
	removeNaNFromTs1(ts1, tsas);

	/* Check if the time series is sorted */
	sortTS(tsas, tsa);
	freeMemTs(tsas);
	
	/* Time series inventory */
	nyears = nUniqueInt(tsa->year, tsa->n);
	dtsi = (dtsinv *)malloc(nyears * sizeof(dtsinv));
	/*dayTSinventory(tsa, dtsi);*/
	/*for(i = 0; i < nyears; i++){
		printf("\n");
		for (j = 0; j<dtsi[i].nmonths; j++)
			printf("Year:%d, Month:%d, nmonths:%d, ndays:%d, ndayspy:%d\n",dtsi[i].year, 
			dtsi[i].month[j], dtsi[i].nmonths, dtsi[i].dayspm[j], dtsi[i].ndayspy);
	}*/

	/* Extract the portion of ts to be used. Extract 85% completed years */
	j  = 0;
	ncyears = 0;
	for(i = 0; i < nyears; i++){
		ndaysy = nDaysYear(dtsi[i].year);
		if ((float)dtsi[i].ndayspy/ndaysy > 0.67){
			/*printf("Year %d ndays %d\n",dtsi[i].year, dtsi[i].ndayspy);*/
			j += dtsi[i].ndayspy;
			ncyears++;
		}
	}
	tsb = allocMemTs(j);	
	years = (int *)malloc(ncyears * sizeof(int));
	if (ncyears==0){
		printf("Incomplete time series, stopping...\n");
		free(dtsi);
		freeMemTs(tsa);
		exit(0);
	}
	j = 0;
	for(i = 0; i < nyears; i++){
		ndaysy = nDaysYear(dtsi[i].year);
		if ((float)dtsi[i].ndayspy/ndaysy > 0.67){
			years[j] = (int)dtsi[i].year;
			j++;
		}
	}
	k = 0;
	for(i = 0; i < ncyears; i++){
		for(j = 0; j < tsa->n; j++){
			if (tsa->year[j] == years[i]){
				tsb->year[k] = tsa->year[j];
				tsb->month[k] = tsa->month[j];
				tsb->day[k] = tsa->day[j];
				tsb->var[k] = tsa->var[j];
				k++;
			}
		}
	}
	free(years);
	/*printf("size ts1:%d, size tsa:%d, size tsb:%d\n", ts1->n, tsa->n, tsb->n);*/

	/* Get the total number of days in a completed time series */ 
    sdate.tm_year = tsb->year[0]-1900;  /* years since 1900 //1973-1900;*/
    sdate.tm_mon = 0;
    sdate.tm_mday = 1;
    edate.tm_year = tsb->year[(tsb->n)-1]-1900; 
    edate.tm_mon = 11; 
    edate.tm_mday = 31;
	/*printf("tsb.year0 %d, tsb.yearN %d\n", tsb.year[0], tsb.year[tsb.n-1]);*/
	tdays = difftime(mktime(&edate), mktime(&sdate))/86400 + 1;
	/*allocMemTs(ts2, tdays);*/	

	/* Interpolation for individual years */
	nyears = nUniqueInt(tsb->year, tsb->n);
	/*printf("nyears = %d\n", nyears);*/
	years = (int *)malloc(nyears * sizeof(int));
	uniqueInt(tsb->year, tsb->n, years);
	tdays = 0;
	for(i = 0; i<nyears; i++)
		tdays += nDaysYear(years[i]);	
	ts2 = allocMemTs(tdays);	
		
	k = 0;
	for(i = 0; i < nyears; i++){
		printf("Year: %d\n", years[i]);
		sdate.tm_year = years[i]-1900; /* years since 1900 */
		sdate.tm_mon = 0;
		sdate.tm_mday = 1;
		edate.tm_year = sdate.tm_year; 
		edate.tm_mon = 11; 
		edate.tm_mday = 31; 
		tdays = difftime(mktime(&edate), mktime(&sdate))/86400 + 1;
		ddates = (struct tm *)malloc(tdays * sizeof(struct tm));	
		makeUpSeqDayDates(&sdate, tdays, ddates);
		/*sliceDayTimeSeries(&sdate, &edate, &tsb, &tsbs);*/
		tsbs = extractOneYearTSFromTS(tsb, years[i]);
		/*printf("y0:%d, yn:%d\n",tsbs.year[0], tsbs.year[tsbs.n-1]);*/

		if (tsbs->n < tdays){
			printf("Incomplete year %d, with days %d and tdays %d\n",years[i], tsbs->n, tdays);
			x1 = (double *)malloc(tsbs->n * sizeof(double));
			y1 = (double *)malloc(tsbs->n * sizeof(double));
			for(j = 0; j < tsbs->n; j++){
				adate.tm_year = tsbs->year[j];
				adate.tm_mon = tsbs->month[j]-1;
				adate.tm_mday = tsbs->day[j];
				x1[j] = mktime(&adate);
				y1[j] = tsbs->var[j];
				/*printf("x1:%ld, y1:%f, year %d, month %d, day %d\n",x1[j], y1[j], tsbs.year[j], tsbs.month[j], tsbs.day[j]);*/
			}

			x2 = (double *)malloc(tdays * sizeof(double));
			y2 = (double *)malloc(tdays * sizeof(double));
			/*i1 = 0;*/
			/*adate.tm_year = tsbs.year[i1]-1900;*/
			/*adate.tm_mon = tsbs.month[i1]-1;*/
			/*adate.tm_mday = tsbs.day[i1];*/
			for(j = 0; j<tdays; j++){
				ddates[j].tm_year+=1900;
				x2[j] = mktime(&ddates[j]);
				ddates[j].tm_year-=1900;
				/*if (mktime(&ddates[j])==mktime(&adate)){
				//	x1[i1] = j;
				//	y1[i1] = tsbs.var[i1];
				//	i1++;
				//	adate.tm_year = tsbs.year[i1]-1900;
				//	adate.tm_mon = tsbs.month[i1]-1;
				//	adate.tm_mday = tsbs.day[i1];
				}*/
			}
			/*for(j = 0; j < tsbs.n; j++)
				printf("x1=%lf, y1=%lf, x2=%lf\n", x1[j], y1[j], x2[j]);*/

			linInterp((double *)x1, y1, tsbs->n, (double *)x2, y2, tdays);
			for(j = 0; j<tdays; j++){
				ts2->year[k] = ddates[j].tm_year + 1900;
				ts2->month[k] = ddates[j].tm_mon + 1;
				ts2->day[k] = ddates[j].tm_mday;
				ts2->var[k] = y2[j];
				/*printf("year=%d, month=%d, day=%d\n", ts2->year[k], ts2->month[k], ts2->day[k]);*/
				k++;
			}
			free(x1);
			free(x2);
			free(y1);
			free(y2);
		}else{
			for(j = 0; j<tdays; j++){
				ts2->year[k] = ddates[j].tm_year + 1900;
				ts2->month[k] = ddates[j].tm_mon + 1;
				ts2->day[k] = ddates[j].tm_mday;
				ts2->var[k] = tsbs->var[j];
				k++;
			}
		}
		free(ddates);
		freeMemTs(tsbs);

	}
	free(years);
	free(dtsi);
	freeMemTs(tsa);
	freeMemTs(tsb);
	/*freeMemTs(ts1);*/
	
	return ts2;
}
