#include "utils.h"
#include "tspre.h"

//#include <string.h>

/*
/Read the time series from a csv file
*/
int readFile(char *filename, tserie *ts)
{
	FILE *fp;


	if ((fp = fopen(filename,"r")) == NULL){
		printf("Error reading %s stopping... \n", filename);
		exit(1);
	
	}
	//printf("Reading file\n");
	/*char *token;*/
	int i = 0;
	char line[MAXCHAR];
	char *token;
	char seps[]=",";
	
	/* Read the header's file*/
	fgets(line, MAXCHAR, fp); 
	/*printf("%s",line);*/
	char *newline;
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
		//strcpy(ts->var_symbol[i], token);
		token = strtok(NULL,",");

		//printf("%d,%d,%d,%g,%s\n", ts->year[i], ts->month[i], ts->day[i], ts->var[i], ts->var_symbol[i]);
		//printf("%d,%d,%d,%g\n", ts->year[i], ts->month[i], ts->day[i], ts->var[i]);
	
		/*if (i == 80){
			break;
		}*/

		i++;
	}
	fclose(fp);
	return 0;
}

/*
Read time series from csv file
*/
int readTSfromCSV(tfile *tsf, tserie *ts){
	char *filename;

	/* Make up filename */
	filename = malloc(strlen(tsf->dirname) + strlen(tsf->filename));
	strcpy(filename, tsf->dirname);
	strcat(filename, tsf->filename);
	
	/* Number of file rows */
	tsf->nrows = nrowsFile(filename); 

	/* Reading ts from csv file */ 
	allocMemTs(ts, tsf->nrows-1);  /* -1 to remove header in 1rs row */ 
	printf("Reading %s ts file with %d rows.\n", filename, tsf->nrows);
	
	/* Read time series from ascii file */
	readFile(filename, ts);
	free(filename);

	return 0;
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
	filename = malloc(strlen(tsf->dirname) + strlen(tsf->filename));
	strcpy(filename, tsf->dirname);
	strcat(filename, tsf->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"w")) == NULL){
		printf("Error writing %s stopping... \n", filename);
		return -1;
	}
	
	//printf("%s\n",concInts2string(x, 3, "-"));
	//fprintf(fp,"%s,%s,%s,%s\n","YEAR", "MONTH", "DAY", "VAR");
	for(i = 0; i < ts->n; i++){
		x[0]=ts->year[i];
		x[1]=ts->month[i];
		x[2]=ts->day[i];
		str = concInts2string(x, 3, "-");
		//printf("%s\n",str);
		fprintf(fp,"%s,%f\n",str, ts->var[i]);
		free(str);
		//fprintf(fp,"%d,%d,%d,%f\n",ts->year[i], ts->month[i], ts->day[i], ts->var[i]);
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
	filename = malloc(strlen(tsf->dirname) + strlen(tsf->filename));
	strcpy(filename, tsf->dirname);
	strcat(filename, tsf->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"wb")) == NULL){
		printf("Error writing %s stopping... \n", filename);
		return -1;
	}
	
	//printf("%s\n",concInts2string(x, 3, "-"));
	//fprintf(fp,"%s,%s,%s,%s\n","YEAR", "MONTH", "DAY", "VAR");
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
	printf("here\n");

	fclose(fp);
	free(filename);

	return 0;
}

/*
Read and display bin file with tserie
*/
int readTserieBinFile(void){

    // Create a FILE pointer
    FILE* my_file = NULL;
	int year, month, day;
	double var;
	unsigned i;

    // Open the file
    my_file = fopen("ts.bin", "rb");

    // Checking to see that the file definitely opened:
    if (my_file == NULL) {
        printf("Couldn't open file\n");
        return -1;
    }

    // Read the object and store in the buffer
	i = 0;
	while (true){
		fread(&year, sizeof(int), 1, my_file);
    	fread(&month, sizeof(int), 1, my_file);
    	fread(&day, sizeof(int), 1, my_file);
    	fread(&var, sizeof(double), 1, my_file);

		if (feof(my_file))
			break;

		// Print each element of the object
    	printf("%d-%d-%d, %f\n", year, month, day, var);
			i++;
	}
    // Close the file
    fclose(my_file);
	//printf("i=%d\n",i);
    return 0;
}

