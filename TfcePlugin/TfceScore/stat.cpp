// statistic routines used for creation of maps etc. (nach Press et al. 1992)
//

#include <math.h>

#include <stdlib.h>
#if defined(OS_WIN32)
#include <malloc.h>
#endif

#include "stat.h"

#if defined(OS_WIN32)
//#include <mkl.h>
#endif


static int mfit;
static float ochisq,*atry,*beta,*da,**oneda;

void ttest(float data1[], unsigned long n1, float data2[], unsigned long n2, float *t, float *prob)
{
	float var1, var2, svar, df, ave1, ave2;

	avevar(data1, n1, &ave1, &var1);
	avevar(data2, n2, &ave2, &var2);

	df = n1 + n2 - 2;
	svar=((n1-1)*var1 + (n2-1)*var2)/df;
	if(svar == 0) // "Fehler" bei t-tests abfangen: beide Varianzen sind 0 -> *t = 0/0  !!!
		*t = 0;
	else
		*t = (ave1-ave2)/sqrt(svar*(1.0/n1 + 1.0/n2));

	// prob nicht berechnen, da für statistical map *d-Wert ausreichend
	//*prob = betai(0.5*df, 0.5, df/(df + (*t) * (*t)));
}

void tutest(float data1[], unsigned long n1, float data2[], unsigned long n2, float *t, float *prob)
{
	float var1, var2, df, ave1, ave2;

	avevar(data1,n1,&ave1,&var1);
	avevar(data2,n2,&ave2,&var2);

	if((var1 == 0.0f) && (var2 == 0.0f)) // "Fehler" bei t-tests abfangen: beide Varianzen sind 0 -> *t = 0/0  !!!
		*t = 0;
	else
		*t=(ave1-ave2)/sqrt(var1/n1+var2/n2);

	// prob nicht berechnen, da für statistical map *d-Wert ausreichend
	//df=SQR(var1/n1+var2/n2)/(SQR(var1/n1)/(n1-1)+SQR(var2/n2)/(n2-1));
	//*prob=betai(0.5*df,0.5,df/(df+SQR(*t)));
}


void kstwo(float data1[], unsigned long n1, float data2[], unsigned long n2, float *d, float *prob)
{
	unsigned long j1=1,j2=1;
	float d1,d2,dt,en1,en2,en,fn1=0.0,fn2=0.0;
	sort(n1,data1);
	sort(n2,data2);
	en1=n1;
	en2=n2;
	*d=0.0;
	while (j1 <= n1 && j2 <= n2) {
		if ((d1=data1[j1]) <= (d2=data2[j2])) fn1=j1++/en1;
		if (d2 <= d1) fn2=j2++/en2;
		if ((dt=fabs(fn2-fn1)) > *d) *d=dt;
	}
	// prob nicht berechnen, da für statistical map *d-Wert ausreichend
	//en=sqrt(en1*en2/(en1+en2));
	//*prob=probks((en+0.12+0.11/en)*(*d));
}


#define TINY 1.0e-20
void pearsn(float x[], float y[], unsigned long n, float *r, float *prob, float *z)
{
	unsigned long j;

	float yt, xt, t, df;
	float syy=0.0, sxy=0.0, sxx=0.0, ay=0.0, ax=0.0, div;

	for(j=1; j<=n; j++)
	{
		ax += x[j];
		ay += y[j];
	}
	ax /= n;
	ay /= n;

	for(j=1; j<=n; j++)
	{
		xt = x[j] - ax;
		yt = y[j] - ay;
		sxx += xt*xt;
		syy += yt*yt;
		sxy += xt*yt;
	}

	div = sqrt(sxx*syy);
	if(div >0.0f)
		*r = sxy/div;
	else
		*r = 0.0f;
	//*z = 0.5*log((1.0+(*r)+TINY)/(1.0-(*r)+TINY));  // Fisher's z transformation
	//df = n - 2;
	//t = (*r)*sqrt(df/((1.0-(*r)+TINY)*(1.0+(*r)+TINY)));
	//*prob = betai(0.5*df, 0.5, df/(df+t*t));
/*  *prob = erfcc... */
}

// incremental version for real-time fMRI
void pearsn_inc(float x, float y, float& n, float& ax, float& ay, float& sxx, float& syy, float& sxy, float *r)
{
	unsigned long j;

	float yt, xt, t, df, div;

	n  += 1.0f;
	ax += x;
	ay += y;

	xt = x - ax/n;
	yt = y - ay/n;
	sxx += xt*xt;
	syy += yt*yt;
	sxy += xt*yt;

	div = sqrt(sxx*syy);
	if(div >0.0f)
		*r = sxy/div;
	else
		*r = 0.0f;
}

// incremental linear regression for real-time fMRI
void regr_inc(float x, float y, float& n, float& ax, float& ay, float& sxx, float& syy, float& sxy, float *a, float *b, float *ym)
{
	unsigned long j;

	float yt, xt, t, df, div;

	n  += 1.0f;

	ax += x;
	ay += y;

	xt = x - ax/n;
	yt = y - ay/n;
	sxx += xt*xt;
	syy += yt*yt;
	sxy += xt*yt;

	div = sxx;
	if(div >0.0f)
		*b = sxy/div;
	else
		*b = 0.0f;

	*ym = ay/n;

	*a = *ym - *b * ax/n;
}

void pearsn_prob(float x[], float y[], unsigned long n, float *r, float *prob, float *z)
{
	unsigned long j;

	float yt, xt, t, df;
	float syy=0.0, sxy=0.0, sxx=0.0, ay=0.0, ax=0.0, div;

	for(j=1; j<=n; j++)
	{
		ax += x[j];
		ay += y[j];
	}
	ax /= n;
	ay /= n;

	for(j=1; j<=n; j++)
	{
		xt = x[j] - ax;
		yt = y[j] - ay;
		sxx += xt*xt;
		syy += yt*yt;
		sxy += xt*yt;
	}

	div = sqrt(sxx*syy);
	if(div >0.0f)
		*r = sxy/sqrt(sxx*syy);
	else
		*r = 0.0f;
	// *z = 0.5*log((1.0+(*r)+TINY)/(1.0-(*r)+TINY));  // Fisher's z transformation
	df = n - 2;
	t = (*r)*sqrt(df/((1.0-(*r)+TINY)*(1.0+(*r)+TINY)));
	*prob = betai(0.5*df, 0.5, df/(df+t*t));
/*  *prob=erfcc(fabs((*z)*sqrt(n-1.0))/1.4142136); // schneller für großes n */
}


float r_to_p(float r, unsigned long n)
{
	float t, prob, df;
	
	df = (float)(n - 2);

	if(df < 0.0f)
		return 1.0f;

	if(r == 0.0f)
		return 1.0f;

	if(r == 1.0f)
		return 0.0f;

	// r in t-Wert umrechnen
	t = r*sqrt(df/((1.0-r+TINY)*(1.0+r+TINY)));
	
	// korrespondierender p-Wert:
	prob = betai(0.5*df, 0.5, df/(df+t*t));
	return prob;
}

float t_to_p(float t, unsigned long df)
{
	float prob;
	
	// korrespondierender p-Wert:
	prob = betai(0.5f*df, 0.5f, df/(df+t*t));
	return prob;
}

// don't know how to do the inverted fn, so we use brute force method..
float p_to_t(float p, unsigned long df)
{
	float t, ptmp, t_step;
	int i, dir, last_dir = 1;
	
	t = 4.0f;
	t_step = 2.0f;

	for(i=0; i<25; i++)  // 25 iterations should be enough
	{
		ptmp = t_to_p(t, df);

		if(ptmp < p) 
		{ 
			t -= t_step; 
			dir = -1;
			if(last_dir == +1)  // if we turned direction, lower step size
				t_step /= 2.0f;
			last_dir = -1;
		}
		else 
		{
			t += t_step;
			dir = 1;
			if(last_dir == -1)  // if we turned direction, lower step size
				t_step /= 2.0f;
			last_dir = 1;
		}
	}

	return t;
}

// don't know how to do the inverted fn, so we use brute force method..
float p_to_r(float p, unsigned long df)
{
	float r, ptmp, r_step;
	int i, dir, last_dir = 1;
	
	r = 0.4f;
	r_step = 0.2f;
	
	for(i=0; i<25; i++)  // 25 iterations should be enough
	{
		ptmp = r_to_p(r, df);
		
		if(ptmp < p) 
		{ 
			r -= r_step; 
			dir = -1;
			if(last_dir == +1)  // if we turned direction, lower step size
				r_step /= 2.0f;
			last_dir = -1;
		}
		else 
		{
			r += r_step;
			dir = 1;
			if(last_dir == -1)  // if we turned direction, lower step size
				r_step /= 2.0f;
			last_dir = 1;
		}
	}
	
	return r;
}

// don't know how to do the inverted fn, so we use brute force method..
float p_to_F(float p, unsigned long df_1, unsigned long df_2)
{
	float F, ptmp, F_step;
	int i, dir, last_dir = 1;
	
	F = 10.0f;
	F_step = 2.0f;
	
	for(i=0; i<25; i++)  // 25 iterations should be enough
	{
		ptmp = F_to_p(F, df_1, df_2);
		
		if(ptmp < p) 
		{ 
			F -= F_step; 
			dir = -1;
			if(last_dir == +1)  // if we turned direction, lower step size
				F_step /= 2.0f;
			last_dir = -1;
		}
		else 
		{
			F += F_step;
			dir = 1;
			if(last_dir == -1)  // if we turned direction, lower step size
				F_step /= 2.0f;
			last_dir = 1;
		}
	}
	
	return F;
}

