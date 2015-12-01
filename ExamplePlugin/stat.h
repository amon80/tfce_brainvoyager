#ifndef STAT_H
#define STAT_H

/*
NUMERICAL RECIPES SOFTWARE
P.O. Box 243, Cambridge, MA 02238 (USA)
License Terms and Conditions (Single-Screen ``Shrinkwrap'')

The Numerical Recipes software (``our Software'') consists of the computer programs that appear in the Numerical Recipes series of books published by
Cambridge University Press, plus a few extra, incidental programs that we include with most distributions. Our software is copyrighted, and you can use
it only if you have a license from us. Unless you have an individually signed license that supersedes the terms on this page, your use of our software
signifies your agreement with these terms (``this shrinkwrap license'').
READ THIS PAGE CAREFULLY. IF YOU DO NOT ACCEPT THE TERMS, THEN REMOVE ALL NUMERICAL RECIPES SOFTWARE FROM YOUR MACHINE AND CONTACT US WITHIN 30 DAYS
OF YOUR RECEIPT OF THIS LICENSE. If the problem cannot be resolved, and if you have destroyed all unlicensed copies of our software in your possession,
we will refund your money.

Under this shrinkwrap license, our software is licensed by the screen, not by the CPU. You must have a separate license (or unit of a multiple unit license)
for each physical screen on which our software is actually used. (You must also have a separate license for each computer language version of our software,
e.g., C or FORTRAN.) A workstation screen, an X-terminal, a PC or Macintosh screen, or an ascii terminal each count as a single screen. You must decide which
physical single screen this license applies to, and you must identify that screen to us, on request, by a serial number or other unique identifier. (This is
not a "floating" license.) You may transfer this license from one screen to another, but only after notifying us, by regular mail in writing, of your intent to do so.

On a licensed screen, you may use, modify, or incorporate our software into your own computer programs. Incorporated or modified versions of our software still
belong to us. However, you may transfer pre-compiled, executable programs incorporating our software to other, unlicensed, screens or computers, providing
that the following two conditions hold (i) the programs were first developed, compiled, and successfully run on a licensed screen, and (ii) our software is
bound into the programs in such a manner that it cannot be accessed as individual routines and cannot practicably be unbound and used in other programs.
Specifically, under this license, your program user must not be able to use our programs as part of a program library or ``mix-and-match'' workbench.

If you sell or otherwise commercially distribute programs that have our software bound into them (as above), you must additionally (i) notify us in advance
of such use and describe the nature of the use, (ii) include our Copyright notice within your machine-readable distribution (you need not display it when your
program runs), and (iii) insure that no business relationship is created between us, or the authors of the Numerical Recipes books, and your users.

Otherwise, this license does not authorize you to transfer, publish, disclose, display, or otherwise make available our software in any form (whether source
or object code) to other organizations or persons. In particular, you may not put our software on a computer bulletin board or anonymous ftp directory from
which it could be downloaded or copied by unlicensed users.

In using our software, you acknowledge and accept this DISCLAIMER OF WARRANTY:

OUR SOFTWARE IS FURNISHED ``AS IS". WE MAKE NO WARRANTIES, EXPRESS OR IMPLIED, THAT OUR SOFTWARE IS FREE OF ERROR, OR IS CONSISTENT WITH ANY PARTICULAR STANDARD
OF MERCHANTABILITY, OR THAT IT WILL MEET YOUR REQUIREMENTS FOR ANY PARTICULAR APPLICATION. WE ACCEPT NO RESPONSIBILITY FOR ANY MATHEMATICAL OR TECHNICAL LIMITATIONS
OF THE PROCEDURES AND FUNCTIONS WHICH MAKE UP OUR SOFTWARE. OUR SOFTWARE SHOULD NOT BE RELIED ON FOR SOLVING A PROBLEM WHOSE INCORRECT SOLUTION COULD RESULT IN
INJURY TO A PERSON OR LOSS OF PROPERTY. NEITHER NUMERICAL RECIPES SOFTWARE, NOR THE AUTHORS OF THE BOOK ``NUMERICAL RECIPES: THE ART OF SCIENTIFIC COMPUTING",
NOR THE PUBLISHER OF THAT BOOK SHALL IN ANY EVENT BE LIABLE FOR ANY DAMAGES, WHETHER DIRECT OR INDIRECT, SPECIAL OR GENERAL, CONSEQUENTIAL OR INCIDENTAL, ARISING
FROM USE OF OUR SOFTWARE. YOUR USE OF OUR SOFTWARE IS ENTIRELY AT YOUR OWN RISK.

Numerical Recipes Software
Cambridge, Massachusetts
(shrinkwrap version 1/1/96) 
*/

#include "global.h"
#include "nrutil.h"

