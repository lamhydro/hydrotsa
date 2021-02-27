#include "utils.h"
#include "stats.h"
#include "aggts.h"

/* 
Count the number of months per year in at ts struct and then add them up
to return a total.
*/
int nMonthsInYearDayTs(const ctserie *ts, const int n){
	int i, j, year, month;

	year = ts[0].dt.tm_year;
	month = ts[0].dt.tm_mon;
	j = 1;
	for(i = 1; i < n; i++){
		if (ts[i].dt.tm_year != year){
			year = ts[i].dt.tm_year;
			month = ts[i].dt.tm_mon;
			j++;
		}
		if (ts[i].dt.tm_mon != month){
			month = ts[i].dt.tm_mon;
			j++;
		}
	}

	return j;
}

/*
Aggreate daily ts into monthly ts.
*/
agg *aggDayToMonth1(const ctserie *ts, const int n, int *nm){
	int i,j,k, start;	
	int month, year;
	int *nd, *ids, *ide;
	float *y;
	agg *tsm;

	*nm = nMonthsInYearDayTs(ts, n);
	tsm = (agg *)malloc(*nm * sizeof(agg));
	nd = malloc(*nm * sizeof(int));
	ids = malloc(*nm * sizeof(int));
	ide = malloc(*nm * sizeof(int));

	month = ts[0].dt.tm_mon;
	year = ts[0].dt.tm_year;
	j = 0;
	start = 1;
	ids[0] = 0;
	for(i = 1; i < n; i++){
		if(ts[i].dt.tm_mon == month && ts[i].dt.tm_year == year){
			if (start){
				ids[j] = i-1;
				start = 0;
			}
		}else{
			start = 1;
			ide[j] = i-1;
			nd[j] = ide[j] - ids[j] + 1;
			year = ts[i].dt.tm_year;
			month = ts[i].dt.tm_mon;
			j++;
		}
	}
	ide[j] = i-1;
	nd[j] = ide[j] - ids[j]+1;

	for (j = 0; j < *nm; j++){
		k = 0;
		y = (float *)malloc(nd[j] * sizeof(float));
		for(i = ids[j]; i <= ide[j]; i++){
			y[k] = ts[i].var;
			k++;
		}
		/*printf("year %d, month %d, ids %d, ide %d, nd %d\n", ts->year[ids[j]], ts->month[ids[j]], ids[j], ide[j], nd[j]);*/
		tsm[j].dt.tm_year = ts[ids[j]].dt.tm_year;
		tsm[j].dt.tm_mon = ts[ids[j]].dt.tm_mon;
		tsm[j].mean = mean(y,nd[j]);
		tsm[j].median = median(y,nd[j]);
		tsm[j].std = stdDev(y,nd[j]);
		tsm[j].min= minval(y,nd[j]);
		tsm[j].max= maxval(y,nd[j]);
		/*y = realloc(y, nd[j]*sizeof(float));*/
		free(y);
	}
	free(nd);
	free(ide);
	free(ids);
	return tsm;
}

/*
Aggreate daily ts into monthly ts.
*/
int aggDayToMonth(tserie *ts1, tserie *ts2){
	int i = 0;
	int j = 0;
	int k;
	
	/*ts2->n = nMonthsInYearDayTs(ts1);
	allocMemTs(ts2, ts2->n);*/

	ts2->month[j] = ts1->month[i];
	ts2->year[j] = ts1->year[i];
	ts2->var[j] = ts1->var[i];
	k = 1;
	for(i = 1; i < ts1->n; i++){
		if(ts2->month[j] == ts1->month[i]){
			ts2->var[j] += ts1->var[i];
			k++;
		}
		else{
			ts2->var[j] = ts2->var[j]/k; 
			j++;
			ts2->month[j] = ts1->month[i];
			ts2->var[j] = ts1->var[i];
			k = 1;
		}
		if(ts2->year[j] != ts1->year[i])
			ts2->year[j] = ts1->year[i];
	}
	ts2->var[j] = ts2->var[j]/k; 

	return 0;
}

