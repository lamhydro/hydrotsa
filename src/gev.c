#include "gev.h"
#include "utils.h"
#include "freqa.h"
#include "stats.h"

/*
Fit Generalize Extreme Value (GEV) distribution using the Probabilily Weighted 
Moments (PWM) method: 
Estimation of the Generalized Extreme-Value Distribution by the Method of Probability-Weighted Moments
J. R. M. Hosking , J. R. Wallis & E. F. Wood
*/
pdfpar pwme_gev_pdf(float *x, int n){
	int i, r;
	pdfpar pdfp;
	double xs[n];
	double b[3];
	double c, k, a, e;
	double nf, nume, deno;

	for(i = 0; i < n; i++)
		xs[i] = x[i];
	ascenSort(xs, n);

	nf = (double)fact(n-1);
	for(r = 0; r < 3; r++){
		deno  = nf/(double)fact(n-r-1);
		nume = 0;
		for(i = 1; i <= n; i++){
			if((i-r-1)>=0)
				nume += (double)fact(i-1)*xs[i-1]/(double)fact(i-r-1);
		}
		b[r] = nume/(n*deno);
		//printf("b[r] = %lf, mean = %lf\n", b[r], mean(x,n));
	}

	c = (2*b[1]-b[0])/(3*b[2]-b[0]) - log(2)/log(3);

	k = 7.8590*c + 2.9554*c*c;
	a = ((2*b[1]-b[0])*k)/(lacz_gamma(1+k)*(1-pow(2.,-k)));
	e = b[0]+a*(lacz_gamma(1+k)-1)/k;
	pdfp.p3=k;
	pdfp.p2=a;
	pdfp.p1=e;

	return pdfp;
}

double logLike_gev(double e,double a, double k, float *x, int n){
	int i;
	double y, sly, syk;

	sly = 0;
	syk = 0;
	for(i = 0; i < n; i++){
		y = 1-(k/a)*(x[i]-e);
		sly +=log(y);
		syk +=pow(y,1/k);
	}

	return (-n*log(a) - (1 - 1/k)*sly - syk);
}

void jacov_logLike_gev(double e,double a, double k, float *x, int n, double *J){
	int i;
	double eq1, eq2, eq3;
	double y, yk, ya, au, ly, ya2;

	eq1 = 0;
	eq2 = -n/a;
	eq3 = 0;

	for(i = 0; i < n; i++){
		y = 1-(k/a)*(x[i]-e);
		yk = pow(y,1/k);
		ya = 1-k-yk;
			
		// Derivatives of dL/de
		eq1 += (1/a)*(ya/y); // dL/de

		// Derivatives of dL/da
		au = (x[i]-e)/a;
		eq2 += (1/a)*(ya*au/y); // dL/da

		// Derivatives of dL/dk
		ly = log(y);
		ya2 = 1-yk;
		eq3 += (-1/(k*k))*(ly*ya2 + ya*k*au/y); // dL/dk
	}
	// Derivatives to the log likelihood function (equation's system)
	J[0] = eq1;
	J[1] = eq2;
	J[2] = eq3;
}

void hessian_logLike_gev(double e,double a, double k, float *x, int n, double **H){
	int i;
	double y, dya, dye, dyk, yk, dyka, dyke, dykk, ya;
	double au, daua, daue;
	double ly, dlya, dlye, dlyk, ya2; 
	double deq1a = 0;
	double deq1e = 0;
	double deq1k = 0;
	double deq2a = n/(a*a);
	double deq2e = 0;
	double deq2k = 0;
	double deq3a = 0;
	double deq3e = 0;
	double deq3k = 0;

	for(i = 0; i < n; i++){
		y = 1-(k/a)*(x[i]-e);
		//y = -(k/a)*(x[i]-e);
		dya = k*(x[i]-e)/(a*a);
		dye = k/a;
		dyk = (e-x[i])/a;

		yk = pow(y,1/k);
		dyka = (1/k)*pow(y,1/k-1)*dya;
		dyke = (1/k)*pow(y,1/k-1)*dye;
		dykk = (-log(y)*yk/(k*k))+(yk*dyk)/(k*y);

		ya = 1-k-yk;
			
		// Derivatives of dL/de
		deq1e += (1/a)*(-dyke*y - dye*ya)/(y*y);
		deq1a += (1/a)*(-dyka*y - dya*ya)/(y*y) - (1/(a*a))*(ya/y);
		deq1k += (1/a)*((-1-dykk)*y - dyk*ya)/(y*y);

		// Derivatives of dL/da
		au = (x[i]-e)/a;
		daua = (e-x[i])/(a*a);
		daue = -1/a;
		deq2e += (1/a)*((ya/y)*daue - (dyke*y + dye*ya)*au/(y*y));
		deq2a += -(1/(a*a))*(ya*au/y) + (1/a)*((ya/y)*daua -(dyka*y+dya*ya)*au/(y*y));
		deq2k += (1/a)*(((-1-dykk)*y - dyk*ya)*au/(y*y));

		// Derivatives of dL/dk
		ly = log(y);
		dlya = (1/y)*dya;
		dlye = (1/y)*dye;
		dlyk = (1/y)*dyk;
		ya2 = 1-yk;
		//ya2 = 1-k-yk;
		deq3e += (-1/(k*k))*( -ly*dyke + dlye*ya2 + (ya/y)*k*daue + (-dyke*y-dye*ya)*k*au/(y*y) );
		deq3a += (-1/(k*k))*( -ly*dyka + dlya*ya2 + (ya*k*au/y) + (-dyka*y-dya*ya)*k*au/(y*y) );
		deq3k += (2/(k*k*k))*(ly*ya2 + ya*k*au/y) - (1/(k*k))*( ly*(-dykk) + dlyk*ya2 + ((-1-dykk)*y - dyk*ya)*k*au/(y*y) + au*ya/y );  
		//deq3k += (2/(k*k*k))*(ly*ya2 + ya*k*au/y) - (1/(k*k))*( ly*(-1-dykk) + dlyk*ya2 + ((-1-dykk)*y - dyk*ya)*k*au/(y*y) + au*ya/y );  

	}

	// Jacobian of the equation's systems(above) Hessian
	H[0][0] = deq1e;
	H[0][1] = deq1a;
	H[0][2] = deq1k;
	H[1][0] = deq2e;
	H[1][1] = deq2a;
	H[1][2] = deq2k;
	H[2][0] = deq3e;
	H[2][1] = deq3a;
	H[2][2] = deq3k;
}