float F_to_p(float F, unsigned long df_1, unsigned long df_2)
{
	float prob;
	
	if(F < 0.0f)
		return 0.0f;

	// korrespondierender p-Wert:
	prob = betai(0.5f*df_2, 0.5f*df_1, df_2/(df_2 + df_1*F));
	return prob;
}

void spear(float data1[], float data2[], unsigned long n, float *d, float *zd, float *probd, float *rs, float *probrs)
{
	unsigned long j;
	float vard,t,sg,sf,fac,en3n,en,df,aved,*wksp1,*wksp2;
	wksp1=vector(1,n);
	wksp2=vector(1,n);
	for (j=1;j<=n;j++) {
		wksp1[j]=data1[j];
		wksp2[j]=data2[j];
	}
	sort2(n,wksp1,wksp2);
	crank(n,wksp1,&sf);
	sort2(n,wksp2,wksp1);
	crank(n,wksp2,&sg);
	*d=0.0;
	for (j=1;j<=n;j++)
		*d += SQR(wksp1[j]-wksp2[j]);
	en=n;
	en3n=en*en*en-en;
	aved=en3n/6.0-(sf+sg)/12.0;
	fac=(1.0-sf/en3n)*(1.0-sg/en3n);
	vard=((en-1.0)*en*en*SQR(en+1.0)/36.0)*fac;
if((vard == 0.0f) || (fac == 0.0f))
{
	*rs = 0.0f;
	*probrs = 0.0f;
	return;
}
	*zd=(*d-aved)/sqrt(vard);
	*probd=erfcc(fabs(*zd)/1.4142136);
	*rs=(1.0-(6.0/en3n)*(*d+(sf+sg)/12.0))/sqrt(fac);
	/*
	fac=(*rs+1.0)*(1.0-(*rs));
	if (fac > 0.0) {
		t=(*rs)*sqrt((en-2.0)/fac);
		df=en-2.0;
		*probrs=betai(0.5*df,0.5,df/(df+t*t));
	} else
		*probrs=0.0;   */
	free_vector(wksp2,1,n);
	free_vector(wksp1,1,n);
}

float betai(float a, float b, float x)
{
	float bt;

	if(x < 0.0 || x > 1.0)
	{
		return 0.0;
		// CriticalErrorMessageBox("FATAL ERROR: Bad x in routine betai.");
		exit(1);
	}
	if(x == 0.0 || x == 1.0) 
		bt = 0.0;
	else
		bt = exp(gammln(a+b)-gammln(a)-gammln(b)+a*log(x)+b*log(1.0-x));
	
	if(x < (a+1.0)/(a+b+2.0))
		return bt*betacf(a, b, x)/a;
	else
		return 1.0 - bt*betacf(b, a, 1.0-x)/b;
}


#define MAXIT 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30

float betacf(double a, double b, double x)
{
	int m, m2;
	double aa, c, d, del, h, qab, qam, qap;

	qab = a+b;
	qap = a+1.0;
	qam = a-1.0;
	c = 1.0;
	d = 1.0-qab*x/qap;
	if(fabs(d) < FPMIN) d = FPMIN;
	d = 1.0/d;
	h=d;
	for(m=1; m<=MAXIT; m++)
	{
		m2 = 2*m;
		aa = m*(b-m)*x/((qam+m2)*(a+m2));
		d=1.0+aa*d;
		if(fabs(d) < FPMIN) d = FPMIN;
		c = 1.0+aa/c;
		if(fabs(c) < FPMIN) c = FPMIN;
		d = 1.0/d;
		h *= d*c;
		aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
		d = 1.0+aa*d;
		if(fabs(d) < FPMIN) d = FPMIN;
		c = 1.0+aa/c;
		if(fabs(c) < FPMIN) c = FPMIN;
		d = 1.0/d;
		del = d*c;
		h *= del;
		if(fabs(del-1.0) < EPS) break;
	}

	if(m > MAXIT)
	{
		// CriticalErrorMessageBox("FATAL ERROR: a or b too big, or MAXIT too small in betacf");
//		bvqxMainWnd()->page3log->append("// ERROR: a or b too big, or MAXIT too small in betacf");
	}

	return (float)h;
}

#undef MAXIT
#undef EPS
#undef FPMIN


void avevar(float data[], unsigned long n, float *ave, float *var)
{
	unsigned long j;
	float s, ep;

	for(*ave=0.0, j=1; j<=n; j++) *ave += data[j];
	*ave /= n;
	
	*var = ep = 0.0;
	for(j=1; j<=n; j++)
	{
		s = data[j]-(*ave);
		ep += s;
		*var += s*s;
	}
	*var=(*var-ep*ep/n)/(n-1);
}



// fit.c
void fit(float x[], float y[], int ndata, float sig[], int mwt, float *a,
	float *b, float *siga, float *sigb, float *chi2, float *q)
{
	float gammq(float a, float x);
	int i;
	float wt,t,sxoss,sx=0.0,sy=0.0,st2=0.0,ss,sigdat;
	*b=0.0;
	if (mwt) {
		ss=0.0;
		for (i=1;i<=ndata;i++) {
			wt=1.0/SQR(sig[i]);
			ss += wt;
			sx += x[i]*wt;
			sy += y[i]*wt;
		}
	} else {
		for (i=1;i<=ndata;i++) {
			sx += x[i];
			sy += y[i];
		}
		ss=ndata;
	}
	sxoss=sx/ss;
	if (mwt) {
		for (i=1;i<=ndata;i++) {
			t=(x[i]-sxoss)/sig[i];
			st2 += t*t;
			*b += t*y[i]/sig[i];
		}
	} else {
		for (i=1;i<=ndata;i++) {
			t=x[i]-sxoss;
			st2 += t*t;
			*b += t*y[i];
		}
	}
	*b /= st2;
	*a=(sy-sx*(*b))/ss;
	*siga=sqrt((1.0+sx*sx/(ss*st2))/ss);
	*sigb=sqrt(1.0/st2);
	*chi2=0.0;
	if (mwt == 0) {
		for (i=1;i<=ndata;i++)
			*chi2 += SQR(y[i]-(*a)-(*b)*x[i]);
		*q=1.0;
		sigdat=sqrt((*chi2)/(ndata-2));
		*siga *= sigdat;
		*sigb *= sigdat;
	} else {
		for (i=1;i<=ndata;i++)
			*chi2 += SQR((y[i]-(*a)-(*b)*x[i])/sig[i]);
		*q=gammq(0.5*(ndata-2),0.5*(*chi2));
	}
}


double gammln(double xx)
{
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;
	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}

float gammq(float a, float x)
{
	void gcf(float *gammcf, float a, float x, float *gln);
	void gser(float *gamser, float a, float x, float *gln);
	void nrerror(char error_text[]);
	float gamser,gammcf,gln;
	if (x < 0.0 || a <= 0.0) nrerror("Invalid arguments in routine gammq");
	if (x < (a+1.0)) {
		gser(&gamser,a,x,&gln);
		return 1.0-gamser;
	} else {
		gcf(&gammcf,a,x,&gln);
		return gammcf;
	}
}