/*
Aggreate daily ts into yearly ts.
*/
agg *aggDayToYear1(tserie *ts, int *n){
	int i,j,k, start;	
	int year;
	int *nd, *ids, *ide;
	float *y;
	agg *tsy;

	*n = nUniqueInt(ts->year, ts->n);
	tsy = (agg *)malloc(*n * sizeof(agg));
	nd = (int *)malloc(*n * sizeof(int));
	ids = (int *)malloc(*n * sizeof(int));
	ide = (int *)malloc(*n * sizeof(int));

	year = ts->year[0];
	j = 0;
	start = 1;
	ids[0] = 0;
	for(i = 1; i < ts->n; i++){
		if(ts->year[i] == year){
			if (start){
				ids[j] = i-1;
				start = 0;
			}
		}else{
			start = 1;
			ide[j] = i-1;
			nd[j] = ide[j] - ids[j] + 1;
			year = ts->year[i];
			j++;
		}
	}
	ide[j] = i-1;
	nd[j] = ide[j] - ids[j]+1;
	/*printf("ids %d, ide %d, nd %d\n", ids[*n-1], ide[*n-1], nd[*n-1]);*/

	for (j = 0; j < *n; j++){
		k = 0;
		y = (float *)malloc(nd[j] * sizeof(float));
		for(i = ids[j]; i <= ide[j]; i++){
			y[k] = ts->var[i];
			k++;
		}
		/*printf("i %d, year %d, ids %d, ide %d, nd %d, nt %d\n", j, ts->year[ids[j]],  ids[j], ide[j], nd[j], ts->n);*/
		tsy[j].dt.tm_year = ts->year[ids[j]];
		tsy[j].mean = mean(y,nd[j]);
		tsy[j].median = median(y,nd[j]);
		tsy[j].std = stdDev(y,nd[j]);
		tsy[j].min= minval(y,nd[j]);
		tsy[j].max= maxval(y,nd[j]);
		/*y = realloc(y, nd[j]*sizeof(float));*/
		free(y);
	}
	return tsy;
}	

/*
Aggregate daily ts into yearly ts.
*/
int aggDayToYear(tserie *ts1, tserie *ts2){
	int i, j, k;
	float sum;


/*	x1.n = ts1->n;
//	x1.a = (int *)malloc(x1.n * sizeof(int));
//	for(i = 0; i<ts1->n; i++)
//		x1.a[i] = ts1->year[i];
//
//	uniqueInt(&x1, &x2);
//	ts2->n = x2.n;
	allocMemTs(ts2, ts2->n);*/

	for(i = 0; i < ts2->n; i++){
		sum = 0;
		k = 0;
		for(j = 0; j < ts1->n; j++)
			if (ts2->year[i] == ts1->year[j]){
				sum += ts1->var[j];
				k++;
			}
		/*ts2->year[i] = x2.a[i];*/
		ts2->var[i] = sum/k;

	}
/*	for(i = 0; i < ts2->n; i++)
		printf("Year %d Q %f \n", ts2->year[i], ts2->var[i]);*/

	return 0;
}