/*
Fit Generalize Extreme Value (GEV) distribution to x using the Maximun Likelihood
method by:
Algorithm AS 215: Maximum-Likelihood Estimation of the Parameters of the Generalized Extreme-Value Distribution
J. R. M. Hosking, 1985. 
Generalized maximum-likelihood generalized extreme-valuequantile estimators for hydrologic data  
Martins and Stinger, 2000.
Notes: 
1. Usually k is in (0.5,  0.5).  
2. Parameters are initialized using the PWM
*/
pdfpar mle_gev_pdf(float *x, int n){
	int i, j, i1, i2;
	pdfpar pdfp;
	double a,e,k,a0,e0,k0;
	double J[3], J2[3], Hn[NROWS][NCOLS], diagH[3][3];
	double **H, **Hi, *delta, **Hn1, **diagHl;
	double error = 1.;
	double xmin, xmax;
	double d;
	double Li1, Li2, ymi, yma;
	double lambda=LAMB;
	double alpha = 1.0, alphak = 1, c1 = 0.8, c2 = 0.8;
	double JHJ, JHJ2;
	double mk, *aux;

	// Initialize the a (scale), e (location) and k (shape) parameters using
	// probability weighted moments (pwme)
	pdfp = pwme_gev_pdf(x, n);
	e = pdfp.p1; 
	a = pdfp.p2;
	k = pdfp.p3;
	//if (k<-1) k=-1;

	//a = stdDev(x,n)*sqrt(6)/M_PI; //pdfp.p2;
	//e = mean(x,n)-0.57721566*a; //pdfp.p1; 
	//k = 0.001; //pdfp.p3;
	//if (k>1) k=0.5;
	
	// Check if y = (1-(k/a)(x-e)) > 0	
	xmin = minval(x,n);
	xmax = maxval(x,n);
	if (k>0){
		if(xmax > (e+a/k)){
			printf("xmax = %lf > e+a/k = %lf\n", xmax, e+a/k);
			k = 0.99*a/(xmax-e);
			//break;
		}
	}
	if (k<0){
		if(xmin < (e+a/k)){
			printf("xmin = %lf < e+a/k = %lf\n", xmin, e+a/k);
			k = 1.01*a/(xmin-e);
			//break;
		}
	}
	//if( (1-k*(xmin-e)/a)<=0 || (1-k*(xmax-e)/a) ) printt  

	//printf("a,e,k = %lf, %lf, %lf\n", a, e, k);
	H = allocdmat(3, 3);
	Hi = allocdmat(3, 3);
	Hn1 = allocdmat(3, 3);
	diagHl = allocdmat(3, 3);
	j = 0;
	//while((error > ERR_GEV) || (j < NMAXITER)){
	//while(error > ERR_GEV){
	Li1 = logLike_gev(e,a,k,x,n);
	while (j < NMAXITER){
		//lambda=LAMB;
		jacov_logLike_gev(e, a, k, x, n, J);
		hessian_logLike_gev(e, a, k, x, n, H);
		for(i1 = 0; i1<3; i1++)
			for(i2 = 0; i2<3; i2++){
				Hn[i1][i2] = H[i1][i2];
				//Hn1[i1][i2] = H[i1][i2];
				if (i1 == i2)
					diagH[i1][i2] = H[i1][i2];
				else
					diagH[i1][i2] = 0;
			}
		
		printf("lambda = %lf\n",lambda);
		i = 0;
		while(i<MAXSR){
			printf("--------------------->i= %d\n",i);
			// Increment Hessian matrix
			Hn[0][0] = lambda*diagH[0][0] + H[0][0];
			Hn[1][1] = lambda*diagH[1][1] + H[1][1];
			Hn[2][2] = lambda*diagH[2][2] + H[2][2];
			//Hn1[0][0] = lambda*diagH[0][0] + H[0][0];
			//Hn1[1][1] = lambda*diagH[1][1] + H[1][1];
			//Hn1[2][2] = lambda*diagH[2][2] + H[2][2];
			//for(i1 = 0; i1<3; i1++)
			//	for(i2 = 0; i2<3; i2++)
			//		if (i1==i2)
			//			Hn[i1][i2] = H[i1][i2] + lambda;

			//for(int k1 = 0; k1<3; k1++){
			//	for(int k2=0; k2<3; k2++)
			//		printf("%lf  ", J[k1][k2]);
			//	printf("\n");
			//}
			//printf("\n");

			// Inverse of J
			d = determinant(Hn, 3);
			//printf("det = %lf",d);
			if (d == 0)
				printf("\nInverse of Entered Matrix is not possible\n");
			//else{
			cofactor(Hn, 3, Hi);
			//}
			//for(int k1 = 0; k1<3; k1++){
			//	for(int k2=0; k2<3; k2++)
			//		printf("%lf  ", Ji[k1][k2]);
			//	printf("\n");
			//}
			//printf("\n");

			// Newton-Raphson method. Update the parameters estimates
			delta = dMatVecMult(Hi, J, 3, 3);
			alpha = 1.; //logLike_gev(e-alpha*delta[1],a-alpha*delta[0],k-alpha*delta[2],x,n);
			if (alpha < 0) alpha = 0.2;
			//Jif[0] = 0.1*eq1;
			//Jif[1] = 0.1*eq2;
			//Jif[2] = 0.1*eq3;

			// Check limits of increments of the parameters
			//if (delta[0]<-0.25*signl(a)) delta[0]=-0.25*signl(a);
			//if (delta[0]>0.25*signl(a)) delta[0]=0.25*signl(a);
			//if (delta[1]<-0.5*signl(a)) delta[1]=-0.5*signl(a);
			//if (delta[1]>0.5*signl(a)) delta[1]=0.5*signl(a);
			//if (delta[2]<-0.2) delta[2]=-0.2;
			//if (delta[2]>0.2) delta[2]=0.2;
			e += alpha*delta[0];
			a += alpha*delta[1];
			k += alpha*delta[2];
			if (k>0){
				if(xmax > (e+a/k)){
					printf("xmax = %lf > e+a/k = %lf\n", xmax, e+a/k);
					k = 0.99*a/(xmax-e);
				}
			}
			if (k<0){
				if(xmin < (e+a/k)){
					printf("xmin = %lf < e+a/k = %lf\n", xmin, e+a/k);
					k = 1.01*a/(xmin-e);
				}
			}
			Li2 = logLike_gev(e,a,k,x,n);
			ymi = 1-k*(xmin-e)/a;
			yma = 1-k*(xmax-e)/a;
			//JHJ = J[0]*delta[0]+J[1]*delta[1]+J[2]*delta[2];
			//printf("------------JHJ = %lf\n",JHJ);
			//jacov_logLike_gev(e,a, k, x, n, J2);
			//JHJ2 = J2[0]*delta[0]+J2[1]*delta[1]+J2[2]*delta[2];
			//Condition 1
			for(i1 = 0; i1<3; i1++)
				for(i2 = 0; i2<3; i2++)
					diagHl[i1][i2] = lambda*diagH[i1][i2];	
			aux = dMatVecMult(diagHl, delta, 3, 3);
			aux[0] += J[0];
			aux[1] += J[1];
			aux[2] += J[2];
			//mk = Li1 + JHJ + 0.5*(delta[0]*aux[0]+delta[1]*aux[0]+delta[2]*aux[0]);
			alphak = (Li1 - Li2)/(aux[0]*delta[0]+aux[1]*delta[1]+aux[2]*delta[2]);
			printf("alphak = %lf\n",alphak);
			//if (Li2>Li1 && ymi>0. && yma>0){
			//if( Li2<=(Li1+c1*alphak*JHJ) && signl(JHJ2)>=c2*signl(JHJ) ){ 
			//if (Li2<Li1 && ymi>0. && yma>0){
			if (alphak>1.e-1 && ymi>0. && yma>0.){
				//printf("%lf > %lf\n", Li2, Li1);
				//lambda *= 0.1;
				lambda = MAX(lambda/9, 1.e-7);
				Li1 = Li2;
				break;
			}else{
				e -= alpha*delta[0];
				a -= alpha*delta[1];
				k -= alpha*delta[2];
				//lambda *=10.;
				lambda = MIN(lambda*11, 1.e7);
			} 
			i++;	
		}

		printf("j = %d, Li = %lf\n",j, Li1);
		// Check if y<=0 and Lim<Li
		//ymi = 1-k*(xmin-e)/a;
		//yma = 1-k*(xmax-e)/a;
		//i = 0;
		//while(ymi<=0 || yma<=0 || Lim<Li){
		//	printf("--------------------->i= %d\n",i);
		//	delta[0] *= SRF;
		//	delta[1] *= SRF;
		//	delta[2] *= SRF;
		//	
		//	a = a - delta[0];
		//	e = e - delta[1];
		//	k = k - delta[2];

		//	ymi = 1-k*(xmin-e)/a;
		//	yma = 1-k*(xmax-e)/a;
		//	Li = Lim;
		//	Lim = logLike_gev(e,a,k,x,n);
		//	if(i>MAXSR) break;
		//	i++;
		//}
		//printf("Li = %lf, Lim = %lf\n", Li, Lim);
		//printf("Jif = %lf, %lf, %lf\n", Jif[0], Jif[1], Jif[2]);
		//a0 = a;
		//e0 = e;
		//k0 = k;
		//		if (k>1) k=0.5;
		////if (k<-1) k=-1;

		printf("e,a,k = %lf, %lf, %lf\n", e, a, k);

		error = sqrt(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]);
		//error = sqrt(J[0]*J[0]+J[1]*J[1]+J[2]*J[2]);
		//eraek[0]=abs((a-a0)/a);
		//eraek[1]=abs((e-e0)/e);
		//eraek[2]=abs((k-k0)/k);
		//error=maxval(eraek,n)*100;
		printf("error = %f, ERR = %f\n",error, ERR_GEV);
		printf("J0 = %lf,J1 = %lf, J2 = %lf\n", J[0], J[1], J[2]);
		printf("Da = %lf, De = %lf, Dk = %lf\n", delta[0], delta[1], delta[2]);
		//if( signl(delta[0])<=(ACCA*a) && signl(delta[1])<=(ACCE*a) && signl(delta[2])<=ACCK ){
		if(error <= ERR_GEV && j>2){ 
			free(delta);
			break;
		}
		free(delta);
		free(aux);
		//if (j > NMAXITER) break;
		j += 1;
	}
	deallocdmat(H,3);
	deallocdmat(Hi,3);
	deallocdmat(Hn1,3);
	deallocdmat(diagHl,3);
	pdfp.p1 = e;
	pdfp.p2 = a;
	pdfp.p3 = k;
	
	return pdfp;
}