static bool NoConvergenceInSVD;

extern void ttest(float data1[], unsigned long n1, float data2[], unsigned long n2, float *t, float *prob);
extern void tutest(float data1[], unsigned long n1, float data2[], unsigned long n2, float *t, float *prob);
extern void kstwo(float data1[], unsigned long n1, float data2[], unsigned long n2, float *d, float *prob);
extern void sort(unsigned long n, float arr[]);
extern void sort2(unsigned long n, float arr[], float brr[]);
extern void pearsn(float x[], float y[], unsigned long n, float *r, float *prob, float *z);
extern void pearsn_inc(float x, float y, float& n, float& ax, float& ay, float& sxx, float& syy, float& sxy, float *r);
extern void regr_inc(float x, float y, float& n, float& ax, float& ay, float& sxx, float& syy, float& sxy, float *a, float *b, float *ym);
extern void pearsn_prob(float x[], float y[], unsigned long n, float *r, float *prob, float *z);
extern float t_to_p(float t, unsigned long n);
extern float r_to_p(float r, unsigned long n);
extern float F_to_p(float F, unsigned long df_1, unsigned long df_2);
extern float p_to_t(float p, unsigned long df);
extern float p_to_r(float p, unsigned long df);
extern float p_to_F(float p, unsigned long df_1, unsigned long df_2);
extern void spear(float data1[], float data2[], unsigned long n, float *d, float *zd, float *probd, float *rs, float *probrs);
extern void crank(unsigned long n, float w[], float *s);
extern float erfcc(float x);
extern float probks(float alam);
extern void sort(unsigned long n, float arr[]);
extern float betai(float a, float b, float x);
extern float betacf(double a, double b, double x);
extern double gammln(double xx);
extern void avevar(float data[], unsigned long n, float *ave, float *var);
extern void fit(float x[], float y[], int ndata, float sig[], int mwt, float *a,
	float *b, float *siga, float *sigb, float *chi2, float *q);
extern float gammq(float a, float x);
extern float gasdev(long *idum);
extern void gcf(float *gammcf, float a, float x, float *gln);
extern void gser(float *gamser, float a, float x, float *gln);
extern float ran1(long *idum);

extern void realft(float data[], unsigned long n, int isign);
extern void four1(float data[], unsigned long nn, int isign);
extern void rlft3(float ***data, float **speq, unsigned long nn1, unsigned long nn2, unsigned long nn3, int isign);
extern void fourn(float data[], unsigned long nn[], int ndim, int isign);
extern float select(unsigned long k, unsigned long n, float arr[]);
extern unsigned short select_ushort(unsigned long k, unsigned long n, unsigned short arr[]);

extern float p_to_z(float pvalue);

extern void svdfit(float x[], float y[], float sig[], int ndata, float a[], int ma,	float **u, float **v, float w[], float *chisq, void (*funcs)(float, float [], int));
extern void svdvar(float **v, int ma, float w[], float **cvm);
extern void svdcmp(float **a, int m, int n, float w[], float **v);
extern void svbksb(float **u, float w[], float **v, int m, int n, float b[], float x[]);

extern float pythag(float a, float b);

extern void fpoly(float x, float p[], int np);
extern void fgauss(float x, float a[], float *y, float dyda[], int na);
extern void mrqmin(float x[], float y[], float sig[], int ndata, float a[], int ia[],
	int ma, float **covar, float **alpha, float *chisq,
	void (*funcs)(float, float [], float *, float [], int), float *alamda);
extern void mrqcof(float x[], float y[], float sig[], int ndata, float a[], int ia[],
	int ma, float **alpha, float beta[], float *chisq,
	void (*funcs)(float, float [], float *, float [], int));

extern void gaussj(float **a, int n, float **b, int m);
extern void covsrt(float **covar, int ma, int ia[], int mfit);

extern void hpsort(unsigned long n, float ra[]);

extern void jacobi(float **a, int n, float d[], float **v, int *nrot);
extern void eigsrt(float d[], float **v, int n);

// simple matrix-matrix and matrix-vector mult fns
//
extern void matrix_matrix_mult(float **a, float **b, float **c, int n, int s, int m);
extern void transposematrix_matrix_mult(float **a, float **c, int n, int s);
extern void matrix_vector_mult(float **a, float *b, float *c, int n, int s);
extern float vector_vector_mult(float *a, float *b, int n);
extern void matrix_transpose(float **m, float **t, int n, int s);
extern void copy_matrix(float **im, float **om, int n, int s, int copy_mode);
extern void diagmatrix_matrix_mult(float *w, float **m, int n, int s);
extern bool getInverseMatrixUsingSVD(float **m, float **inv, int n);

#endif // STAT_H