/*
Summary of statistic per year for a time series. This suppose to be
used for pre-treated time series
*/
yrsum *tsYearlySummary(const ctserie *ts, const int n, int *nyears){
	int i;
	int *years;
	tserie *tsy;
	float pe[] = {5,15,25,75,85,95};
	float pv[6];
	yrsum *ys;
	int *x;

	/* Get the number of years */
	x = malloc(n * sizeof(int));
	for(i = 0; i < n; i++){
		x[i] = ts[i].dt.tm_year;
	}
	*nyears = nUniqueInt(x, n);

	/* Set the years array */
	years = malloc(*nyears * sizeof(int));
	uniqueInt(x, n, years);
	free(x);

	ys = (yrsum *)malloc(*nyears * sizeof(yrsum));	

	for(i = 0; i < *nyears; i++){
		tsy = extractOneYearTSFromTS(ts, n, years[i]);
		perctl(tsy->var, tsy->n, pe, 6, pv);
		
		ys[i].year = years[i];
		ys[i].mean = mean(tsy->var, tsy->n);
		ys[i].q25 = pv[2]; 
		ys[i].q50 = median(tsy->var, tsy->n);
		ys[i].q75 = pv[3]; 
		ys[i].std = stdDev(tsy->var, tsy->n);
		ys[i].covar = coeffVari(tsy->var, tsy->n);
		ys[i].skew = skewness(tsy->var, tsy->n);
		ys[i].max = maxval(tsy->var, tsy->n);
		ys[i].min  = minval(tsy->var, tsy->n);
		ys[i].range = ys[i].max-ys[i].min;
		ys[i].bfix = baseFlowIndex(tsy);
		ys[i].riser = riseRate(tsy->var, tsy->n);
		ys[i].fallr = fallRate(tsy->var, tsy->n);
		/*printf("year = %d, mean = %f, Q25 = %f, Q50 = %f, Q75 = %f, std = %f, cv = %f, skew = %f,
		//		max = %f, min = %f, range = %f, baseFlowInd = %f, riseRate = %f, fallRate = %f\n",
		//		years[i], mea, pv[2], med, pv[3], std, cv, skew, max, min, range, bfix, riser, fallr);*/
		
		freeMemTs(tsy);
	}
	free(years);
	return ys;
}

/*
Write yearly summary of a ts into a csv file
*/
int writeTsYearlySummary(const tfile *ysf, const yrsum *ys, const int n){
	int i;
	FILE *fp;
	char *filename;

	/* Make up filename */
	filename = malloc(strlen(ysf->dirname) + strlen(ysf->filename));
	strcpy(filename, ysf->dirname);
	strcat(filename, ysf->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"w")) == NULL){
		printf("Error reading %s stopping... \n", filename);
		return -1;
	}
	
	fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",\
			"YEAR","MEAN","Q25","Q50","Q75","STD","COVAR","SKEW","MAX","MIN","RANGE","BFIX","RISER","FALLR");
	for(i = 0; i < n; i++){
		fprintf(fp,"%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",\
				ys[i].year, ys[i].mean,ys[i].q25,ys[i].q50,ys[i].q75,ys[i].std,ys[i].covar,\
				ys[i].skew,ys[i].max,ys[i].min,ys[i].range,ys[i].bfix,ys[i].riser,ys[i].fallr);;
	}	
	
	free(filename);

	return 0;
}

/*
Aggregation to every month of the year using montly mean
*/
regi *monthlyRegime(const ctserie *ts, const int n){

	int i, j, k, nm;
	int nd[NYMON];
	float *m;
	agg *tsm;
	regi *mre;
	
	/* Aggregate to montly ts first */
	tsm = aggDayToMonth1(ts, n, &nm);

	mre = (regi *)malloc(NYMON * sizeof(regi));

	for(i = 0; i < NYMON; i++){
		k = 0;
		for(j = 0; j < nm; j++){
			if (tsm[j].dt.tm_mon == i+1)
				k++;
		}
		nd[i] = k;
	}
	
	for(i = 0; i < NYMON; i++){
		if (nd[i] > 0){	
			m = malloc(nd[i] * sizeof(float));
			k = 0;
			for(j = 0; j < nm; j++){
				if (tsm[j].dt.tm_mon == i+1){
					m[k] = tsm[j].mean;
					k++;
				}
			}
			mre[i].x = i+1;
			mre[i].mean = mean(m,nd[i]);
			mre[i].median = median(m,nd[i]);
			mre[i].std = stdDev(m,nd[i]);
			mre[i].max = maxval(m,nd[i]);
			mre[i].min = minval(m,nd[i]);
			free(m);
		}else{
			mre[i].x = i+1;
			mre[i].mean = NAN; 
			mre[i].median = NAN; 
			mre[i].std = NAN; 
			mre[i].max = NAN; 
			mre[i].min = NAN; 
		}
	}
	free(tsm);	
	return mre;
}