/*
Scalar product of vectors u and v of n elements.
*/
double dVectorScalarProduct(double *u, double *v, int n){
	int i;
	double s = 0;
	for(i = 0; i < n; i++) s +=u[i]*v[i];	
	return s;
}

void dVectorProduct(double *u, double *v, int n, double **uv){
	int i, j;
	
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			uv[i][j] = u[i]*v[j];
}

void BFGS(double *dx, double *dfx1, double *dfx2, int n, double **B, double **H){
	int i, j;
	double c1, c2;
	double y[n];
	double *Bdx;
	double ytdx, dxtBdx;
	double **yyt, **BdxBdxt, **dxyt, **ydxt, **dxdxt;

	yyt = allocdmat(n, n);
	BdxBdxt = allocdmat(n, n);
	dxyt = allocdmat(n, n);
	ydxt = allocdmat(n, n);
	dxdxt = allocdmat(n, n);

	for(i = 0; i < n; i++)
		y[i] = dfx2[i] - dfx1[i]; 

	ytdx = dVectorScalarProduct(y, dx, n);

	Bdx = dMatVecMult(B, dx, n, n);
	dxtBdx = dVectorScalarProduct(dx, Bdx, n);

	// B
	c1 = 1/ytdx;
	c2 = 1/dxtBdx;
	dVectorProduct(y, y, n, yyt);
	dVectorProduct(Bdx, Bdx, n, BdxBdxt);
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			B[i][j] += c1*yyt[i][j] - c2*BdxBdxt[i][j];
	
	// Inverse of B = H
	dVectorProduct(dx, y, n, dxyt);
	dVectorProduct(y, dx, n, ydxt);
	dVectorProduct(dx, dx, n, dxdxt);
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			H[i][j] = (1-c1*dxyt[i][j])*H[i][j]*(1-c1*ydxt[i][j]) + c1*dxdxt[i][j];
	
	free(Bdx);
	deallocdmat(yyt,n);
	deallocdmat(BdxBdxt,n);
	deallocdmat(dxyt,n);
	deallocdmat(ydxt,n);
	deallocdmat(dxdxt,n);
}