float gasdev(long *idum)
{
	static int iset=0;
	static float gset;
	float fac,rsq,v1,v2;
	if  (iset == 0) {
		do {
			v1=2.0*ran1(idum)-1.0;
			v2=2.0*ran1(idum)-1.0;
			rsq=v1*v1+v2*v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac=sqrt(-2.0*log(rsq)/rsq);
		gset=v1*fac;
		iset=1;
		return v2*fac;
	} else {
		iset=0;
		return gset;
	}
}

#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30
void gcf(float *gammcf, float a, float x, float *gln)
{
	double gammln(double xx);
	void nrerror(char error_text[]);
	int i;
	float an,b,c,d,del,h;
	*gln=gammln(a);
	b=x+1.0-a;
	c=1.0/FPMIN;
	d=1.0/b;
	h=d;
	for (i=1;i<=ITMAX;i++) {
		an = -i*(i-a);
		b += 2.0;
		d=an*d+b;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=b+an/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0/d;
		del=d*c;
		h *= del;
		if (fabs(del-1.0) < EPS) break;
	}
	if (i > ITMAX) nrerror("a too large, ITMAX too small in gcf");
	*gammcf=exp(-x+a*log(x)-(*gln))*h;
}
#undef ITMAX
#undef EPS
#undef FPMIN

#define ITMAX 100
#define EPS 3.0e-7
void gser(float *gamser, float a, float x, float *gln)
{
	double gammln(double xx);
	void nrerror(char error_text[]);
	int n;
	float sum,del,ap;
	*gln=gammln(a);
	if (x <= 0.0) 
	{
		if (x < 0.0) nrerror("x less than 0 in routine gser");
		*gamser=0.0;
		return;
	} 
	else 
	{
		ap=a;
		del=sum=1.0/a;
		for (n=1;n<=ITMAX;n++) 
		{
			++ap;
			del *= x/ap;
			sum += del;
			if (fabs(del) < fabs(sum)*EPS) 
			{
				*gamser=sum*exp(-x+a*log(x)-(*gln));
				return;
			}
		}
		nrerror("a too large, ITMAX too small in routine gser");
		return;
	}
}
#undef ITMAX
#undef EPS

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran1(long *idum)
{
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	float temp;
	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	j=iy/NDIV;
	iy=iv[j];
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX





// Fourier-Transform Funktionen

void realft(float data[], unsigned long n, int isign)
{
	unsigned long i,i1,i2,i3,i4,np3;
	float c1=0.5,c2,h1r,h1i,h2r,h2i;
	double wr,wi,wpr,wpi,wtemp,theta;

	theta=3.141592653589793/(double) (n>>1);
	if (isign == 1) 
	{
		c2 = -0.5;
		four1(data,n>>1,1);
	} 
	else 
	{
		c2=0.5;
		theta = -theta;
	}
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	np3=n+3;
	for (i=2;i<=(n>>2);i++) 
	{
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
		h1r=c1*(data[i1]+data[i3]);
		h1i=c1*(data[i2]-data[i4]);
		h2r = -c2*(data[i2]+data[i4]);
		h2i=c2*(data[i1]-data[i3]);

		data[i1]=h1r+wr*h2r-wi*h2i;
		data[i2]=h1i+wr*h2i+wi*h2r;
		data[i3]=h1r-wr*h2r+wi*h2i;
		data[i4] = -h1i+wr*h2i+wi*h2r;
		wr=(wtemp=wr)*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
	}
	if (isign == 1) 
	{
		data[1] = (h1r=data[1])+data[2];
		data[2] = h1r-data[2];
	} 
	else 
	{
		data[1]=c1*((h1r=data[1])+data[2]);
		data[2]=c1*(h1r-data[2]);
		four1(data,n>>1,-1);
	}
}


#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
void four1(float data[], unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	float tempr,tempi;
	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) 
	{
		if (j > i) 
		{
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=n >> 1;
		while (m >= 2 && j > m) 
		{
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) 
	{
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) 
		{
			for (i=m;i<=n;i+=istep) 
			{
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}
#undef SWAP


void rlft3(float ***data, float **speq, unsigned long nn1, unsigned long nn2, unsigned long nn3, int isign)
{
	unsigned long i1,i2,i3,j1,j2,j3,nn[4],ii3;
	double theta,wi,wpi,wpr,wr,wtemp;
	float c1,c2,h1r,h1i,h2r,h2i;

	if (1+&data[nn1][nn2][nn3]-&data[1][1][1] != nn1*nn2*nn3)
		exit(1); // CriticalErrorMessageBox("rlft3: problem with dimensions or contiguity of data array");
	c1=0.5;
	c2 = -0.5*isign;
	theta=isign*(6.28318530717959/nn3);
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	nn[1]=nn1;
	nn[2]=nn2;
	nn[3]=nn3 >> 1;
	if (isign == 1) {
		fourn(&data[1][1][1]-1,nn,3,isign);
		for (i1=1;i1<=nn1;i1++)
			for (i2=1,j2=0;i2<=nn2;i2++) 
			{
				speq[i1][++j2]=data[i1][i2][1];
				speq[i1][++j2]=data[i1][i2][2];
			}
	}
	for (i1=1;i1<=nn1;i1++) 
	{
		j1=(i1 != 1 ? nn1-i1+2 : 1);
		wr=1.0;
		wi=0.0;
		for (ii3=1,i3=1;i3<=(nn3>>2)+1;i3++,ii3+=2) 
		{
			for (i2=1;i2<=nn2;i2++) 
			{
				if (i3 == 1) 
				{
					j2=(i2 != 1 ? ((nn2-i2)<<1)+3 : 1);
					h1r=c1*(data[i1][i2][1]+speq[j1][j2]);
					h1i=c1*(data[i1][i2][2]-speq[j1][j2+1]);
					h2i=c2*(data[i1][i2][1]-speq[j1][j2]);

					h2r= -c2*(data[i1][i2][2]+speq[j1][j2+1]);
					data[i1][i2][1]=h1r+h2r;
					data[i1][i2][2]=h1i+h2i;
					speq[j1][j2]=h1r-h2r;
					speq[j1][j2+1]=h2i-h1i;
				} 
				else 
				{
					j2=(i2 != 1 ? nn2-i2+2 : 1);
					j3=nn3+3-(i3<<1);
					h1r=c1*(data[i1][i2][ii3]+data[j1][j2][j3]);
					h1i=c1*(data[i1][i2][ii3+1]-data[j1][j2][j3+1]);
					h2i=c2*(data[i1][i2][ii3]-data[j1][j2][j3]);
					h2r= -c2*(data[i1][i2][ii3+1]+data[j1][j2][j3+1]);

					data[i1][i2][ii3]=h1r+wr*h2r-wi*h2i;
					data[i1][i2][ii3+1]=h1i+wr*h2i+wi*h2r;
					data[j1][j2][j3]=h1r-wr*h2r+wi*h2i;
					data[j1][j2][j3+1]= -h1i+wr*h2i+wi*h2r;
				}
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
	}
	if (isign == -1)
		fourn(&data[1][1][1]-1,nn,3,isign);
}


#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void fourn(float data[], unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	float tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) 
	{
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) 
		{
			if (i2 < i2rev) 
			{
				for (i1=i2;i1<=i2+ip1-2;i1+=2) 
				{
					for (i3=i1;i3<=ip3;i3+=ip2) 
					{
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) 
			{
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) 
		{
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) 
			{
				for (i1=i3;i1<=i3+ip1-2;i1+=2) 
				{
					for (i2=i1;i2<=ip3;i2+=ifp2) 
					{
						k1=i2;
						k2=k1+ifp1;
						tempr=(float)wr*data[k2]-(float)wi*data[k2+1];
						tempi=(float)wr*data[k2+1]+(float)wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}
#undef SWAP


#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;

float select(unsigned long k, unsigned long n, float arr[])
{
	unsigned long i,ir,j,l,mid;
	float a,temp;

	l=1;
	ir=n;
	for (;;) 
	{
		if (ir <= l+1) 
		{
			if (ir == l+1 && arr[ir] < arr[l]) 
			{
				SWAP(arr[l],arr[ir])
			}
			return arr[k];
		} 
		else 
		{
			mid=(l+ir) >> 1;
			SWAP(arr[mid],arr[l+1])
			if (arr[l] > arr[ir]) 
			{
				SWAP(arr[l],arr[ir])
			}
			if (arr[l+1] > arr[ir]) 
			{
				SWAP(arr[l+1],arr[ir])
			}
			if (arr[l] > arr[l+1]) 
			{
				SWAP(arr[l],arr[l+1])
			}
			i=l+1;
			j=ir;
			a=arr[l+1];
			for (;;) 
			{
				do i++; while (arr[i] < a);
				do j--; while (arr[j] > a);
				if (j < i) break;
				SWAP(arr[i],arr[j])
			}
			arr[l+1]=arr[j];
			arr[j]=a;
			if (j >= k) ir=j-1;
			if (j <= k) l=i;
		}
	}
}

unsigned short select_ushort(unsigned long k, unsigned long n, unsigned short arr[])
{
	unsigned long i,ir,j,l,mid;
	unsigned short a,temp;

	l=1;
	ir=n;
	for (;;) 
	{
		if (ir <= l+1) 
		{
			if (ir == l+1 && arr[ir] < arr[l]) 
			{
				SWAP(arr[l],arr[ir])
			}
			return arr[k];
		} 
		else 
		{
			mid=(l+ir) >> 1;
			SWAP(arr[mid],arr[l+1])
			if (arr[l] > arr[ir]) 
			{
				SWAP(arr[l],arr[ir])
			}
			if (arr[l+1] > arr[ir]) 
			{
				SWAP(arr[l+1],arr[ir])
			}
			if (arr[l] > arr[l+1]) 
			{
				SWAP(arr[l],arr[l+1])
			}
			i=l+1;
			j=ir;
			a=arr[l+1];
			for (;;) 
			{
				do i++; while (arr[i] < a);
				do j--; while (arr[j] > a);
				if (j < i) break;
				SWAP(arr[i],arr[j])
			}
			arr[l+1]=arr[j];
			arr[j]=a;
			if (j >= k) ir=j-1;
			if (j <= k) l=i;
		}
	}
}
#undef SWAP


#define EPS1 0.001
#define EPS2 1.0e-8
float probks(float alam)
{
	int j;
	float a2,fac=2.0,sum=0.0,term,termbf=0.0;
	a2 = -2.0*alam*alam;
	for (j=1;j<=100;j++) {
		term=fac*exp(a2*j*j);
		sum += term;
		if (fabs(term) <= EPS1*termbf || fabs(term) <= EPS2*sum) return sum;
		fac = -fac;
		termbf=fabs(term);
	}
	return 1.0;
}
#undef EPS1
#undef EPS2


#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;
#define M 7
#define NSTACK 60   // was 50, but might be too small
void sort(unsigned long n, float arr[])
{
	unsigned long i,ir=n,j,k,l=1;
	int jstack=0,*istack;
	float a,temp;
	istack=ivector(1,NSTACK);
	for (;;) {
		if (ir-l < M) {
			for (j=l+1;j<=ir;j++) {
				a=arr[j];
				for (i=j-1;i>=l;i--) {
					if (arr[i] <= a) break;
					arr[i+1]=arr[i];
				}
				arr[i+1]=a;
			}
			if (jstack == 0) break;
			ir=istack[jstack--];
			l=istack[jstack--];
		} else {
			k=(l+ir) >> 1;
			SWAP(arr[k],arr[l+1])
			if (arr[l] > arr[ir]) {
				SWAP(arr[l],arr[ir])
			}
			if (arr[l+1] > arr[ir]) {
				SWAP(arr[l+1],arr[ir])
			}
			if (arr[l] > arr[l+1]) {
				SWAP(arr[l],arr[l+1])
			}
			i=l+1;
			j=ir;
			a=arr[l+1];
			for (;;) {
				do i++; while (arr[i] < a);
				do j--; while (arr[j] > a);
				if (j < i) break;
				SWAP(arr[i],arr[j]);
			}
			arr[l+1]=arr[j];
			arr[j]=a;
			jstack += 2;
			if (jstack > NSTACK) nrerror("NSTACK too small in sort.");
			if (ir-i+1 >= j-l) {
				istack[jstack]=ir;
				istack[jstack-1]=i;
				ir=j-1;
			} else {
				istack[jstack]=j-1;
				istack[jstack-1]=l;
				l=i;
			}
		}
	}
	free_ivector(istack,1,NSTACK);
}
#undef M
#undef NSTACK
#undef SWAP

#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;
#define M 7
#define NSTACK 50
void sort2(unsigned long n, float arr[], float brr[])
{
	unsigned long i,ir=n,j,k,l=1;
	int *istack,jstack=0;
	float a,b,temp;
	istack=ivector(1,NSTACK);
	for (;;) {
		if (ir-l < M) {
			for (j=l+1;j<=ir;j++) {
				a=arr[j];
				b=brr[j];
				for (i=j-1;i>=l;i--) {
					if (arr[i] <= a) break;
					arr[i+1]=arr[i];
					brr[i+1]=brr[i];
				}
				arr[i+1]=a;
				brr[i+1]=b;
			}
			if (!jstack) {
				free_ivector(istack,1,NSTACK);
				return;
			}
			ir=istack[jstack];
			l=istack[jstack-1];
			jstack -= 2;
		} else {
			k=(l+ir) >> 1;
			SWAP(arr[k],arr[l+1])
			SWAP(brr[k],brr[l+1])
			if (arr[l] > arr[ir]) {
				SWAP(arr[l],arr[ir])
				SWAP(brr[l],brr[ir])
			}
			if (arr[l+1] > arr[ir]) {
				SWAP(arr[l+1],arr[ir])
				SWAP(brr[l+1],brr[ir])
			}
			if (arr[l] > arr[l+1]) {
				SWAP(arr[l],arr[l+1])
				SWAP(brr[l],brr[l+1])
			}
			i=l+1;
			j=ir;
			a=arr[l+1];
			b=brr[l+1];
			for (;;) {
				do i++; while (arr[i] < a);
				do j--; while (arr[j] > a);
				if (j < i) break;
				SWAP(arr[i],arr[j])
				SWAP(brr[i],brr[j])
			}
			arr[l+1]=arr[j];
			arr[j]=a;
			brr[l+1]=brr[j];
			brr[j]=b;
			jstack += 2;
			if (jstack > NSTACK) nrerror("NSTACK too small in sort2.");
			if (ir-i+1 >= j-l) {
				istack[jstack]=ir;
				istack[jstack-1]=i;
				ir=j-1;
			} else {
				istack[jstack]=j-1;
				istack[jstack-1]=l;
				l=i;
			}
		}
	}
}
#undef M
#undef NSTACK
#undef SWAP
#undef NRANSI


void crank(unsigned long n, float w[], float *s)
{
	unsigned long j=1,ji,jt;
	float t,rank;
	*s=0.0;
	while (j < n) {
		if (w[j+1] != w[j]) {
			w[j]=j;
			++j;
		} else {
			for (jt=j+1;jt<=n && w[jt]==w[j];jt++);
			rank=0.5*(j+jt-1);
			for (ji=j;ji<=(jt-1);ji++) w[ji]=rank;
			t=jt-j;
			*s += t*t*t-t;
			j=jt;
		}
	}
	if (j == n) w[n]=n;
}

float erfcc(float x)
{
	float t,z,ans;
	z=fabs(x);
	t=1.0/(1.0+0.5*z);
	ans=t*exp(-z*z-1.26551223+t*(1.00002368+t*(0.37409196+t*(0.09678418+
		t*(-0.18628806+t*(0.27886807+t*(-1.13520398+t*(1.48851587+
		t*(-0.82215223+t*0.17087277)))))))));
	return x >= 0.0 ? ans : 2.0-ans;
}


// von gasdev abgeleitet, zur Umrechnung von p-Wert in z-Wert
float p_to_z(float pvalue)
{
	float fac,rsq,v1;

	if(pvalue == 1.0f)
		pvalue = 0.9999999f;

	v1 = 1.0-pvalue;
	rsq=v1*v1;

	fac=sqrt(-2.0*log(rsq)/rsq);
	return v1*fac;
}



// Functions for least-square fit using singular value decomposition
#define TOL 1.0e-5
void svdfit(float x[], float y[], float sig[], int ndata, float a[], int ma,
	float **u, float **v, float w[], float *chisq,
	void (*funcs)(float, float [], int))
{
	int j,i;
	float wmax,tmp,thresh,sum,*b,*afunc;
	b=vector(1,ndata);
	afunc=vector(1,ma);
	for (i=1;i<=ndata;i++) 
	{
		(*funcs)(x[i],afunc,ma);
		tmp=1.0/sig[i];

		for (j=1;j<=ma;j++) u[i][j]=afunc[j]*tmp;

		b[i]=y[i]*tmp;
	}
	
	svdcmp(u,ndata,ma,w,v);
	
	wmax=0.0;
	for (j=1;j<=ma;j++)
		if (w[j] > wmax) wmax=w[j];
	thresh=TOL*wmax;
	
	for (j=1;j<=ma;j++)
		if (w[j] < thresh) w[j]=0.0;
	
	svbksb(u,w,v,ndata,ma,b,a);
	
	*chisq=0.0;
	for (i=1;i<=ndata;i++) 
	{
		(*funcs)(x[i],afunc,ma);
		for (sum=0.0,j=1;j<=ma;j++) sum += a[j]*afunc[j];
		*chisq += (tmp=(y[i]-sum)/sig[i],tmp*tmp);
	}
	free_vector(afunc,1,ma);
	free_vector(b,1,ndata);
}
#undef TOL

void svdvar(float **v, int ma, float w[], float **cvm)
{
	int k,j,i;
	float sum,*wti;
	wti=vector(1,ma);
	for (i=1;i<=ma;i++) 
	{
		wti[i]=0.0;
		if (w[i]) wti[i]=1.0/(w[i]*w[i]);
	}
	for (i=1;i<=ma;i++) 
	{
		for (j=1;j<=i;j++) 
		{
			for (sum=0.0,k=1;k<=ma;k++) sum += v[i][k]*v[j][k]*wti[k];
			cvm[j][i]=cvm[i][j]=sum;
		}
	}
	free_vector(wti,1,ma);
}

void svdcmp(float **a, int m, int n, float w[], float **v)
{
	NoConvergenceInSVD = false;

	int flag,i,its,j,jj,k,l,nm;
	float anorm,c,f,g,h,s,scale,x,y,z,*rv1;
	rv1=vector(1,n);
	g=scale=anorm=0.0;
	
	for (i=1;i<=n;i++) 
	{
		l=i+1;
		rv1[i]=scale*g;
		g=s=scale=0.0;
		
		if (i <= m) 
		{
			for (k=i;k<=m;k++) scale += QX_ABS(a[k][i]);
	
			if (scale) 
			{
				for (k=i;k<=m;k++) 
				{
					a[k][i] /= scale;
					s += a[k][i]*a[k][i];
				}
				f=a[i][i];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][i]=f-g;
				
				for (j=l;j<=n;j++) 
				{
					for (s=0.0,k=i;k<=m;k++) s += a[k][i]*a[k][j];
					f=s/h;
					for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
				}
				
				for (k=i;k<=m;k++) a[k][i] *= scale;
			}
		}
		w[i]=scale *g;
		g=s=scale=0.0;
		if (i <= m && i != n) 
		{
			for (k=l;k<=n;k++) scale += QX_ABS(a[i][k]);
			if (scale) 
			{
				for (k=l;k<=n;k++) 
				{
					a[i][k] /= scale;
					s += a[i][k]*a[i][k];
				}
				
				f=a[i][l];
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][l]=f-g;
				
				for (k=l;k<=n;k++) rv1[k]=a[i][k]/h;
				
				for (j=l;j<=m;j++) 
				{
					for (s=0.0,k=l;k<=n;k++) s += a[j][k]*a[i][k];
					for (k=l;k<=n;k++) a[j][k] += s*rv1[k];
				}
				
				for (k=l;k<=n;k++) a[i][k] *= scale;
			}
		}
		anorm=FMAX(anorm,(QX_ABS(w[i])+QX_ABS(rv1[i])));
	}
	for (i=n;i>=1;i--) 
	{
		if (i < n) 
		{
			if (g) 
			{
				for (j=l;j<=n;j++)
					v[j][i]=(a[i][j]/a[i][l])/g;
			
				for (j=l;j<=n;j++) 
				{
					for (s=0.0,k=l;k<=n;k++) s += a[i][k]*v[k][j];
				
					for (k=l;k<=n;k++) v[k][j] += s*v[k][i];
				}
			}
			for (j=l;j<=n;j++) v[i][j]=v[j][i]=0.0;
		}
		v[i][i]=1.0;
		g=rv1[i];
		l=i;
	}

	for (i=IMIN(m,n);i>=1;i--) 
	{
		l=i+1;
		g=w[i];
	
		for (j=l;j<=n;j++) a[i][j]=0.0;
		
		if (g) 
		{
			g=1.0/g;
			
			for (j=l;j<=n;j++) 
			{
				for (s=0.0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
				
				f=(s/a[i][i])*g;
				
				for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
			}
			for (j=i;j<=m;j++) a[j][i] *= g;
		} 
		else 
			for (j=i;j<=m;j++) a[j][i]=0.0;
		
		++a[i][i];
	}

//#define debug_file
#ifdef debug_file
	FILE *fp;
	if((fp = fopen("svdcmp.txt", "wt")) != NULL)
	{
		fprintf(fp, "%20.18f\n", anorm);
	}
#endif	
	
	for (k=n;k>=1;k--) 
	{
		for (its=1;its<=100;its++) 	 // increased form "30" to "100" iterations
		{
			flag=1;
			for (l=k;l>=1;l--) 
			{
				nm=l-1;
#ifdef debug_file
				fprintf(fp, "rv1[%i]=%20.18f w[%i]=%20.18f\n", l, rv1[l], nm, w[nm]);
#endif
				// Problem with stop criterion: "float" values do not become zero, this prevents then below setting w-values to absolute values, which again produced the error message "zero w's" in "getInverseMatrix"
				//
				//if ((float)(QX_ABS(rv1[l])+anorm) == anorm) 
				if( QX_ABS( rv1[l] ) < 0.000001f )
				{
					flag=0;
					break;
				}
				//if ((float)(QX_ABS(w[nm])+anorm) == anorm) 
				if( QX_ABS(w[nm]) < 0.000001f )
					break;
			}

			if (flag) 
			{
				c=0.0;
				s=1.0;
			
				for (i=l;i<=k;i++) 
				{
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
				
					// if ((float)(QX_ABS(f)+anorm) == anorm)
					if( QX_ABS(f) < 0.000001f )
						break;
					
					g=w[i];
					h=pythag(f,g);
					w[i]=h;
					h=1.0/h;
					c=g*h;
					s = -f*h;
					
					for (j=1;j<=m;j++) 
					{
						y=a[j][nm];
						z=a[j][i];
						a[j][nm]=y*c+z*s;
						a[j][i]=z*c-y*s;
					}
				}
			}
			
			z=w[k];

			if (l == k) 
			{
				if (z < 0.0) 
				{
					w[k] = -z;
					for (j=1;j<=n;j++) v[j][k] = -v[j][k];
				}
				break;
			}

			// do no longer quit if no convergence, continue but signal problem to user
			if (its == 100) // nrerror("no convergence in 100 svdcmp iterations");
			{
				NoConvergenceInSVD = true;
			}

			x=w[l];
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0f*h*y);
			g=pythag(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			c=s=1.0;
			
			for (j=l;j<=nm;j++) 
			{
				i=j+1;
				g=rv1[i];
				y=w[i];
				h=s*g;
				g=c*g;
				z=pythag(f,h);
#ifdef debug_file
				fprintf(fp, "z=%20.18f\n", z);
#endif
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y *= c;
			
				for (jj=1;jj<=n;jj++) 
				{
					x=v[jj][j];
					z=v[jj][i];
					v[jj][j]=x*c+z*s;
					v[jj][i]=z*c-x*s;
				}
				
				z=pythag(f,h);
				w[j]=z;
				
				if (z) 
				{
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
				
				f=c*g+s*y;
				x=c*y-s*g;
				
				for (jj=1;jj<=m;jj++) 
				{
					y=a[jj][j];
					z=a[jj][i];
					a[jj][j]=y*c+z*s;
					a[jj][i]=z*c-y*s;
				}
			}
			rv1[l]=0.0f;
			rv1[k]=f;
			w[k]=x;
		}
	}
	free_vector(rv1,1,n);

#ifdef debug_file
	fclose(fp);	
#endif
}

void svbksb(float **u, float w[], float **v, int m, int n, float b[], float x[])
{
	int jj,j,i;
	float s,*tmp;
	tmp=vector(1,n);
	for (j=1;j<=n;j++) {
		s=0.0;
		if (w[j]) {
			for (i=1;i<=m;i++) s += u[i][j]*b[i];
			s /= w[j];
		}
		tmp[j]=s;
	}
	for (j=1;j<=n;j++) {
		s=0.0;
		for (jj=1;jj<=n;jj++) s += v[j][jj]*tmp[jj];
		x[j]=s;
	}
	free_vector(tmp,1,n);
}



float pythag(float a, float b)
{
	float absa,absb;
	
	absa=QX_ABS(a);
	absb=QX_ABS(b);
	
	if (absa > absb) 
		return absa*(float)sqrt(1.0f+SQR(absb/absa));
	else 
		return (absb == 0.0f ? 0.0f : absb*(float)sqrt(1.0f+SQR(absa/absb)));
}

void fpoly(float x, float p[], int np)
{
	int j;

	p[1] = 1.0;

	for(j=2; j<=np; j++)
		p[j] = p[j-1]*x;
}

void fgauss(float x, float a[], float *y, float dyda[], int na)
{
	int i;
	float fac,ex,arg;
	*y=0.0;
	for (i=1;i<=na-1;i+=3) 
	{
		arg=(x-a[i+1])/a[i+2];
		ex=exp(-arg*arg);
		fac=a[i]*ex*2.0*arg;
		*y += a[i]*ex;
		dyda[i]=ex;
		dyda[i+1]=fac/a[i+2];
		dyda[i+2]=fac*arg/a[i+2];
	}
}


// LevMarq
void mrqmin(float x[], float y[], float sig[], int ndata, float a[], int ia[], int ma, float **covar, float **alpha, float *chisq, void (*funcs)(float, float [], float *, float [], int), float *alamda)
{
	int j,k,l;
	static int mfit;
	static float ochisq,*atry,*beta,*da,**oneda;
	if (*alamda < 0.0) {
		atry=vector(1,ma);
		beta=vector(1,ma);
		da=vector(1,ma);
		for (mfit=0,j=1;j<=ma;j++)
			if (ia[j]) mfit++;
		oneda=matrix(1,mfit,1,1);
		*alamda=0.001;
		mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq,funcs);
		ochisq=(*chisq);
		for (j=1;j<=ma;j++) atry[j]=a[j];
	}
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=mfit;k++) covar[j][k]=alpha[j][k];
		covar[j][j]=alpha[j][j]*(1.0+(*alamda));
		oneda[j][1]=beta[j];
	}
	gaussj(covar,mfit,oneda,1);
	for (j=1;j<=mfit;j++) da[j]=oneda[j][1];
	if (*alamda == 0.0) {
		covsrt(covar,ma,ia,mfit);
		free_matrix(oneda,1,mfit,1,1);
		free_vector(da,1,ma);
		free_vector(beta,1,ma);
		free_vector(atry,1,ma);
		return;
	}
	for (j=0,l=1;l<=ma;l++)
		if (ia[l]) atry[l]=a[l]+da[++j];
	mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq,funcs);
	if (*chisq < ochisq) {
		*alamda *= 0.1;
		ochisq=(*chisq);
		for (j=1;j<=mfit;j++) {
			for (k=1;k<=mfit;k++) alpha[j][k]=covar[j][k];
			beta[j]=da[j];
		}
		for (l=1;l<=ma;l++) a[l]=atry[l];
	} else {
		*alamda *= 10.0;
		*chisq=ochisq;
	}
}

void mrqcof(float x[], float y[], float sig[], int ndata, float a[], int ia[], int ma, float **alpha, float beta[], float *chisq, void (*funcs)(float, float [], float *, float [], int))
{
	int i,j,k,l,m,mfit=0;
	float ymod,wt,sig2i,dy,*dyda;
	dyda=vector(1,ma);
	for (j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=j;k++) alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	for (i=1;i<=ndata;i++) {
		(*funcs)(x[i],a,&ymod,dyda,ma);
		sig2i=1.0/(sig[i]*sig[i]);
		dy=y[i]-ymod;
		for (j=0,l=1;l<=ma;l++) {
			if (ia[l]) {
				wt=dyda[l]*sig2i;
				for (j++,k=0,m=1;m<=l;m++)
					if (ia[m]) alpha[j][++k] += wt*dyda[m];
				beta[j] += dy*wt;
			}
		}
		*chisq += dy*dy*sig2i;
	}
	for (j=2;j<=mfit;j++)
		for (k=1;k<j;k++) alpha[k][j]=alpha[j][k];
	free_vector(dyda,1,ma);
}



// LevMarq
// geändert, sig rausgenommen, da immer 1.0 und funcs-Aufruf durch pointer auf pDoc ersetzt
/*void mrqmin(Q_INT16 *x, Q_INT16 *y, Q_INT16 *ymod, int ndata, float a[], int ia[],
	int ma, float **covar, float **alpha, float *chisq,
	float *alamda, QtBrainVoyagerDoc *pDoc)
{
	int j,k,l;
	//static int mfit;
	//static float ochisq,*atry,*beta,*da,**oneda;
	if (*alamda < 0.0) 
	{
		atry=vector(1,ma);
		beta=vector(1,ma);
		da=vector(1,ma);
		for (mfit=0,j=1;j<=ma;j++)
			if (ia[j]) mfit++;
		oneda=matrix(1,mfit,1,1);
		*alamda=0.001;
		mrqcof(x,y,ymod,ndata,a,ia,ma,alpha,beta,chisq,pDoc);
		ochisq=(*chisq);
		for (j=1;j<=ma;j++) atry[j]=a[j];
	}
	for (j=1;j<=mfit;j++) 
	{
		for (k=1;k<=mfit;k++) covar[j][k]=alpha[j][k];
		covar[j][j]=alpha[j][j]*(1.0+(*alamda));
		oneda[j][1]=beta[j];
	}
	gaussj(covar,mfit,oneda,1);
	for (j=1;j<=mfit;j++) da[j]=oneda[j][1];
	if (*alamda == 0.0) {
		covsrt(covar,ma,ia,mfit);
		free_matrix(oneda,1,mfit,1,1);
		free_vector(da,1,ma);
		free_vector(beta,1,ma);
		free_vector(atry,1,ma);
		return;
	}
	for (j=0,l=1;l<=ma;l++)
		if (ia[l]) atry[l]=a[l]+da[++j];
	mrqcof(x,y,ymod,ndata,atry,ia,ma,covar,da,chisq,pDoc);
	if (*chisq < ochisq) 
	{
		*alamda *= 0.1;
        *alamda = max(0.000000000000001f, *alamda);
		ochisq=(*chisq);
		for (j=1;j<=mfit;j++) 
		{
			for (k=1;k<=mfit;k++) alpha[j][k]=covar[j][k];
			beta[j]=da[j];
		}
		for (l=1;l<=ma;l++) a[l]=atry[l];
	} 
	else 
	{
		*alamda *= 10.0;
        *alamda = min(10000000000000.0f, *alamda);
		*chisq=ochisq;
	}
} */

/*void mrqcof(Q_INT16 *x, Q_INT16 *y, Q_INT16 *ymod, int ndata, float a[], int ia[],
	int ma, float **alpha, float beta[], float *chisq,
	QtBrainVoyagerDoc *pDoc)
{
	int i,j,k,l,m,mfit=0;
	float wt,sig2i,dy,*dyda;
	dyda=vector(1,ma);
	for (j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=j;k++) alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	sig2i = 1.0;
// TODO:	pDoc->LevMrqResliceVTCVolAndGradients(y+1, x+1, ymod+1, a[1], a[2], a[3], a[4], a[5], a[6]);
	pDoc->MC3DNrOfChiSquareValues = 0;
	int intensity_thresh = pDoc->MC3DIntensityThresh;

	int xyz;
	int xx, yy, zz, xb_v = 2, xb_h = 2, yb_o = 2, yb_u = 2, zb_l = 2, zb_r = 2;
	int x_sample_size = pDoc->MC3DSampleSize, y_sample_size = pDoc->MC3DSampleSize, z_sample_size = pDoc->MC3DSampleSize;
	int n_pixels_x = (pDoc->XEndVTC - pDoc->XStartVTC)/pDoc->ResolutionVTC;
	int n_pixels_y = (pDoc->YEndVTC - pDoc->YStartVTC)/pDoc->ResolutionVTC;
	int n_pixels_z = (pDoc->ZEndVTC - pDoc->ZStartVTC)/pDoc->ResolutionVTC;
	int n_pixels_xy = n_pixels_x * n_pixels_y;
	int n_pixels_xyz = n_pixels_xy * n_pixels_z;

	for(zz=zb_l; zz<(n_pixels_z-zb_r); zz+=z_sample_size)
	{
		for(yy=yb_o; yy<(n_pixels_y-yb_u); yy+=y_sample_size)
		{
			for(xx=xb_v; xx<(n_pixels_x-xb_h); xx+=x_sample_size)
			{
				xyz = zz*n_pixels_xy + yy*n_pixels_x + xx;

                if((y[xyz+1] > intensity_thresh) && (ymod[xyz+1] > intensity_thresh))
		        {
			        dy=y[xyz+1]-ymod[xyz+1];
	        
			        pDoc->MC3DNrOfChiSquareValues++;
			        
			        dyda[1] = -pDoc->MC3DGradTranslVolX[xyz];
			        dyda[2] = -pDoc->MC3DGradTranslVolY[xyz];
			        dyda[3] = -pDoc->MC3DGradTranslVolZ[xyz];
			        dyda[4] = -pDoc->MC3DGradRotVolX[xyz];
			        dyda[5] = -pDoc->MC3DGradRotVolY[xyz];
			        dyda[6] = -pDoc->MC3DGradRotVolZ[xyz];

			        for (j=0,l=1;l<=ma;l++) 
			        {
				        if (ia[l]) 
				        {
					        wt=dyda[l]*sig2i;
					        for (j++,k=0,m=1;m<=l;m++)
						        if (ia[m]) alpha[j][++k] += wt*dyda[m];
					        beta[j] += dy*wt;
				        }
			        }
			        *chisq += dy*dy*sig2i;
                }
            }
		}
	}
	for (j=2;j<=mfit;j++)
		for (k=1;k<j;k++) alpha[k][j]=alpha[j][k];
	free_vector(dyda,1,ma);
} */

// LevMarq
// modified version for FMR-STC 3D motion correction
/* void mrqmin_stc(Q_INT16 *x, Q_INT16 *y, Q_INT16 *ymod, int ndata, float a[], int ia[],
	int ma, float **covar, float **alpha, float *chisq,
	float *alamda, QtBrainVoyagerDoc *pDoc)
{
	int j,k,l;
	//static int mfit;
	//static float ochisq,*atry,*beta,*da,**oneda;
	if (*alamda < 0.0) 
	{
		atry=vector(1,ma);
		beta=vector(1,ma);
		da=vector(1,ma);
		for (mfit=0,j=1;j<=ma;j++)
			if (ia[j]) mfit++;
		oneda=matrix(1,mfit,1,1);
		*alamda=0.001;
		mrqcof_stc(x,y,ymod,ndata,a,ia,ma,alpha,beta,chisq,pDoc);
		ochisq=(*chisq);
		for (j=1;j<=ma;j++) atry[j]=a[j];
	}

	// added security check:
	bool AllZeros = TRUE;
	for (j=1;j<=mfit;j++) 
	{
		for (k=1;k<=mfit;k++)
		{
			covar[j][k]=alpha[j][k];
			if(covar[j][k] != 0.0f)  // security checking
				AllZeros = FALSE;
		}
		covar[j][j]=alpha[j][j]*(1.0+(*alamda));
		oneda[j][1]=beta[j];
	}
	if(AllZeros) // gaussj will fail!
	{
		g_Terminate = TRUE;
		WarningMessageBox("FATAL ERROR: Zero covariance matrix in fitting routine.\n\nPlease contact Brain Innovation: support@BrainVoyager.com");
		return;
	}
	else
		gaussj(covar,mfit,oneda,1);
	
	for (j=1;j<=mfit;j++) da[j]=oneda[j][1];
	if (*alamda == 0.0) {
		covsrt(covar,ma,ia,mfit);
		free_matrix(oneda,1,mfit,1,1);
		free_vector(da,1,ma);
		free_vector(beta,1,ma);
		free_vector(atry,1,ma);
		return;
	}
	for (j=0,l=1;l<=ma;l++)
		if (ia[l]) atry[l]=a[l]+da[++j];
	mrqcof_stc(x,y,ymod,ndata,atry,ia,ma,covar,da,chisq,pDoc);
	if (*chisq < ochisq) 
	{
		*alamda *= 0.1;
        *alamda = max(0.00000000000000000001f, *alamda);
		ochisq=(*chisq);
		for (j=1;j<=mfit;j++) 
		{
			for (k=1;k<=mfit;k++) alpha[j][k]=covar[j][k];
			beta[j]=da[j];
		}
		for (l=1;l<=ma;l++) a[l]=atry[l];
	} 
	else 
	{
		*alamda *= 10.0;
        *alamda = min(1000000000000000000.0f, *alamda);
		*chisq=ochisq;
	}
} */

/* void mrqcof_stc(Q_INT16 *x, Q_INT16 *y, Q_INT16 *ymod, int ndata, float a[], int ia[],
	int ma, float **alpha, float beta[], float *chisq,
	QtBrainVoyagerDoc *pDoc)
{
	int i,j,k,l,m,mfit=0;
	float wt,sig2i,dy,*dyda;
	dyda=vector(1,ma);
	for (j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=j;k++) alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	sig2i = 1.0;
	pDoc->LevMrqResliceSTCVolAndGradients(y, x, ymod, a[1], a[2], a[3], a[4], a[5], a[6]);
	pDoc->MC3DNrOfChiSquareValues = 0;
	int intensity_thresh = pDoc->MC3DIntensityThresh;

	int xyz;
	int xx, yy, zz, xb_v = 2, xb_h = 2, yb_o = 2, yb_u = 2, zb_l = 1, zb_r = 1;
	int x_sample_size = pDoc->MC3DSampleSizeX, y_sample_size = pDoc->MC3DSampleSizeY, z_sample_size = pDoc->MC3DSampleSizeZ;

	int n_slices = pDoc->m_NSlicesFMR;
	int NPixels = pDoc->NZeilen * pDoc->NSpalten;
	int NVoxels = n_slices * NPixels;
	
//	int n_pixels_x = (pDoc->XEndVTC - pDoc->XStartVTC)/pDoc->ResolutionVTC;
//	int n_pixels_y = (pDoc->YEndVTC - pDoc->YStartVTC)/pDoc->ResolutionVTC;
//	int n_pixels_z = (pDoc->ZEndVTC - pDoc->ZStartVTC)/pDoc->ResolutionVTC;
//	int n_pixels_xy = n_pixels_x * n_pixels_y;
//	int n_pixels_xyz = n_pixels_xy * n_pixels_z;

	for(zz=zb_l; zz<(n_slices-zb_r); zz+=z_sample_size)
	{
		for(yy=yb_o; yy<(pDoc->NZeilen-yb_u); yy+=y_sample_size)
		{
			for(xx=xb_v; xx<(pDoc->NSpalten-xb_h); xx+=x_sample_size)
			{
				xyz = zz*NPixels + yy*pDoc->NSpalten + xx;

                if((y[xyz] > intensity_thresh) && (ymod[xyz] > intensity_thresh))
		        {
			        dy=y[xyz]-ymod[xyz];
	        
			        pDoc->MC3DNrOfChiSquareValues++;
			        
			        dyda[1] = -pDoc->MC3DGradTranslVolX[xyz];
			        dyda[2] = -pDoc->MC3DGradTranslVolY[xyz];
			        dyda[3] = -pDoc->MC3DGradTranslVolZ[xyz];
			        dyda[4] = -pDoc->MC3DGradRotVolX[xyz];
			        dyda[5] = -pDoc->MC3DGradRotVolY[xyz];
			        dyda[6] = -pDoc->MC3DGradRotVolZ[xyz];

			        for (j=0,l=1;l<=ma;l++) 
			        {
				        if (ia[l]) 
				        {
					        wt=dyda[l]*sig2i;
					        for (j++,k=0,m=1;m<=l;m++)
						        if (ia[m]) alpha[j][++k] += wt*dyda[m];
					        beta[j] += dy*wt;
				        }
			        }
			        *chisq += dy*dy*sig2i;
                }
            }
		}
	}
	for (j=2;j<=mfit;j++)
		for (k=1;k<j;k++) alpha[k][j]=alpha[j][k];
	free_vector(dyda,1,ma);
} */


#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}
void gaussj(float **a, int n, float **b, int m)
{
	int *indxc,*indxr,*ipiv;
	int i,icol,irow,j,k,l,ll;
	float big,dum,pivinv,temp;
	indxc=ivector(1,n);
	indxr=ivector(1,n);
	ipiv=ivector(1,n);
	for (j=1;j<=n;j++) ipiv[j]=0;
	for (i=1;i<=n;i++) {
		big=0.0;
		for (j=1;j<=n;j++)
			if (ipiv[j] != 1)
				for (k=1;k<=n;k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					} else if (ipiv[k] > 1) nrerror("gaussj: Singular Matrix-1");
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=1;l<=n;l++) SWAP(a[irow][l],a[icol][l])
			for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
		}
		indxr[i]=irow;
		indxc[i]=icol;
		if (a[icol][icol] == 0.0) nrerror("gaussj: Singular Matrix-2");
		pivinv=1.0/a[icol][icol];
		a[icol][icol]=1.0;
		for (l=1;l<=n;l++) a[icol][l] *= pivinv;
		for (l=1;l<=m;l++) b[icol][l] *= pivinv;
		for (ll=1;ll<=n;ll++)
			if (ll != icol) {
				dum=a[ll][icol];
				a[ll][icol]=0.0;
				for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
				for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l=n;l>=1;l--) {
		if (indxr[l] != indxc[l])
			for (k=1;k<=n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}
	free_ivector(ipiv,1,n);
	free_ivector(indxr,1,n);
	free_ivector(indxc,1,n);
}
#undef SWAP

#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}
void covsrt(float **covar, int ma, int ia[], int mfit)
{
	int i,j,k;
	float swap;
	for (i=mfit+1;i<=ma;i++)
		for (j=1;j<=i;j++) covar[i][j]=covar[j][i]=0.0;
	k=mfit;
	for (j=ma;j>=1;j--) {
		if (ia[j]) {
			for (i=1;i<=ma;i++) SWAP(covar[i][k],covar[i][j])
			for (i=1;i<=ma;i++) SWAP(covar[k][i],covar[j][i])
			k--;
		}
	}
}
#undef SWAP


/********************************/
/*  heapsort mit QValueList :   */
/*   Delimiter zum segmentieren	*/
/*   benutzen, damit sort nach  */
/*   Zahlen funktioniert        */
/********************************/
/*void hpsort_delimiter_strings(unsigned long n, QChar delimiter, QStringList &ListOfFilesStr)
{
	unsigned long i, ir, j, l;
	QString rra;

	if(n<2)
		return;

	l = (n >> 1) + 1;
	ir = n;

	for(;;)
	{
		if(l > 1)
		{
			rra = ListOfFilesStr[--l - 1];
		}
		else
		{
			rra = ListOfFilesStr[ir-1];
			ListOfFilesStr[ir-1] = ListOfFilesStr[0];
			if(--ir == 1)
			{
				ListOfFilesStr[0] = rra;
				break;
			}
		}
		i = l;
		j = l+l;
		while(j <= ir)
		{
			if(j < ir && compare_delim(ListOfFilesStr[j-1], ListOfFilesStr[j], delimiter))
				j++;
			if(compare_delim(rra, ListOfFilesStr[j-1], delimiter))
			{
				ListOfFilesStr[i-1] = ListOfFilesStr[j-1];
				i=j;
				j <<= 1;
			}
			else
				j = ir+1;
		}
		ListOfFilesStr[i-1] = rra;
	}
} */

/*bool compare_delim(QString String1, QString String2, QChar delimiter)
{
	QString NewString1, NewString2;
	QString TempStr1_1, TempStr1_2;
	QString TempStr2_1, TempStr2_2;
	int index1, index2, fill_zero_digits, f;
	QString DelStr;
	DelStr = delimiter;

	index1 = String1.findRev(delimiter);
	index2 = String2.findRev(delimiter);
	if((index1 > -1) && (index2 > -1))
	{
		TempStr1_1 = String1.right(String1.length() - index1 - 1);
		TempStr1_2 = String2.right(String2.length() - index2 - 1);

		fill_zero_digits = TempStr1_2.length() - TempStr1_1.length();
		if(fill_zero_digits > 0)
		{
			for(f=0; f<fill_zero_digits; f++)
			{
				TempStr1_1 = "0" + TempStr1_1;
			}
		}
		else if(fill_zero_digits < 0)
		{
			for(f=0; f < abs(fill_zero_digits); f++)
			{
				TempStr1_2 = "0" + TempStr1_2;
			}
		}
		
		String1 = String1.left(index1);
		String2 = String2.left(index2);


		index1 = String1.findRev(delimiter);
		index2 = String2.findRev(delimiter);
		if((index1 > -1) && (index2 > -1))
		{
			TempStr2_1 = String1.right(String1.length() - index1 - 1);
			TempStr2_2 = String2.right(String2.length() - index2 - 1);

			fill_zero_digits = TempStr2_2.length() - TempStr2_1.length();
			if(fill_zero_digits > 0)
			{
				for(f=0; f<fill_zero_digits; f++)
				{
					TempStr2_1 = "0" + TempStr2_1;
				}
			}
			else if(fill_zero_digits < 0)
			{
				for(f=0; f < abs(fill_zero_digits); f++)
				{
					TempStr2_2 = "0" + TempStr2_2;
				}
			}
			
			String1 = String1.left(index1);
			String2 = String2.left(index2);

			NewString1 = String1 + DelStr + TempStr2_1 + DelStr + TempStr1_1;
			NewString2 = String2 + DelStr + TempStr2_2 + DelStr + TempStr1_2;
			return (NewString1 < NewString2);
		}
		else
		{
			NewString1 = String1 + DelStr + TempStr1_1;
			NewString2 = String2 + DelStr + TempStr1_2;
			return (NewString1 < NewString2);
		}
	}
	else  // kein Delimiter, daher Standard-string Vergleich
		return (String1 < String2);
} */

/*******************************/
/*  heapsort mit CStringArray  */
/*******************************/
/*void hpsort_strings(unsigned long n, CStringArray *ListOfFilesStr)
{
	unsigned long i, ir, j, l;
	CString rra;

	if(n<2)
		return;

	l = (n >> 1) + 1;
	ir = n;

	for(;;)
	{
		if(l > 1)
		{
			rra = (*ListOfFilesStr)[--l - 1];
		}
		else
		{
			rra = (*ListOfFilesStr)[ir-1];
			(*ListOfFilesStr)[ir-1] = (*ListOfFilesStr)[0];
			if(--ir == 1)
			{
				(*ListOfFilesStr)[0] = rra;
				break;
			}
		}
		i = l;
		j = l+l;
		while(j <= ir)
		{
			if(j < ir && (*ListOfFilesStr)[j-1] < (*ListOfFilesStr)[j])
				j++;
			if(rra < (*ListOfFilesStr)[j-1])
			{
				(*ListOfFilesStr)[i-1] = (*ListOfFilesStr)[j-1];
				i=j;
				j <<= 1;
			}
			else
				j = ir+1;
		}
		(*ListOfFilesStr)[i-1] = rra;
	}
}
*/

// Original numrec heap sort fuer floats
void hpsort(unsigned long n, float ra[])
{
	unsigned long i, ir, j, l;
	float rra;

	if(n<2)
		return;

	l = (n >> 1) + 1;
	ir = n;

	for(;;)
	{
		if(l > 1)
		{
			rra = ra[--l];
		}
		else
		{
			rra = ra[ir];
			ra[ir] = ra[1];
			if(--ir == 1)
			{
				ra[1] = rra;
				break;
			}
		}
		i = l;
		j = l+l;
		while(j <= ir)
		{
			if(j < ir && ra[j] < ra[j+1])
				j++;
			if(rra < ra[j])
			{
				ra[i] = ra[j];
				i=j;
				j <<= 1;
			}
			else
				j = ir+1;
		}
		ra[i] = rra;
	}
}


#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);a[k][l]=h+s*(g-h*tau);

void jacobi(float **a, int n, float d[], float **v, int *nrot)
{
	int j,iq,ip,i;
	float tresh,theta,tau,t,sm,s,h,g,c,*b,*z;
	
	b=vector(1,n);
	z=vector(1,n);
	for (ip=1;ip<=n;ip++) {
		for (iq=1;iq<=n;iq++) v[ip][iq]=0.0;
		v[ip][ip]=1.0;
	}
	for (ip=1;ip<=n;ip++) {
		b[ip]=d[ip]=a[ip][ip];
		z[ip]=0.0;
	}
	*nrot=0;
	for (i=1;i<=50;i++) {
		sm=0.0;
		for (ip=1;ip<=n-1;ip++) {
			for (iq=ip+1;iq<=n;iq++)
				sm += fabs(a[ip][iq]);
		}
		if (sm == 0.0) {
			free_vector(z,1,n);
			free_vector(b,1,n);
			return;
		}
		if (i < 4)
			tresh=0.2*sm/(n*n);
		else
			tresh=0.0;
		for (ip=1;ip<=n-1;ip++) {
			for (iq=ip+1;iq<=n;iq++) {
				g=100.0*fabs(a[ip][iq]);
				if (i > 4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
					&& (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
					a[ip][iq]=0.0;
				else if (fabs(a[ip][iq]) > tresh) {
					h=d[iq]-d[ip];
					if ((float)(fabs(h)+g) == (float)fabs(h))
						t=(a[ip][iq])/h;
					else {
						theta=0.5*h/(a[ip][iq]);
						t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0) t = -t;
					}
					c=1.0/sqrt(1+t*t);
					s=t*c;
					tau=s/(1.0+c);
					h=t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq]=0.0;
					for (j=1;j<=ip-1;j++) {
						ROTATE(a,j,ip,j,iq)
					}
					for (j=ip+1;j<=iq-1;j++) {
						ROTATE(a,ip,j,j,iq)
					}
					for (j=iq+1;j<=n;j++) {
						ROTATE(a,ip,j,iq,j)
					}
					for (j=1;j<=n;j++) {
						ROTATE(v,j,ip,j,iq)
					}
					++(*nrot);
				}
			}
		}
		for (ip=1;ip<=n;ip++) {
			b[ip] += z[ip];
			d[ip]=b[ip];
			z[ip]=0.0;
		}
	}
	nrerror("Too many iterations in routine jacobi");
}
#undef ROTATE

void eigsrt(float d[], float **v, int n)
{
	int k,j,i;
	float p;
	
	for (i=1;i<n;i++) {
		p=d[k=i];
		for (j=i+1;j<=n;j++)
			if (d[j] >= p) p=d[k=j];
			if (k != i) {
				d[k]=d[i];
				d[i]=p;
				for (j=1;j<=n;j++) {
					p=v[j][i];
					v[j][i]=v[j][k];
					v[j][k]=p;
				}
			}
	}
}



// Own matrix routines - ZERO-based
//

// general matrix mult
//
void matrix_matrix_mult(float **a, float **b, float **c, int n, int s, int m)
{
	float sum;
	register int i, j, k;

	for(i=0; i<n; i++)
	{
		for(j=0; j<m; j++)
		{
			sum = 0.0f;

			for(k=0; k<s; k++)
			{
				sum += a[i][k]*b[k][j];
			}
			c[i][j] = sum;
		}
	}
}

// multiply transposed matrix with itself; input is transpose of a (-> we can use pointer arithmetic over columns)
// alternatively we could accept a and transpose it here for fast pointer arithmetic
//
void transposematrix_matrix_mult(float **a, float **c, int n, int s)
{
	register float sum;
	register float *col_a, *col_b;
	register int i, j, k;
	
	for(i=0; i<n; i++)
	{
		for(j=0; j<=i; j++)
		{
			sum = 0.0f;
			
			col_a = &(a[i][0]);
			col_b = &(a[j][0]);
			
			for(k=0; k<s; k++, col_a++, col_b++)
			{
				// sum += a[i][k]*b[k][j];
				sum += *col_a * *col_b;
			}
			c[i][j] = sum;
		}
	}
	
	// now fill other triangle
	for(i=0; i<n; i++)
	{
		for(j=0; j<i; j++)
		{
			c[j][i] = c[i][j];
		}
	}
}

void matrix_vector_mult(float **a, float *b, float *c, int n, int s)
{
	int i, j, k;

	for(i=0; i<n; i++)
	{
		c[i] = 0.0f;

		for(k=0; k<s; k++)
		{
			c[i] += a[i][k]*b[k];
		}
	}
}

float vector_vector_mult(float *a, float *b, int n)
{
	int i;
	float out = 0.0f;

	for(i=0; i<n; i++)
	{
		out += a[i]*b[i];
	}
	return out;
}

void matrix_transpose(float **m, float **t, int n, int s)
{
	int i, j;

	for(i=0; i<n; i++)
	{
		for(j=0; j<s; j++)
		{
			t[j][i] = m[i][j];
		}
	}
}

// copy a matrix in different modes (to go back and forth between 1-based NumRecipies and own zero-based matrices
//                                   mode 0: im 0-based, om 0-based
//                                   mode 1: im 1-based, om 1-based
//									 mode 2: im 0-based, om 1-based
//                                   mode 3: im 1-based, om 0-based
void copy_matrix(float **im, float **om, int n, int s, int copy_mode)
{
	int i, j;
	
	if(copy_mode == 0)
	{
		for(i=0; i<n; i++)
		{
			for(j=0; j<s; j++)
			{
				om[i][j] = im[i][j];
			}
		}
	}
 	else if(copy_mode == 1)
	{
		for(i=1; i<=n; i++)
		{
			for(j=1; j<=s; j++)
			{
				om[i][j] = im[i][j];
			}
		}
	}
	else if(copy_mode == 2)
	{
		for(i=0; i<n; i++)
		{
			for(j=0; j<s; j++)
			{
				om[i+1][j+1] = im[i][j];
			}
		}
	}
	else if(copy_mode == 3)
	{
		for(i=0; i<n; i++)
		{
			for(j=0; j<s; j++)
			{
				om[i][j] = im[i+1][j+1];
			}
		}
	}
}

// for building inverse matrix from SVD result
// input is a vector representing the diagonal non-zero elements
// this effectively multiplies row i of matrix m with w[i]
// Note that no result matrix is produced but matrix is directly changed
//
void diagmatrix_matrix_mult(float *w, float **m, int n, int s)
{
	int i, j;

	for(i=0; i<n; i++)
	{
		for(j=0; j<s; j++)
		{
			m[i][j] = m[i][j] * w[i];
		}
	}
}

// computes the inverse of square matrix "m" with resolution n x n, result is returned in "inv" (mem must be prepared BEFORE calling this fn, i.e. for iXXG)
//
bool getInverseMatrixUsingSVD(float **m, float **inv, int n)
{
	float **u, **v, *w;
	u=matrix(1,n,1,n);
	v=matrix(1,n,1,n);
	w=vector(1,n);
	
	float **u0, **v0, **tmpm;		// just for our matrix mult routines (0-based versions of u, v)
	u0=matrix(0,n-1,0,n-1);
	v0=matrix(0,n-1,0,n-1);
	tmpm=matrix(0,n-1,0,n-1);

	float wmin, wmax=0.0f;
	copy_matrix(m, u, n, n, 2);

	int i, j;	
	
	// Compute singular value decomposition (SVD) for matrix XX (copied to 1-based u)
	//
	svdcmp(u, n, n, w, v);
	
	if( NoConvergenceInSVD ) // if no convergence after 100 its, switch negative w's to positive so that diagnostic (assuming only positive w's) works below
	{
		for(j=1;j<=n;j++)
		{
			w[j] = QX_ABS(w[j]); 
		}
	}

	// check if matrix is (close to) singular and if so, set respective w elements to zero
	//
	for(j=1;j<=n;j++)
	{
		if(w[j] > wmax) 
			wmax = w[j];
	}

	wmin=wmax*1.0e-6;			// Threshold for singular values allowed to be nonzero

	bool zero_w_found = false;
	for(j=1;j<=n;j++)
	{
		if(w[j] < wmin)
		{
			w[j] = 0.0;

			zero_w_found = true;
		}
		else 
			w[j] = 1.0 / w[j];	// inverse of "diag matrix" w
	}

	if(1) // zero_w_found)
	{
		/*if(zero_w_found)
			qxMessageBox("INFO - A matrix has been inverted which is (close to) singular. For details, inspect the Log tab in the sidebar.");

		qxLogText(" ");
		qxLogText("// Matrix inversion using Singular Value Decomposition (SVD)");
		if( NoConvergenceInSVD )
			qxLogText("// SVD: No convergence reached in 100 iterations !");
		qxLogText("// SVD: Check for singular matrix using values of diagonal elements of matrix \"w\" ");
		qxLogText("// SVD: Matrix \"w\" - zero values indicate reduced dimensionality: ");
		
		char cText[101];
		for (j=1;j<=n;j++)
		{
			sprintf(cText, "//%i:  %f", i, w[j]);
			qxLogText( cText );
		} */
	}
				
	// now get inverse (i.e. iXXGLM) from u, v, w	   (we could alternatively also use backsubstitution approach ("svbksb(u,w,v,n,n,b,x)" in calling routines but constructing ONE inverse matrix is nicer)
	//
	copy_matrix(u, tmpm, n, n, 3);
	matrix_transpose(tmpm, u0, n, n);
	diagmatrix_matrix_mult(w+1, u0, n, n);
	copy_matrix(v, tmpm, n, n, 3);
	copy_matrix(tmpm, v0, n, n, 0); // just copy, not transpose since v is already transpose of vT in SVD // 	matrix_transpose(tmpm, v0, n, n);
	matrix_matrix_mult(v0, u0, inv, n, n, n);
	
	// now finally clean temporary memory
	free_matrix(u0,0,n-1,0,n-1);
	free_matrix(v0,0,n-1,0,n-1);
	free_matrix(tmpm,0,n-1,0,n-1);
	
	free_matrix(v,1,n,1,n);
	free_matrix(u, 1,n,1,n);  // old version: // free_matrix(u,1,NPT,1,n);	
	free_vector(w,1,n);

	return true;
}

// generic call to matrix-matrix mult routine: try using fast platform-specific math libs
// if a fast lib is not available, fall back to own matrix multiplication routine
//
// matrix are given in numrec **float type; for fast routines *float pointers to data block are given
//
void fast_matrix_matrix_mult(float **a, float **b, float **c, int m, int n, int k, bool tA, bool tB)
{
	//i.e. **a -> DMt, **b -> DM, **c -> XX

	transposematrix_matrix_mult(a, c, m, k);
}	