regi *dailyRegime(ctserie *ts, int n, int *ny){

	unsigned int i, j, k;
	int *nd, *yday;
	float *m;
	regi *dre;

	/* Number of days in a year */
	yday = dayOfYearTs(ts, n);
	*ny = maxvald(yday, n); 
	if (*ny < 365 || *ny > 366){
		printf("Number of year day of the range %d. Stopping...\n", *ny);
		exit(0);
	}
	dre = malloc(*ny * sizeof(regi));

	/* Set the array of days in a year */
	for(i = 0; i < *ny; i++){
		dre[i].x = i+1; 
	}

	/* Save the number of occurence of each year in the ts */
	nd = malloc(*ny * sizeof(int));
	for(i = 0; i < *ny; i++){
		k = 0;
		for(j = 0; j < n; j++){
			if (yday[j] == dre[i].x)
				k++;
		}
		nd[i] = k;
		/*printf("nd = %d\n", nd[i]);*/
	}

	/* Estimate statistics of the daily regime */
	for(i = 0; i < *ny; i++){
		if (nd[i] > 0){
			m = malloc(nd[i] * sizeof(float));
			k = 0;
			for(j = 0; j < n; j++){
				if ((int)yday[j] == dre[i].x){
					m[k] = ts[j].var;
					k++;
				}
			}
			dre[i].mean = mean(m,nd[i]);
			dre[i].median = median(m,nd[i]);
			dre[i].std = stdDev(m,nd[i]);
			dre[i].max = maxval(m,nd[i]);
			dre[i].min = minval(m,nd[i]);
			free(m);
		}else{
			dre[i].mean = NAN;
			dre[i].median = NAN;
			dre[i].std = NAN;
			dre[i].max = NAN;
			dre[i].min = NAN; 
		}
	}
	free(nd);
	free(yday);
	return dre;
}

/*
Aggregation to every day of the year
*/
regi *dailyRegime2(tserie *ts, int *nydays){

	int i, j, k;
	int *nd;
	float *m;
	int *yday;
	int *ydayu;
	regi *dre;
	
	yday = (int *)malloc(ts->n * sizeof(int));

	/*dayOfYearTs(ts, yday);*/
	for(i=0; i<ts->n; i++) printf("day = %d\n", yday[i]);
	*nydays = nUniqueInt(yday, ts->n);
	ydayu = (int *)malloc(*nydays * sizeof(int));
	uniqueInt(yday, ts->n, ydayu);

	ascenSortInt(ydayu, *nydays); 
	
	dre = (regi *)malloc(*nydays * sizeof(regi));

	nd = (int *)malloc(*nydays * sizeof(int));
	for(i = 0; i < *nydays; i++){
		k = 0;
		for(j = 0; j < ts->n; j++){
			if (yday[j] == ydayu[i])
				k++;
		}
		nd[i] = k;
		/*printf("%d, %d\n", ydayu[i], nd[i]);*/
	}
	
	for(i = 0; i < *nydays; i++){
		/*printf("%d\n",ydayu[i]);*/
		m = (float *)malloc(nd[i] * sizeof(float));
		k = 0;
		for(j = 0; j < ts->n; j++){
			if (yday[j] == ydayu[i]){
				m[k] = ts->var[j];
				k++;
			}
		}
		dre[i].x = ydayu[i];
		dre[i].mean = mean(m,nd[i]);
		dre[i].median = median(m,nd[i]);
		dre[i].std = stdDev(m,nd[i]);
		dre[i].max = maxval(m,nd[i]);
		dre[i].min = minval(m,nd[i]);
		free(m);
	}
		
	/*freeMemTs(ts);*/
	free(yday);
	free(ydayu);
	free(nd);
	return dre;
}