/*
Check if y = (1-(k/a)(x-e)) <= 0	
*/
void yisgt0(float *data, int n, double e, double a, double *k){
	double xmin, xmax;
	double kmin, kmax;

	xmin = minval(data,n);
	xmax = maxval(data,n);
	
	//if (*k > 0){
	//	if(xmax > (e+a/(*k)) ){
	//		printf("xmax = %lf > e+a/k = %lf\n", xmax, e+a/(*k));
	//		*k = 0.99*a/(xmax-e);
	//	}
	//}else if (*k < 0){
	//	if(xmin < (e+a/(*k)) ){
	//		printf("xmin = %lf < e+a/k = %lf\n", xmin, e+a/(*k));
	//		*k = 1.01*a/(xmin-e);
	//	}
	//}
	if( 1-(*k/a)*(xmin-e)<=0 && 1-(*k/a)*(xmax-e)<=0 ){
		kmin = 0.99*a/(xmin-e);
		kmax = 0.99*a/(xmax-e);

		if( 1-(kmin/a)*(xmin-e)<=0 && 1-(kmin/a)*(xmax-e)<=0 )
			*k = kmin;
		else if( 1-(kmax/a)*(xmin-e)<=0 && 1-(kmax/a)*(xmax-e)<=0 )
			*k = kmax;
		else
			*k = 1.e-3;
		printf("k = %lf\n", *k);
		//*k =  (0.99*a/(xmin-e)+0.99*a/(xmax-e))*0.5;
		//printf(" for xmin -> k=%lf, for xmax -> k=%lf\n",  0.99*a/(xmin-e), 0.99*a/(xmax-e));
		//if (*k<0) *k = -0.1;
		//if (*k>0) *k = 0.1;
		//if(1-(*k/a)*(xmin-e) <= 0){
		//	printf("Broken conditions %lf of xmin = %lf, e = %lf, a = %lf, k =%lf \n", 1-(*k/a)*(xmin-e), xmin, e, a, *k);
		//}
		//if(1-(*k/a)*(xmax-e) <= 0){
		//	printf("Broken conditions %lf of xmax = %lf, e = %lf, a = %lf, k =%lf \n", 1-(*k/a)*(xmax-e), xmax, e, a, *k);
		//}
	}
}