/*
Allocate heap memory of an struct tserie
*/
void allocMemTs(tserie *ts, int n){
	ts->n = n;
	ts->year = (int *)malloc(n * sizeof(int));
	ts->month= (int *)malloc(n * sizeof(int));
	ts->day = (int *)malloc(n * sizeof(int));
	ts->var = (float *)malloc(n * sizeof(float));
	//ts->var_symbol = (char *)malloc(ts->n * sizeof(char));
	//for (int i = 0; i < ts->n; i++)
	//	ts->var_symbol[i] = malloc(5 * sizeof(char));
}

/*
Free heap memory taken by tserie struct
*/
void freeMemTs(tserie *ts){
	free(ts->year);
	free(ts->month);
	free(ts->day);
	free(ts->var);
	//for (int i = 0; i < ts->n; i++){
	//	free(ts->var_symbol[i]);
	//}	
}

/*
Remove rows that containt NAN values from  ts struct.
*/
int removeNaNFromTs(tserie *ts1, tserie *ts2){
	int j = 0;

	//ts2->n = countNoNaNs(ts1->var, ts1->n);
	//allocMemTs(ts2, ts2->n);
	
	for (int i = 0; i < ts1->n; i++){
		if (!isnan(ts1->var[i])){
			ts2->year[j] = ts1->year[i];
			ts2->month[j] = ts1->month[i];
			ts2->day[j] = ts1->day[i];
			ts2->var[j] = ts1->var[i];
			//ts2->var_symbol[j] = ts1->var_symbol[j];	
			j++;
		}
	}
	return 0;
}