/*
Save a regime str (regi) into a cvs file
*/
int writeRegi2csv(const tfile *regif, const regi *dre, const int n){
	int i;
	FILE *fp;
	char *filename;

	/* Make up filename */
	if ((filename = malloc(strlen(regif->dirname) + strlen(regif->filename)+1)) == NULL){
		return -1;
	}
	strcpy(filename, regif->dirname);
	strcat(filename, regif->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"w")) == NULL){
		printf("Error writing %s stopping... \n", filename);
		return -1;
	}
	
	/* Save dre into csv file */
	fprintf(fp,"%s,%s,%s,%s,%s,%s\n","DAY","MEAN","MEDIAN","STD","MAX","MIN");
	for(i = 0; i < n; i++){
		fprintf(fp,"%d,%f,%f,%f,%f,%f\n", dre[i].x, dre[i].mean, dre[i].median, dre[i].std,dre[i].max, dre[i].min);
	}	

	fclose(fp);
	free(filename);

	return 0;
}

/*
Write the monthly time series (struct agg)
*/
int writeMonthlyTs2csv(const tfile *fi, const agg *ts, const int n){
	int i;
	FILE *fp;
	char *filename;

	/* Make up filename */
	if ((filename = malloc(strlen(fi->dirname) + strlen(fi->filename)+1)) == NULL){
		return -1;
	}
	strcpy(filename, fi->dirname);
	strcat(filename, fi->filename);
	
	/* Write into *.csv file */
	if ((fp = fopen(filename,"w")) == NULL){
		printf("Error writing %s stopping... \n", filename);
		return -1;
	}
	
	/* Save dre into csv file */
	fprintf(fp,"%s,%s,%s,%s,%s,%s,%s\n","YEAR", "MONTH", "MEAN", "MEDIAN", "STD", "MIN", "MAX");
	for(i = 0; i < n; i++){
		fprintf(fp,"%d,%d,%f,%f,%f,%f,%f\n", ts[i].dt.tm_year, ts[i].dt.tm_mon, ts[i].mean, ts[i].median, ts[i].std, ts[i].min, ts[i].max);
	}	

	fclose(fp);
	free(filename);

	return 0;

}

/*
Return an 1d array with the day of the year of the tserie struct ts. 
*/
int *dayOfYearTs(const ctserie *ts, const int n){
	int i;
	int *dyear;
	int dyear0;
	time_t si0, ds;
	struct tm da = { 0 };
	
	dyear = malloc(n * sizeof(int));
	da.tm_year = ts[0].dt.tm_year-IYEAR;
	da.tm_mon = ts[0].dt.tm_mon-1;
	da.tm_mday = ts[0].dt.tm_mday;
	si0 = mktime(&da);
	dyear0 = dayOfYear(&da);
	dyear[0] = dyear0;
	for(i = 1; i < n; i++){
		if(ts[i].dt.tm_year == ts[i-1].dt.tm_year){
			da.tm_year = ts[i].dt.tm_year-IYEAR;
			da.tm_mon = ts[i].dt.tm_mon-1;
			da.tm_mday = ts[i].dt.tm_mday;
			ds = mktime(&da) - si0;
			dyear[i] = dyear0 + ds/NSECDAY;
		}else{
			da.tm_year = ts[i].dt.tm_year-IYEAR;
			da.tm_mon = ts[i].dt.tm_mon-1;
			da.tm_mday = ts[i].dt.tm_mday;
			/*printf("year = %d, month = %d, day = %d\n", da.tm_year, da.tm_mon, da.tm_mday);*/
			/*printf("year = %d, month = %d, day = %d\n", ts->year[i], ts->month[i], ts->day[i]);*/
			dyear0 = dayOfYear(&da);
			/*da.tm_year = ts[i].dt.tm_year-IYEAR;*/
			si0 = mktime(&da);
			/*if (dyear0 == 0){
			//printf("year = %d , Day = %d, si0 = %ld\n",da.tm_year, dyear0, si0);
			}*/
			dyear[i] = dyear0;
		}
		/*if (dyear[i] == 364 || dyear[i] == 367){
			printf("dyear[%d] = %d\n",i,dyear[i]);
			exit(0);
		}*/
	}
	
	return dyear;
}