/*
BFGS Quasi-Newton algorithm to optimize the log-likelihood gev function and 
find the parameters.
*/
pdfpar quasiNewMet(float *data, int n){
	int i, j, i1, i2;
	pdfpar pdfp;
	double alpha;
	//double x[NPAR];
	double **B, **H;
	double fx1, fx2; 
	double dfx1[NPAR], dfx2[NPAR];
	double *dx;
	double dxdfx1, dxdfx2;
	double error;
	double e, a, k;

	// Initialize the e (location), a (scale) and k (shape) parameters using
	// probability weighted moments (pwme)
	pdfp = pwme_gev_pdf(data, n);
	e = pdfp.p1; // e 
	a = pdfp.p2; // a
	k = pdfp.p3; // k

	// Check if y = (1-(k/a)(x-e)) > 0	
	yisgt0(data, n, e, a, &k);
	
	// Evaluate function
	fx1 = logLike_gev(e, a, k, data, n);

	// Set initial value of B(Hessian) and its inverse H 
	B = allocdmat(NPAR, NPAR);
	H = allocdmat(NPAR, NPAR);
	for(i1 = 0; i1<NPAR; i1++)
		for(i2 = 0; i2<NPAR; i2++){
			B[i1][i2] = 1.;
			H[i1][i2] = 1.; // Inverse of B
		}

	// Estimate the gradient 
	jacov_logLike_gev(e, a, k, data, n, dfx1);
	
	// Set initial alpha value
	alpha = ALPHA0;

	// Main iteration
	i = 0;
	while (i < NMAXITER){
		printf("i = %d\n", i); 
		printf("e,a,k = %lf, %lf, %lf\n", e, a, k);
		
		// Estimate increment dx 
		dx = dMatVecMult(H, dfx1, NPAR, NPAR);
		for(i1 = 0; i1 < NPAR; i1++) dx[i1] *= -1; // Negative of the Hessian
		printf("dx1 = %lf, dx2 = %lf, dx3 = %lf\n", dx[0], dx[1], dx[2]);
		printf("fx = %lf\n", fx1);
		printf("alpha = %lf\n", alpha);

				//for(i1 = 0; i1<NPAR; i1++){
		//	for(i2 = 0; i2<NPAR; i2++)
		//		printf("H = %lf  ", H[i1][i2]);
		//	printf("\n");
		//}

		// Loop to accept the new step
		j = 0;
		while(j < MAXSR){
			// Condition to dx
			if (alpha*dx[0] > 0.5*a) dx[0] = 0.5*a/alpha;
			if (alpha*dx[0] < -0.5*a) dx[0] = -0.5*a/alpha;
			if (alpha*dx[1] > 0.25*a) dx[0] = 0.25*a/alpha;
			if (alpha*dx[1] < -0.25*a) dx[0] = -0.25*a/alpha;
			if (alpha*dx[2] > 0.2) dx[0] = 0.2/alpha;
			if (alpha*dx[2] < -0.2) dx[0] = -0.2/alpha;

			// Increment parameters
			e += alpha*dx[0];
			a += alpha*dx[1];
			k += alpha*dx[2];
			
			// Check if y = (1-(k/a)(x-e)) > 0	
			yisgt0(data, n, e, a, &k);
			
			//printf("j = %d\n", j); 
			// Evaluate new function
			fx2 = logLike_gev(e, a, k, data, n);

			// Estimate the new gradient	
			jacov_logLike_gev(e, a, k, data, n, dfx2);
			//printf("dfx2_0 = %lf, dfx2_1 = %lf, dfx2_2 = %lf\n", dfx2[0], dfx2[1],dfx2[2]);
			
			// Strong Wolf condition
			dxdfx1 = dx[0]*dfx1[0] + dx[1]*dfx1[1] + dx[2]*dfx1[2];
			dxdfx2 = dx[0]*dfx2[0] + dx[1]*dfx2[1] + dx[2]*dfx2[2];
			//if( fx2<=(fx1+C1*alpha*dxdfx1) && signl(dxdfx2)<=C2*signl(dxdfx1) ){   
			if( fx2>=(fx1+C1*alpha*dxdfx1) && signl(dxdfx2)>=C2*signl(dxdfx1) ){   
				// Update B
				BFGS(dx, dfx1, dfx2, NPAR, B, H);

				printf("fx1 = %lf, fx2 = %lf\n", fx1, fx2);
				fx1 = fx2;
				for(i1 = 0; i1 < NPAR; i1++) dfx1[i1] = dfx2[i1];
				//alpha *=0.1;
				alpha = MAX(alpha/9, 1e-7);
				break;
			}else{
				// Reverse increments	
				e -= alpha*dx[0];
				a -= alpha*dx[1];
				k -= alpha*dx[2];

				// Check if y = (1-(k/a)(x-e)) > 0	
				yisgt0(data, n, e, a, &k);

				// Change alpha
				//alpha *=10;
				alpha = MIN(alpha*11, 1e7);
			}
			j++;
		}

		error = sqrt(alpha*dx[0]*dx[0]+alpha*dx[1]*dx[1]+alpha*dx[2]*dx[2]);
		printf("error = %f, ERR = %f\n",error, ERR_GEV);
		if(error <= ERR_GEV && i>2){ 
			free(dx);
			break;
		}
		free(dx);
		i++;
	}
	deallocdmat(B,NPAR);
	deallocdmat(H,NPAR);

	pdfp.p1 = e;
	pdfp.p2 = a;
	pdfp.p3 = k;
	
	return pdfp;
}