/*
Return a dtsinv structure with the inventory of a 
daily time series.
*/
int dayTSinventory(tserie *ts, dtsinv *dtsi){
	int i, j, k, nd, nm, ndy;
	
	i = 0;
	j = 0;
	k = 0;
	nd = 1;
	nm = 1;
	ndy = 1;
	dtsi[j].year = ts->year[i];
	dtsi[j].ndayspy = ndy;
	dtsi[j].month[k] = ts->month[i];
	dtsi[j].nmonths = nm;
	dtsi[j].dayspm[k] = nd;
	for (i = 1; i < ts->n; i++){
		if (dtsi[j].year == ts->year[i]){
			if (dtsi[j].month[k] == ts->month[i]){
				nd++;
			}else{
				dtsi[j].dayspm[k] = nd;
				k++;
				nm++;
				dtsi[j].nmonths = nm;
				dtsi[j].month[k] = ts->month[i];
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
			dtsi[j].year = ts->year[i];
			dtsi[j].month[k] = ts->month[i];
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
	//struct tm *ptm;
	//char buf[256] = {0};
	
	t2i = mktime(startdate);
	for(i = 0; i < tdays; i++){
		//t2 += one_day;
		t2 = t2i + one_day*i;
		daydates[i] = *gmtime(&t2);  
		/* Set to hour = 0, min = 0 and sec = 0 as it usually 
		covert to UTC giving some hours e.g. 5hours ahead */
		daydates[i].tm_hour = 0;
		daydates[i].tm_min = 0;
		daydates[i].tm_sec = 0;
		//daydates[i].tm_mon++;
		//daydates[i] = *ptm; // = gmtime(&t2);  
		//daydates[i].tm_year = ptm->tm_year;
		//daydates[i].tm_mon = ptm->tm_mon;
//		daydates[i].tm_mday = ptm->tm_mday;

		//printf(" year: %d, month:%d, day:%d, hour:%d\n", daydates[i].tm_year, daydates[i].tm_mon, daydates[i].tm_mday, daydates[i].tm_hour);
//		strftime(buf, 256, "%F", ptm);
		//strftime(buf, 256, "%F", &daydates[i]);
		//puts(buf);
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
//tserie *sliceDayTimeSeries(struct tm *sdate, struct tm *edate, tserie *ts1){
	
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
//
	//tserie *ts2=NULL;
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
//		if (ta == st)
		if (difftime(ta,st)==0)	
			ids = i;
//		if (ta == en)
		if (ta>=st && ta<=en)
			j++;
		if (difftime(ta, en)==0){
			ide = i;
			break;
		}
	}
	allocMemTs(ts2,j);
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
int extractOneYearTSFromTS(tserie *ts1, int year, tserie *ts2){
	int i, j;
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
	allocMemTs(ts2,ide-ids+1);
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
	allocMemTs(ts2,ide-ids+1);
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

	//printf("ts1->month = %d\n", ts1->day[0]);
	for(i = 0; i < ts1->n; i++){
		sd.tm_year = ts1->year[i];
		sd.tm_mon = ts1->month[i]-1;
		sd.tm_mday = ts1->day[i];
		x1[i] = mktime(&sd);
		x2[i] = x1[i];
		//printf("x1=%ld\n",mktime(&sd));
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

int preTreatTS(tserie *ts1, tserie *ts2){

	tserie tsa, tsas, tsb, tsbs;
	dtsinv *dtsi; 
	int i, j, k, nyears, ncyears, ndaysy;
	int *years;
	struct tm sdate = { 0 };
	struct tm edate = { 0 };
	struct tm adate = { 0 };
	struct tm *ddates;	
	int tdays;
	double *y1, *y2, *x1, *x2;

    //char buf[BUF_LEN] = {0};
	/* Switch negative values into NAN in tserie struct*/
	negativeToNanInTs(ts1);
	
	/* Remove rows with NAN values from time series */
	int nnonan = countNoNaNs(ts1->var, ts1->n);
	allocMemTs(&tsas, nnonan); 
	allocMemTs(&tsa, nnonan); 
	removeNaNFromTs(ts1, &tsas);

	/* Check if the time series is sorted */
	sortTS(&tsas, &tsa);
	freeMemTs(&tsas);
	
	/* Time series inventory */
	nyears = nUniqueInt(tsa.year, tsa.n);
	dtsi = (dtsinv *)malloc(nyears * sizeof(dtsinv));
	dayTSinventory(&tsa, dtsi);
	//for(i = 0; i < nyears; i++){
	//	printf("\n");
	//	for (j = 0; j<dtsi[i].nmonths; j++)
	//		printf("Year:%d, Month:%d, nmonths:%d, ndays:%d, ndayspy:%d\n",dtsi[i].year, 
	//		dtsi[i].month[j], dtsi[i].nmonths, dtsi[i].dayspm[j], dtsi[i].ndayspy);
	//}

	/* Extract the portion of ts to be used. Extract 85% completed years */
	j  = 0;
	ncyears = 0;
	for(i = 0; i < nyears; i++){
		ndaysy = nDaysYear(dtsi[i].year);
//		printf("propo %f\n", (float)dtsi[i].ndayspy/ndaysy); 
		if ((float)dtsi[i].ndayspy/ndaysy > 0.67){
			//printf("Year %d ndays %d\n",dtsi[i].year, dtsi[i].ndayspy);
			j += dtsi[i].ndayspy;
			ncyears++;
		}
	}
	allocMemTs(&tsb, j);	
	years = (int *)malloc(ncyears * sizeof(int));
	if (ncyears==0){
		printf("Incomplete time series, stopping...\n");
		free(dtsi);
		freeMemTs(&tsa);
		return -1;
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
		//printf("year = %d\n",years[i]);
		for(j = 0; j < tsa.n; j++){
			if (tsa.year[j] == years[i]){
				tsb.year[k] = tsa.year[j];
				tsb.month[k] = tsa.month[j];
				tsb.day[k] = tsa.day[j];
				tsb.var[k] = tsa.var[j];
				k++;
			}
		}
	}
	free(years);
	//printf("size ts1:%d, size tsa:%d, size tsb:%d\n", ts1->n, tsa.n, tsb.n);

	/* Get the total number of days in a completed time series */ 
    sdate.tm_year = tsb.year[0]-1900; //1973-1900; /* years since 1900 */
    sdate.tm_mon = 0;//tsa.month[0]-1; //8;
    sdate.tm_mday = 1;// tsa.day[0]; //1;
    edate.tm_year = tsb.year[tsb.n-1]-1900; //1995-1900;
    edate.tm_mon = 11; //tsa.month[tsa.n-1]-1; //5;
    edate.tm_mday = 31; //tsa.day[tsa.n-1]; //31;
	//printf("tsb.year0 %d, tsb.yearN %d\n", tsb.year[0], tsb.year[tsb.n-1]);
	tdays = difftime(mktime(&edate), mktime(&sdate))/86400 + 1;
	//allocMemTs(ts2, tdays);	

	/* Interpolation for individual years */
	nyears = nUniqueInt(tsb.year, tsb.n);
	//printf("nyears = %d\n", nyears);
	years = (int *)malloc(nyears * sizeof(int));
	uniqueInt(tsb.year, tsb.n, years);
	tdays = 0;
	for(i = 0; i<nyears; i++)
		tdays += nDaysYear(years[i]);	
	allocMemTs(ts2, tdays);	
		
	k = 0;
	for(i = 0; i < nyears; i++){
		//printf("Year: %d\n", years[i]);
		sdate.tm_year = years[i]-1900; /* years since 1900 */
		sdate.tm_mon = 0;//tsa.month[0]-1; //8;
		sdate.tm_mday = 1;// tsa.day[0]; //1;
		edate.tm_year = sdate.tm_year; //1995-1900;
		edate.tm_mon = 11; //tsa.month[tsa.n-1]-1; //5;
		edate.tm_mday = 31; //tsa.day[tsa.n-1]; //31;
		tdays = difftime(mktime(&edate), mktime(&sdate))/86400 + 1;
		ddates = (struct tm *)malloc(tdays * sizeof(struct tm));	
		makeUpSeqDayDates(&sdate, tdays, ddates);
		//sliceDayTimeSeries(&sdate, &edate, &tsb, &tsbs);
		extractOneYearTSFromTS(&tsb, years[i], &tsbs);
		//printf("y0:%d, yn:%d\n",tsbs.year[0], tsbs.year[tsbs.n-1]);
		//printf("tsbs.n =%d\n", tsbs.n);

		if (tsbs.n < tdays){
			printf("Incomplete year %d, with days %d and tdays %d\n",years[i], tsbs.n, tdays);
			x1 = (double *)malloc(tsbs.n * sizeof(double));
			y1 = (double *)malloc(tsbs.n * sizeof(double));
			for(j = 0; j < tsbs.n; j++){
				adate.tm_year = tsbs.year[j];
				adate.tm_mon = tsbs.month[j]-1;
				adate.tm_mday = tsbs.day[j];
				//adate.tm_isdst = 1;
				//strftime(buf, BUF_LEN,"%-j",&adate);
				x1[j] = mktime(&adate);
				y1[j] = tsbs.var[j];
				//printf("x1:%ld, y1:%f, year %d, month %d, day %d\n",x1[j], y1[j], tsbs.year[j], tsbs.month[j], tsbs.day[j]);
			}

			x2 = (double *)malloc(tdays * sizeof(double));
			y2 = (double *)malloc(tdays * sizeof(double));
			//i1 = 0;
			//adate.tm_year = tsbs.year[i1]-1900;
			//adate.tm_mon = tsbs.month[i1]-1;
			//adate.tm_mday = tsbs.day[i1];
			for(j = 0; j<tdays; j++){
				ddates[j].tm_year+=1900;
				x2[j] = mktime(&ddates[j]);
				ddates[j].tm_year-=1900;
				//if (mktime(&ddates[j])==mktime(&adate)){
				//	x1[i1] = j;
				//	y1[i1] = tsbs.var[i1];
				//	i1++;
				//	adate.tm_year = tsbs.year[i1]-1900;
				//	adate.tm_mon = tsbs.month[i1]-1;
				//	adate.tm_mday = tsbs.day[i1];
				//}
			}
			//for(j = 0; j < tsbs.n; j++)
			//	printf("x1=%lf, y1=%lf, x2=%lf\n", x1[j], y1[j], x2[j]);

			linInterp((double *)x1, y1, tsbs.n, (double *)x2, y2, tdays);
			for(j = 0; j<tdays; j++){
				ts2->year[k] = ddates[j].tm_year + 1900;
				ts2->month[k] = ddates[j].tm_mon + 1;
				ts2->day[k] = ddates[j].tm_mday;
				ts2->var[k] = y2[j];
				//printf("year=%d, month=%d, day=%d\n", ts2->year[k], ts2->month[k], ts2->day[k]);
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
				ts2->var[k] = tsbs.var[j];
				k++;
			}
		}
		free(ddates);
		freeMemTs(&tsbs);

	}
	free(years);
	free(dtsi);
	freeMemTs(&tsa);
	freeMemTs(&tsb);
	
	return 0;
}