/*
Levenberg-Marquardt algorithm to optimize the log-likelihood gev function and 
find the parameters e, a & k..
*/
pdfpar levenbergMarquardtMet(float *data, int n){
	int i, j, i1, i2;
	pdfpar pdfp;
	double lambda;
	double Hn[NROWS][NCOLS];
	double **H, **Hi;
	double fx1, fx2; 
	double dfx1[NPAR], dfx2[NPAR];
	double *dx;
	double dxdfx1, dxdfx2;
	double error;
	double e, a, k;

	// Initialize the e (location), a (scale) and k (shape) parameters using
	// probability weighted moments (pwme)
	pdfp = pwme_gev_pdf(data, n);
	e = pdfp.p1; // e 
	a = pdfp.p2; // a
	k = pdfp.p3; // k

	// Check if y = (1-(k/a)(x-e)) > 0	
	yisgt0(data, n, e, a, &k);
	
	// Evaluate function
	fx1 = logLike_gev(e, a, k, data, n);

	// Hessian (Initial value) 
	H = allocdmat(NPAR, NPAR);
	Hi = allocdmat(NPAR, NPAR);
	hessian_logLike_gev(e, a, k, data, n, H);

	// Estimate the gradient 
	jacov_logLike_gev(e, a, k, data, n, dfx1);
	
	// Set initial lambda
	lambda = LAMB;

	// Main iteration
	i = 0;
	while (i < NMAXITER){
		printf("i = %d\n", i); 
		printf("e,a,k = %lf, %lf, %lf\n", e, a, k);

		//printf("fx = %lf\n", fx1);
		//printf("alpha = %lf\n", alpha);

		// Copy H into Hn 
		for(i1 = 0; i1<NPAR; i1++)
			for(i2 = 0; i2<NPAR; i2++)
					if (i1 != i2)
						Hn[i1][i2] = H[i1][i2];	
				
		// Loop to accept the new step
		j = 0;
		while(j < MAXSR){
			//printf("j = %d\n", j); 

			// Modified Hn=H diagonal
			for(i1 = 0; i1<NPAR; i1++)
					Hn[i1][i1] = H[i1][i1] + lambda*H[i1][i1];
						
			// Hessian's inverse Hi
			cofactor(Hn, NPAR, Hi);

			// Estimate increment dx 
			dx = dMatVecMult(Hi, dfx1, NPAR, NPAR);
			for(i1 = 0; i1 < NPAR; i1++) dx[i1] *= -1; // Negative of the Hessian
		
			// Condition to dx
			//if (alpha*dx[0] > 0.5*a) dx[0] = 0.5*a/alpha;
			//if (alpha*dx[0] < -0.5*a) dx[0] = -0.5*a/alpha;
			//if (alpha*dx[1] > 0.25*a) dx[0] = 0.25*a/alpha;
			//if (alpha*dx[1] < -0.25*a) dx[0] = -0.25*a/alpha;
			//if (alpha*dx[2] > 0.2) dx[0] = 0.2/alpha;
			//if (alpha*dx[2] < -0.2) dx[0] = -0.2/alpha;

			// Increment parameters
			e += dx[0];
			a += dx[1];
			k += dx[2];
			
			// Check if y = (1-(k/a)(x-e)) > 0	
			yisgt0(data, n, e, a, &k);
			
			// Evaluate new function
			fx2 = logLike_gev(e, a, k, data, n);

			// Estimate the new gradient	
			jacov_logLike_gev(e, a, k, data, n, dfx2);
			//printf("dfx2_0 = %lf, dfx2_1 = %lf, dfx2_2 = %lf\n", dfx2[0], dfx2[1],dfx2[2]);
			
			if (fx2>fx1){
				// Hessian (Initial value) 
				hessian_logLike_gev(e, a, k, data, n, H);
				
				printf("dx1 = %lf, dx2 = %lf, dx3 = %lf\n", dx[0], dx[1], dx[2]);
				printf("fx1 = %lf, fx2 = %lf\n", fx1, fx2);
				fx1 = fx2;
				for(i1 = 0; i1 < NPAR; i1++) dfx1[i1] = dfx2[i1];
				//lambda *=0.1;
				lambda = MAX(lambda/9, 1e-7);
				break;
			}else{
				// Reverse increments	
				e -= dx[0];
				a -= dx[1];
				k -= dx[2];

				// Check if y = (1-(k/a)(x-e)) > 0	
				yisgt0(data, n, e, a, &k);

				// Change alpha
				//lambda *=10;
				lambda = MIN(lambda*11, 1e7);
			}
			j++;
		}

		error = sqrt(dx[0]*dx[0]+dx[1]*dx[1]+dx[2]*dx[2]);
		printf("error = %f, ERR = %f\n",error, ERR_GEV);
		if(error <= ERR_GEV && i>20){ 
			free(dx);
			break;
		}
		free(dx);
		i++;
	}
	deallocdmat(H,NPAR);
	deallocdmat(Hi,NPAR);

	pdfp.p1 = e;
	pdfp.p2 = a;
	pdfp.p3 = k;
	
	return pdfp;
}
//pdfpar mle_gev_pdf2(float *x, int n){
//	int i, j, i1, i2;
//	pdfpar pdfp;
//	double a,e,k,a0,e0,k0;
//	double eq1, eq2, eq3;
//	double deq1a, deq1e, deq1k, deq2a, deq2e, deq2k, deq3a, deq3e, deq3k;
//	double y, dya, dye, dyk;
//	double yk, dyka, dyke, dykk;
//	double ya, ya2;
//	double au, daua, daue;
//	double ly, dlya, dlye, dlyk;
//	double J[3], H[NROWS][NCOLS], Hn[NROWS][NCOLS];
//	double *delta;
//	double error = 1.;
//	double xmin, xmax;
//	double eraek[3];
//	//double sly, syk, L;
//	double d;
//	double Li1, Li2, ymi, yma;
//	double lambda=LAMB;
//	double alpha = 1.0;
//
//	// Initialize the a (scale), e (location) and k (shape) parameters using
//	// probability weighted moments (pwme)
//	pdfp = pwme_gev_pdf(x, n);
//	a = pdfp.p2;
//	e = pdfp.p1; 
//	k = pdfp.p3;
//	//if (k<-1) k=-1;
//
//	//a = stdDev(x,n)*sqrt(6)/M_PI; //pdfp.p2;
//	//e = mean(x,n)-0.57721566*a; //pdfp.p1; 
//	//k = 0.001; //pdfp.p3;
//	//if (k>1) k=0.5;
//	
//	// Check if y = (1-(k/a)(x-e)) > 0	
//	xmin = minval(x,n);
//	xmax = maxval(x,n);
//	if (k>0){
//		if(xmax > (e+a/k)){
//			printf("xmax = %lf > e+a/k = %lf\n", xmax, e+a/k);
//			k = 0.99*a/(xmax-e);
//			//break;
//		}
//	}
//	if (k<0){
//		if(xmin < (e+a/k)){
//			printf("xmin = %lf < e+a/k = %lf\n", xmin, e+a/k);
//			k = 1.01*a/(xmin-e);
//			//break;
//		}
//	}
//
//	j = 0;
//	//while((error > ERR_GEV) || (j < NMAXITER)){
//	//while(error > ERR_GEV){
//	Li1 = logLike_gev(e,a,k,x,n);
//	while (j < NMAXITER){
//		
//		eq1 = 0;
//		eq2 = -n/a;
//		eq3 = 0;
//
//		for(i = 0; i < n; i++){
//			y = 1-(k/a)*(x[i]-e);
//			yk = pow(y,1/k);
//			ya = 1-k-yk;
//			
//			// Derivatives of dL/de
//			eq1 += (1/a)*(ya/y); // dL/de
//
//			// Derivatives of dL/da
//			au = (x[i]-e)/a;
//			eq2 += (1/a)*(ya*au/y); // dL/da
//
//			// Derivatives of dL/dk
//			ly = log(y);
//			ya2 = 1-yk;
//			eq3 += (-1/(k*k))*(ly*ya2 + ya*k*au/y); // dL/dk
//		}
//		// Derivatives to the log likelihood function (equation's system)
//		J[0] = eq1;
//		J[1] = eq2;
//		J[2] = eq3;
//		//printf("a,e,k = %lf, %lf, %lf\n", a, e, k);
//		printf("J0 = %lf , J1 = %lf, J2 = %lf\n", J[0], J[1], J[2]); 
//
//		// Jacobian of the equation's systems(above) Hessian
//		H[0][0] = deq1a;
//		H[0][1] = deq1e;
//		H[0][2] = deq1k;
//		H[1][0] = deq2a;
//		H[1][1] = deq2e;
//		H[1][2] = deq2k;
//		H[2][0] = deq3a;
//		H[2][1] = deq3e;
//		H[2][2] = deq3k;
//
//		for(i1 = 0; i1<3; i1++)
//			for(i2 = 0; i2<3; i2++)
//				Hn[i1][i2] = H[i1][i2];
//		printf("lambda = %lf\n",lambda);
//		i = 0;
//		while(i<MAXSR){
//			printf("--------------------->i= %d\n",i);
//			//printf("lambda = %lf\n",lambda);
//			// Increment Hessian matrix
//			Hn[0][0] = lambda*H[0][0] + H[0][0];
//			Hn[1][1] = lambda*H[1][1] + H[1][1];
//			Hn[2][2] = lambda*H[2][2] + H[2][2];
//			//for(i1 = 0; i1<3; i1++)
//			//	for(i2 = 0; i2<3; i2++)
//			//		if (i1==i2)
//			//			Hn[i1][i2] = H[i1][i2] + lambda;
//
//			//for(int k1 = 0; k1<3; k1++){
//			//	for(int k2=0; k2<3; k2++)
//			//		printf("%lf  ", J[k1][k2]);
//			//	printf("\n");
//			//}
//			//printf("\n");
//
//			// Inverse of J
//			d = determinant(Hn, 3);
//			//printf("det = %lf",d);
//			if (d == 0)
//				printf("\nInverse of Entered Matrix is not possible\n");
//			//else{
//			cofactor(Hn, 3, Hi);
//			//}
//			//for(int k1 = 0; k1<3; k1++){
//			//	for(int k2=0; k2<3; k2++)
//			//		printf("%lf  ", Ji[k1][k2]);
//			//	printf("\n");
//			//}
//			//printf("\n");
//
//			// Newton-Raphson method. Update the parameters estimates
//			delta = dMatVecMult(Hi, J, 3, 3);
//			alpha = 1.; //logLike_gev(e-alpha*delta[1],a-alpha*delta[0],k-alpha*delta[2],x,n);
//			if (alpha < 0) alpha = 0.2;
//			//Jif[0] = 0.1*eq1;
//			//Jif[1] = 0.1*eq2;
//			//Jif[2] = 0.1*eq3;
//
//			// Check limits of increments of the parameters
//			//if (delta[0]<-0.25*signl(a)) delta[0]=-0.25*signl(a);
//			//if (delta[0]>0.25*signl(a)) delta[0]=0.25*signl(a);
//			//if (delta[1]<-0.5*signl(a)) delta[1]=-0.5*signl(a);
//			//if (delta[1]>0.5*signl(a)) delta[1]=0.5*signl(a);
//			//if (delta[2]<-0.2) delta[2]=-0.2;
//			//if (delta[2]>0.2) delta[2]=0.2;
//			a -= alpha*delta[0];
//			e -= alpha*delta[1];
//			k -= alpha*delta[2];
//			if (k>0){
//				if(xmax > (e+a/k)){
//					printf("xmax = %lf > e+a/k = %lf\n", xmax, e+a/k);
//					k = 0.99*a/(xmax-e);
//				}
//			}
//			if (k<0){
//				if(xmin < (e+a/k)){
//					printf("xmin = %lf < e+a/k = %lf\n", xmin, e+a/k);
//					k = 1.01*a/(xmin-e);
//				}
//			}
//			Li2 = logLike_gev(e,a,k,x,n);
//			ymi = 1-k*(xmin-e)/a;
//			yma = 1-k*(xmax-e)/a;
//			if (Li2>Li1 && ymi>0. && yma>0){
//				//printf("%lf > %lf\n", Li2, Li1);
//				lambda *= 0.1;
//				//lambda = MAX(lambda/9, 1.e-7);
//				Li1 = Li2;
//				break;
//			}else{
//				a += alpha*delta[0];
//				e += alpha*delta[1];
//				k += alpha*delta[2];
//				lambda *=10.;
//				//lambda = MIN(lambda*11, 1.e7);
//			} 
//			i++;	
//		}
//
//		printf("j = %d, Li = %lf\n",j, Li1);
//		// Check if y<=0 and Lim<Li
//		//ymi = 1-k*(xmin-e)/a;
//		//yma = 1-k*(xmax-e)/a;
//		//i = 0;
//		//while(ymi<=0 || yma<=0 || Lim<Li){
//		//	printf("--------------------->i= %d\n",i);
//		//	delta[0] *= SRF;
//		//	delta[1] *= SRF;
//		//	delta[2] *= SRF;
//		//	
//		//	a = a - delta[0];
//		//	e = e - delta[1];
//		//	k = k - delta[2];
//
//		//	ymi = 1-k*(xmin-e)/a;
//		//	yma = 1-k*(xmax-e)/a;
//		//	Li = Lim;
//		//	Lim = logLike_gev(e,a,k,x,n);
//		//	if(i>MAXSR) break;
//		//	i++;
//		//}
//		//printf("Li = %lf, Lim = %lf\n", Li, Lim);
//		//printf("Jif = %lf, %lf, %lf\n", Jif[0], Jif[1], Jif[2]);
//		//a0 = a;
//		//e0 = e;
//		//k0 = k;
//		//		if (k>1) k=0.5;
//		////if (k<-1) k=-1;
//
//		printf("e,a,k = %lf, %lf, %lf\n", e, a, k);
//
//		//if (k>0){
//		//	if(xmax > (e+a/k)){
//		//		//printf("xmax = %lf > e+a/k = %lf\n", xmax, e+a/k);
//		//		k = 0.95*a/(xmax-e);
//		//		//break;
//		//	}
//		//}
//		//if (k<0){
//		//	if(xmin < (e+a/k)){
//		//		//printf("xmin = %lf < e+a/k = %lf\n", xmin, e+a/k);
//		//		k = 1.05*a/(xmin-e);
//		//		//break;
//		//	}
//		//}
//		
//		error = sqrt(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]);
//		//error = sqrt(J[0]*J[0]+J[1]*J[1]+J[2]*J[2]);
//		//eraek[0]=abs((a-a0)/a);
//		//eraek[1]=abs((e-e0)/e);
//		//eraek[2]=abs((k-k0)/k);
//		//error=maxval(eraek,n)*100;
//		printf("error = %f, ERR = %f\n",error, ERR_GEV);
//		printf("Da = %lf, De = %lf, Dk = %lf\n", delta[0], delta[1], delta[2]);
//		//if( signl(delta[0])<=(ACCA*a) && signl(delta[1])<=(ACCE*a) && signl(delta[2])<=ACCK ){
//		if(error <= ERR_GEV && j>2){ 
//			free(delta);
//			break;
//		}
//		free(delta);
//		//if (j > NMAXITER) break;
//		j += 1;
//	}
//	deallocdmat(Hi,3);
//	pdfp.p1 = e;
//	pdfp.p2 = a;
//	pdfp.p3 = k;
//	
//	return pdfp;
//}


/*
Return the probability p of x based on a GEV distribution with parameters mi, sigma and xi.
*/
double *gev_pdf(pdfpar *pdfp, float *x, int n){
	int i;
	double mi, sigma, xi, xs, t;
	double *p;

	p = (double *)malloc(n * sizeof(double));
	//mi = pdp->loc;
	//xi = pdp->shape;
	//sigma = pdp->scale;
	mi = pdfp->p1;
	sigma = pdfp->p2;
	xi = pdfp->p3;
	for(i = 0; i < n; i++){
		//xs = standarization(x[i],mi,sigma);
		xs = (x[i]-mi)/sigma;
		if (xi == 0)
			t = exp(-xs); 
		else
			t = pow(1+xi*xs,-1/xi);
		p[i] = (1/sigma)*pow(t,xi+1)*exp(-t);
	}
	return p;
}
/*
Return the cumulative probability p of x based on a GEV distribution with parameters mi, sigma and xi.
*/
double *gev_cdf(pdfpar *pdfp, float *x, int n){
	int i;
	double mi, sigma, xi, xs, t;
	double *p;

	p = (double *)malloc(n * sizeof(double));
	//mi = pdp->loc;
	//xi = pdp->shape;
	//sigma = pdp->scale;
	mi = pdfp->p1;
	sigma = pdfp->p2;
	xi = pdfp->p3;

	for(i = 0; i < n; i++){
		//xs = standarization(x[i],mi,sigma);
		xs = (x[i]-mi)/sigma;
		if (xi == 0)
			t = exp(-xs); 
		else
			t = pow(1+xi*xs,-1/xi);
		p[i] = exp(-t);
	}
	return p;
}


