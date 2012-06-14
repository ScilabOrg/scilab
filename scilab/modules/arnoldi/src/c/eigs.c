/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 -Scilab Enterprises - Adeline CARNIS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "eigs.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
// dgemm performs one of the matrix-matrix operations
extern int C2F(dgemm)(char* transa, char* transb, int* m, int* n, int* k, double* alpha, double* A, int* lda, double* B, int* ldb, double* beta, double* C, int* ldc); 

// zgemm performs one of the matrix-matrix operations
extern int C2F(zgemm)();

// dgetrf computes an LU factorization of a general M by N matrix A (double) using partial pivoting with row interchanges
extern int C2F(dgetrf)(int* m, int* n, double* A, int* lda, int* ipiv, int* info); 

// zgetrd computes an LU factorization of a general M by N matrix A (complex*16) using partial pivoting with row interchanges
extern int C2F(zgetrf)(int* m, int* n, doublecomplex* A, int* lda, int* ipiv, int* info); 

// dlaswp performs a series of row interchanges on the matrix A
// one row interchange is initiated for each of rows k1 through k2 of A
extern int C2F(dlaswp)(int* n, double* A, int* lda, int* k1, int* k2, int* ipiv, int* incx);

// dpotrf computes the cholesky factorization of a real symmetric positive definite matrix A
extern int C2F(dpotrf)(char* uplo, int* n, double* A, int* lda, int* info);		

// zpotrf computes the cholesky factorization of a real hermitian positive definite matrix A
extern int C2F(zpotrf)(char* uplo, int* n, doublecomplex* A, int* lda, int* info);	

// dgetri computes the inverse of a matrix using the LU factorization computed by dgetrf
extern int C2F(dgetri)(int* n, double* A, int* lda, int* ipiv, double* work, int* lworkl, int* info); 

// zgetri computes the inverse of a matrix using the LU factorization computed by zgetrf
extern int C2F(zgetri)(int* n, doublecomplex* A, int* lda, int* ipiv, doublecomplex* work, int* lworkl, int* info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dsaupd)(int *ido, char *bmat, int *n, char *which, int *nev, 
		       double *tol, double *resid, int *ncv, double *v, 
		       int *ldv, int *iparam, int *ipntr, double *workd, 
		       double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dseupd)(int *rvec, char *howmny, int *select, double *d, 
		       double *z, int *ldz, double *sigma, char *bmat, 
		       int *n, char *which, int *nev ,double *tol, 
		       double *resid, int *ncv, double *v ,int *ldv, 
		       int *iparam,int *ipntr, double *workd,double *workl,
		       int *lworkl,int *info, unsigned long rvec_length, 
		       unsigned long howmany_length, 
		       unsigned long bmat_length, unsigned long which_len);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dnaupd)(int *ido, char *bmat, int *n, char *which, int *nev, 
		       double *tol, double *resid, int *ncv, double *v, 
		       int *ldv, int *iparam, int *ipntr, double *workd, 
		       double *workl, int *lworkl, int *info, 
		       unsigned long bmat_len, unsigned long which_len);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dneupd)(int *rvec, char *howmny, int *select, double *dr, 
		       double *di, double *z, int *ldz, double *sigmar, 
		       double *sigmai, double *workev, char *bmat, int *n, 
		       char *which, int *nev, double *tol, double *resid, 
		       int *ncv, double *v, int *ldv, int *iparam, int *ipntr, 
		       double *workd, double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(znaupd)(int * ido, char * bmat, int * n, char * which, 
		       int * nev, double * tol, doublecomplex * resid, 
		       int * ncv, doublecomplex * v, int * ldv, int * iparam,
		       int * ipntr, doublecomplex * workd, 
		       doublecomplex * workl, int * lworkl, double * rwork, 
		       int * info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(zneupd)(int * rvec, char * howmny, int * select, 
		       doublecomplex * d, doublecomplex * z, int * ldz, 
		       doublecomplex * sigma, doublecomplex * workev, 
		       char * bmat, int * n, char * which, int * nev, 
		       double *  tol, doublecomplex * resid, int * ncv,
		       doublecomplex * v, int * ldv, int * iparam, int * ipntr,
		       doublecomplex * workd, doublecomplex * workl, 
		       int * lworkl, double * rwork, int * info);
/*--------------------------------------------------------------------------*/

// FIXME : remove me
//#define free(x) 

static double alpha = 1.;
static double beta = 0.;

int eigs(double *AR, doublecomplex *AC, int N, int Acomplex, int Asym, double* B, doublecomplex* BC, int Bcomplex,int matB,double* NEV, doublecomplex* SIGMA,char* which, double* maxiter,
	double* tol, double* NCV, double* RESID, doublecomplex* RESIDC, int* INFO, double* cholB, int* INFO_EUPD, doublecomplex* eigenvalue, doublecomplex* eigenvector)
{ 
	// GENERAL VARIABLES
	int iter = 0;
	int i = 0;
	int j = 0;
	int	k = 0;
	int	l = 0;
	double sum = 0;
	double elemt1 = 0;
	double elemt2 = 0;
	double min_U = 0;
	double max_U = 0;
	doublecomplex sumc;
	doublecomplex ec1;
	doublecomplex ec2;

	int INFO_CHOL = 0;
	int INFO_LU = 0;

	int k1 = 1;

	// VARIABLES DSAUPD, DNAUPD, ZNAUPD 	
	int LWORKL = 0;	
	int IDO = 0;
	int LDV = Max(1,N);
	int nev = (int) NEV[0];
	int ncv = 0;

	int* IPARAM	= NULL;
	int* IPNTR = NULL;

	double* V = NULL;
	doublecomplex* VC = NULL;

	double* WORKD = NULL;
	doublecomplex* WORKDC = NULL;

	double* WORKL = NULL;
	doublecomplex* WORKLC = NULL;

	double* RWORK = NULL;

	char* bmat = "I";

	// VARIABLES DSEUPD, DNEUPD, ZNEUPD
	int RVEC = 0;	// compute eigenvalues if RVEC = 1 also compute eigenvalues and eigenvectors 

	int* SELECT = NULL;

	double* D = NULL;
	double* DI = NULL;
	double* DR = NULL;
	doublecomplex* DC = NULL;

	double* WORKEV = NULL;
	doublecomplex* WORKEVC = NULL;
	
	char* HOWMNY = "A";
	
	double* Z = NULL;
	doublecomplex* ZC  = NULL;
	
	double SIGMAR = SIGMA[0].r;
	double SIGMAI = SIGMA[0].i;


	
	double *R = (double*) malloc(N*N*sizeof(double));
	double *Rprime = (double*) malloc(N*N*sizeof(double));
	double *TRIB = NULL;

	double *INVR = NULL; 
	double *INVRPRIME = NULL;
	double* work	= NULL; 
	double* AMSB = NULL;
	
	double* L = NULL;
	double* U = NULL;
	double* E = NULL;

	double* INVL = NULL;
	double* INVU = NULL;
	double* INVE = NULL;

	doublecomplex* RC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
	doublecomplex* RCprime = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
	doublecomplex *TRIBC = NULL;

	doublecomplex* INVRC = NULL;
	doublecomplex* INVRCPRIME = NULL;
	doublecomplex* AMSBC = NULL;

	doublecomplex* LC = NULL;
	doublecomplex* UC = NULL;
	doublecomplex* EC = NULL;

	doublecomplex* INVLC = NULL;
	doublecomplex* INVUC = NULL;
	
	doublecomplex* workc = NULL;
	
	int* IPVT = NULL; 

	double* toto = NULL;
	doublecomplex* totoc = NULL;

	double* res = NULL;
	doublecomplex* resc = NULL;
	res = (double*) malloc(N*N*sizeof(double));
	toto = (double*) malloc(N*N*sizeof(double));
	resc = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
	totoc = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
	EC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));

	IPARAM = (int*) malloc(11*sizeof(int));
	memset(IPARAM, 0, 11*sizeof(int));
	IPARAM[0] = 1;
	IPARAM[2] = (int) maxiter[0];
	IPARAM[6] = 1; // by default mode = 1
	
	IPNTR = (int*) malloc(14*sizeof(int));
	memset(IPNTR, 0, 14*sizeof(int));
	
	// END VARIABLES

	// MODE
	if ((strcmp(which,"SM")==0) || (SIGMAR !=0 || SIGMAI !=0))
	{
		IPARAM[6] = 3;
		which = "LM";
	}

	// BMAT
	if ((matB == 0) || (IPARAM[6]==1))  // if B = [] or mode = 1 -> bmat = 'I' : standart eigenvalue problem
	{
		bmat = "I";
	}
	else   // generalized eigenvalue problem
	{
		bmat = "G";
	}

	// OBTAIN THE UPPER TRIANGULAR MATRIX B IF GENERALIZED EIGENVALUE PROBLEM
	if (matB != 0)
	{
		if (!Bcomplex) // B is real
		{
			TRIB = (double*) malloc(N*N*sizeof(double));
			memset(TRIB, 0, N*N*sizeof(double));
			for (i=0; i<N; i++)
			{
				for (j=0; j<N; j++)
				{
					if (j<=i)
					{
						TRIB[j+i*N] = B[j+i*N];
					}
				}
			}
		}
		else  // B is complex
		{
			TRIBC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
			memset(TRIBC, 0, N*N*sizeof(doublecomplex));
			for (i=0;i<N; i++)
			{
				for (j=0; j<N; j++)
				{
					if (j<=i)
					{
						TRIBC[j+i*N].r = BC[j+i*N].r;
						TRIBC[j+i*N].i = BC[j+i*N].i;
					}
				}
			}
		}
	}

	// NCV
	if (NCV == NULL) 
	{
		//sciprint("toto \n");
		if(Asym == 0 && !(Acomplex == 1 || Bcomplex == 1)) // if dnaupd  ncv = 2*nev+1
		{
			ncv = Max(2*nev+1,20);
		}
		else // if dsaupd or znaupd ncv = 2*nev
		{
			ncv = Max(2*nev,20);
		}

		if(ncv>N)
		{
			ncv = N;
		}
	}
	else
	{
		ncv = (int) NCV[0];
		if (ncv<=nev || ncv>N) // Error
		{
			return -1;						
		}
	}

	if (Acomplex == 0 && Bcomplex == 0 && Asym == 1)
	{
		if (nev>=N)	// Error
		{
			return -2;
		}
	}
	else
	{
		if (nev>=N-1)
		{
			return -2;
		}
	}

	// B must be symmetric (hermitian) positive (semi-) positive
	if (matB!=0)
	{
		if (cholB[0]) // Comparison between B and upper triangular matrix TRIB: if TRIB and B are not equal -> ERROR 
		{
			if (!Bcomplex)  // B is real
			{		
				for (i=0; i<N*N; i++)
				{
					if (TRIB[i] != B[i]) // Error
					{
						return -3;
					}
				}
			}
			else	// if B is complex
			{
				for (i=0; i<N*N; i++)
				{
					if (TRIBC[i].r != BC[i].r || TRIBC[i].i !=BC[i].i)	// Error
					{
						return -3;
					}
				}
			}

		}
		else
		{
			// B is symmetric ???
			if(!Bcomplex) // if B is real
			{
				for (i=0; i<N; i++)
				{
					for (j=0; j<i; j++)
					{
						if (fabs(B[j+i*N]-B[j*N+i])>0)
						{
							return -4;
						}
					}
				}
			}
			else  // if B is complex
			{
				for (i=0; i<N; i++)
				{
					if (BC[i*N+i].i!=0)
					{
						return -4;
					}
				}

				for (i=1; i<N; i++)
				{
					for (j=1; j<N; j++)
					{
						ec1.r = BC[j-1+i*N].r;
						ec1.i = BC[j-1+i*N].i;
						ec2.r = BC[(j-1)*N+i].r;
						ec2.i = BC[(j-1)*N+i].i;
						if(fabs(ec1.r-ec2.r)>0 || fabs(ec1.i+ec2.i)>0)
						{
							return -4;
						}
					}
				}

			}
		}
	}

	if(cholB[0])
	{
		if(!Bcomplex)  // Cholesky B = R*Rprime with real B
		{
		// Compute the upper triangular matrix
			memcpy(Rprime, B, N*N*sizeof(double));
				
			// Compute the lower triangular matrix
			for (i=0; i<N; i++)
			{
				for (j=0; j<N; j++)
				{
					R[i*N+j] = B[j*N+i];
				}
			}
		}
		else   // Cholesky BC = RC*RCprime with complex B
		{
			// Computes the upper triangular matrix BC
			memcpy(RCprime, BC, N*N*sizeof(doublecomplex));

			// Computes the lower triangular matrix BC
			for (i=0;i<N; i++)
			{
				for (j=0; j<N; j++)
				{
					RC[i*N+j].r = BC[j*N+i].r;
					RC[i*N+j].i = (-1)*BC[j*N+i].i;
				}
			}
		}
	}


	if(!cholB[0] && IPARAM[6] == 1 && matB != 0)
	{
		if (!Bcomplex)  // B is real
		{	
			memcpy(R, B, N*N*sizeof(double));
			memcpy(Rprime, B, N*N*sizeof(double));					

			C2F(dpotrf)("L", &N, R, &N, &INFO_CHOL); // Compute the lower triangular matrix R
			if (INFO_CHOL!=0) // Errors
			{
				return -5;
			}

			for (i=0; i<N; i++)
			{
				for (j=0;j<N;j++)
				{
					if(i<j)
					{
						R[i+j*N] = 0;
					}
				}
			}
			
			for (i=0;i<N;i++)
			{
				for (j=0;j<N;j++)
				{
					sciprint("%f ", R[j+i*N]);
				}
				sciprint("\n");
			}
			sciprint("\n");

			C2F(dpotrf)("U", &N, Rprime, &N, &INFO_CHOL);   // Compute the upper triangular matrix Rprime
			if (INFO_CHOL!=0)
			{
				return -5;
			}
			for (i=0; i<N; i++)
			{
				for (j=0;j<N;j++)
				{
					if(j<i)
					{
						Rprime[i+j*N] = 0;
					}
				}
			}

			for (i=0;i<N;i++)
			{
				for (j=0;j<N;j++)
				{
					sciprint("%f ", Rprime[j+i*N]);
				}
				sciprint("\n");
			}

		}
		else	// B is complex
		{
			memcpy(RC,BC, N*N*sizeof(doublecomplex));
			memcpy(RCprime, BC, N*N*sizeof(doublecomplex));
					
			C2F(zpotrf)("L", &N, RC, &N, &INFO_CHOL); // Computes the lower triangular matrix 
			if (INFO_CHOL!=0)
			{
				return -5;
			}

			for (i=0; i<N; i++)
			{
				for (j=0;j<N;j++)
				{
					if(i<j)
					{
						RC[i+j*N].r = 0;
						RC[i+j*N].i = 0;
					}
				}
			}

			C2F(zpotrf)("U", &N, RCprime, &N, &INFO_CHOL);	// Computes the upper triangular matrix
			if (INFO_CHOL!=0)
			{
				return -5;
			}

			for (i=0; i<N; i++)
			{
				for (j=0;j<N;j++)
				{
					if(j<i)
					{
						RCprime[i+j*N].r = 0;
						RCprime[i+j*N].i = 0;
					}
				}
			}
		}
	}

	IPVT = (int*) malloc(N*sizeof(int));
	memset(IPVT, 0, N*sizeof(int));

	// MAIN
	if (!Acomplex && !Bcomplex)		// A and B are not complex
	{
		if(IPARAM[6] == 3)	// if mode = 3
		{
			AMSB = (double*) malloc(N*N*sizeof(double));
			memcpy(AMSB,AR,N*N*sizeof(double));
			// Compute LU decomposition AMSB = A- sigma*B
			if (matB == 0) // if B = [] -> standart eigenvalue problem : A-sigma
			{
				for (i=0; i<N; i++)
				{
					AMSB[i+i*N] = AMSB[i+i*N]-SIGMAR;
				}
			}
			else	// generalized eigenvalue problem
			{
				if (cholB[0])
				{
					for (i=0; i<N*N; i++)
					{
						AMSB[i] = AR[i] - (SIGMAR * R[i] * Rprime[i]);
					}
				}
				else
				{
					for (i=0;i<N*N;i++)
					{
						AMSB[i] = AR[i]-(SIGMAR*B[i]);
					}
				}
			}

			// LU decomposition
			C2F(dgetrf)(&N, &N, AMSB, &N, IPVT, &INFO_LU);

			// Computes the lower triangular matrix L
			L = (double*) malloc(N*N*sizeof(double));
			memset(L, 0, N*N*sizeof(double));

			for (i=0; i<N; i++)
			{
				for (j=0; j<N; j++)
				{
					if (i == j)
					{
						L[i+j*N] = 1;
					}
					else
					{
						if (i > j)
						{
							L[i+j*N] = AMSB[i+j*N];
						}
					}
				}
			}

			// Computes the upper triangular matrix U
			U = (double*) malloc(N*N*sizeof(double));
			memset(U, 0, N*N*sizeof(double));

			for (i=0; i<N; i++)
			{
				for(j=0; j<N; j++)
				{
					if(i <= j)
						U[i+j*N] = AMSB[i+j*N];
				}
			}

			// Computes the permutation matrix E
			E = (double*) malloc(N*N*sizeof(double));
			memset(E ,0, N*N*sizeof(double));

			for (i=0; i<N; i++)
			{
				E[i*N+i] = 1;
			}

			C2F(dlaswp)(&N, E, &N, &k1, &N, IPVT, &k1);

			// condition number
			min_U = Abs(U[0]);
			max_U = Abs(U[0]);
			for (i=1; i<N; i++)
			{
				min_U = Min(min_U, Abs(U[i*N+i]));
				max_U = Max(max_U, Abs(U[i*N+i]));
			}
			if((min_U/max_U) == 0)
			{
				// warning
			}			
			free(AMSB);
		}

		if (Asym)  // DSAUPD
		{
			LWORKL = ncv*ncv+8*ncv;

			V = (double*) malloc(N *ncv *sizeof(double));
			memset(V, 0, N*ncv*sizeof(double));

			WORKL = (double*) malloc(LWORKL*sizeof(double));
			memset(WORKL,0,LWORKL*sizeof(double));

			WORKD = (double*) malloc(3*N*sizeof(double));
			memset(WORKD,0,3*N*sizeof(double));
		}
		else	// DNAUPD
		{
			LWORKL = 3*ncv*(ncv+2);
			
			V = (double*) malloc(N*(ncv+1)*sizeof(double));
			memset(V,0,N*(ncv+1)*sizeof(double));

			WORKL = (double*) malloc(LWORKL*sizeof(double));
			memset(WORKL,0,LWORKL*sizeof(double));

			WORKD = (double*) malloc(3*N*sizeof(double));
			memset(WORKD,0,3*N*sizeof(double));
		}
		
		INVR = (double*) malloc(N*N*sizeof(double));
		work = (double*) malloc(N*N*sizeof(double));
		INVRPRIME = (double*) malloc(N*N*sizeof(double));
		INVL = (double*) malloc(N*N*sizeof(double));
		INVU = (double*) malloc(N*N*sizeof(double));
		INVE = (double*) malloc(N*N*sizeof(double));

		while(IDO != 99)
		{
			if (Asym == 1) // DSAUPD
			{
				C2F(dsaupd)(&IDO, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO[0]);
			}
			else	// DNAUPD
			{
				C2F(dnaupd)(&IDO, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO[0],1L,2L);
			}

			if (INFO[0] < 0)
			{
				return -6;
			}

			if(IDO == -1 || IDO == 1 || IDO == 2)
			{
				if (IPARAM[6]==1)  // mode = 1
				{
					if (matB == 0) // B = [] -> standart eigenvalue problem
					{
						// OP = A*x
						l = IPNTR[1]-1;
						for (i=0; i<N; i++)
						{
							k = IPNTR[0]-1;
							sum = 0;
							for (j=i; j<N*N; j=j+N)
							{
								sum += AR[j] * WORKD[k];
								k++;
							}
							WORKD[l] = sum;
							l++;
						}
					}
					else // generalized eigenvalue problem
					{
						memset(toto, 0, N*N*sizeof(double));
						memset(res, 0, N*N*sizeof(double));
						// OP = inv(R')*A*inv(R)*x
						// inv(R)
						memcpy(INVR, R, N*N*sizeof(double));   // copy R to invR

						memset(IPVT, 0, N*sizeof(int));
						C2F(dgetrf)(&N, &N, INVR ,&N, IPVT, &INFO_LU);	// LU decomposition

						memset(work, 0, N*N*sizeof(double));
						C2F(dgetri)(&N, INVR, &N, IPVT, work, &N, &INFO_LU);  // Obtain inverse matrix R
						
						// inv(Rprime)
						memcpy(INVRPRIME, Rprime, N*N*sizeof(double));
														
						memset(IPVT, 0, N*sizeof(int));
						C2F(dgetrf)(&N, &N, INVRPRIME, &N, IPVT, &INFO_LU);	// LU decomposition

						memset(work, 0, N*N*sizeof(double));
						C2F(dgetri)(&N, INVRPRIME, &N, IPVT, work, &N,&INFO_LU);	// Obtain inverse matrix Rprime

						C2F(dgemm)("n","n",&N,&N,&N,&alpha,INVR,&N,AR,&N,&beta,toto,&N);

						C2F(dgemm)("n","n",&N,&N,&N,&alpha,toto,&N,INVRPRIME,&N,&beta,res,&N);

						// OP = inv(Rprime)*A*inv(R)*x
						l = IPNTR[1]-1;
						for (i=0; i<N; i++)
						{
							k = IPNTR[0]-1;
							sum = 0;
							for (j=i; j<N*N; j=j+N)
							{
								sum += res[j] * WORKD[k];
								k++;
							}
							WORKD[l] = sum;
							l++;
						}
					}
				}
				else
				{
					if (IPARAM[6] == 3)  // mode = 3
					{
						sciprint("mode 3 \n");
						if (matB == 0)  // B = [] -> standart eigenvalue problem
						{
							if (IDO == 2)
							{
								// y = B*x where B = I so workd[ipntr[1]-1:ipntr[1]+N-1] = workd[ipntr[0]-1:ipntr[0]+N-1]
								for (i=0; i<N; i++)
								{
									WORKD[IPNTR[1]-1+i] = WORKD[IPNTR[0]-1+i];
								}
							}
							else
							{
								memset(toto, 0, N*N*sizeof(double));
								memset(res, 0, N*N*sizeof(double));
								// workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[0]-1:ipntr[0]+N-1]
								// inv L -> L obtained with LU decomposition
								memcpy(INVL, L, N*N*sizeof(double));

								memset(IPVT, 0, N*sizeof(int));
								C2F(dgetrf)(&N, &N, INVL, &N, IPVT, &INFO_LU); // LU decomposition
								
								memset(work, 0, N*N*sizeof(double));
								C2F(dgetri)(&N, INVL, &N, IPVT, work, &N, &INFO_LU);  // inv(L)
								
								// inv U -> U obtained with LU decomposition
								memcpy(INVU, U, N*N*sizeof(double));

								memset(IPVT, 0, N*sizeof(int));
								C2F(dgetrf)(&N, &N, INVU, &N, IPVT, &INFO_LU); // LU decomposition
								
								memset(work, 0, N*N*sizeof(double));
								C2F(dgetri)(&N, INVU, &N, IPVT, work, &N, &INFO_LU); // inv(U)

								C2F(dgemm)("n","n",&N,&N,&N,&alpha,INVU,&N,INVL,&N,&beta,toto,&N);

								C2F(dgemm)("n","n",&N,&N,&N,&alpha,toto,&N,E,&N,&beta,res,&N);
								
								l = IPNTR[1]-1;
								for (i=0; i<N; i++)
								{
									k = IPNTR[0]-1;
									sum = 0;
									for (j=i; j<N*N; j=j+N)
									{
										sum += res[j] * WORKD[k];
										k++;
									}
									WORKD[l] = sum;
									l++;
								}
							}
						}
						else  // matB == 1 so B is not empty and bmat = 'G'-> generalized eigenvalue problem
						{
							if (IDO == 2)
							{
								if (cholB[0])  // workd[ipntr[1]-1:ipntr[1]+N-1] = R * Rprime * workd[ipntr[0]-1:ipntr[0]+N-1]
								{
									memset(res, 0, N*N*sizeof(double));
									C2F(dgemm)("n","n",&N,&N,&N,&alpha,R,&N,Rprime,&N,&beta,res,&N);

									l = IPNTR[1]-1;
									for (i=0; i<N; i++)
									{
										k = IPNTR[0]-1;
										sum = 0;
										for (j=i; j<N*N; j=j+N)
										{
											sum += res[j] * WORKD[k];
											k++;
										}
										WORKD[l] = sum;
										l++;
									}
								}
								else	//  workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
								{
									l = IPNTR[1]-1;
									for (i=0; i<N; i++)
									{
										k = IPNTR[0]-1;
										sum = 0;
										for (j=i; j<N*N; j=j+N)
										{
											sum += B[j] * WORKD[k];
											k++;
										}
										WORKD[l] = sum;
										l++;
									}
								}
							}
							else
							{
								if (IDO == -1)
								{
									if (cholB[0])   // workd[ipntr[1]-1:ipntr[1]+N-1] = R * Rprime * workd[ipntr[0]-1:ipntr[0]+N-1]
									{
										memset(res, 0, N*N*sizeof(double));
										C2F(dgemm)("n","n",&N,&N,&N,&alpha,R,&N,Rprime,&N,&beta,res,&N);
										l = IPNTR[1]-1;
										for (i=0; i<N; i++)
										{
											k = IPNTR[0]-1;
											sum = 0;
											for (j=i; j<N*N; j=j+N)
											{
												sum += res[j] * WORKD[k];
												k++;
											}
											WORKD[l] = sum;
											l++;
										}
									}
									else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
									{
										l = IPNTR[1]-1;
										for (i=0; i<N; i++)
										{
											k = IPNTR[0]-1;
											sum = 0;
											for (j=i; j<N*N; j=j+N)
											{
												sum += B[j] * WORKD[k];
												k++;
											}
											WORKD[l] = sum;
											l++;
										}
									}
									// compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[1]-1:ipntr[1]+N-1]

									// inv L -> L obtained with LU decomposition
									memset(toto, 0, N*N*sizeof(double));
									memset(res, 0, N*N*sizeof(double));
									memcpy(INVL, L, N*N*sizeof(double));

									memset(IPVT, 0, N*sizeof(int));
									C2F(dgetrf)(&N, &N, INVL, &N, IPVT, &INFO_LU); // LU decomposition
									
									memset(work, 0, N*N*sizeof(double));
									C2F(dgetri)(&N, INVL, &N, IPVT, work, &N, &INFO_LU);  // inv(L)
									
									// inv U -> U obtained with LU decomposition
									memcpy(INVU, U, N*N*sizeof(double));

									memset(IPVT, 0, N*sizeof(int));
									C2F(dgetrf)(&N, &N, INVU, &N, IPVT, &INFO_LU); // LU decomposition
									
									memset(work, 0, N*N*sizeof(double));
									C2F(dgetri)(&N, INVU, &N, IPVT, work, &N, &INFO_LU); // inv(U)
																		
									C2F(dgemm)("n","n",&N,&N,&N,&alpha,INVU,&N,INVL,&N,&beta,toto,&N);

									C2F(dgemm)("n","n",&N,&N,&N,&alpha,toto,&N,E,&N,&beta,res,&N);

									l = IPNTR[1]-1;
									for (i=0; i<N; i++)
									{
										k = IPNTR[1]-1;
										sum = 0;
										for (j=i; j<N*N; j=j+N)
										{
											sum += res[j] * WORKD[k];
											k++;
										}
										WORKD[l] = sum;
										l++;
									}
								}
								else
								{
									if (IDO == 1)
									{
										// computes workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[2]-1:ipntr[2]+N-1]
										// inv L -> L obtained with LU decomposition
										memset(toto, 0, N*N*sizeof(double));
										memset(res, 0, N*N*sizeof(double));
										memcpy(INVL, L, N*N*sizeof(double));

										memset(IPVT, 0, N*sizeof(int));
										C2F(dgetrf)(&N, &N, INVL, &N, IPVT, &INFO_LU); // LU decomposition
										
										memset(work, 0, N*N*sizeof(double));
										C2F(dgetri)(&N, INVL, &N, IPVT, work, &N, &INFO_LU);  // inv(L)
										
										// inv U -> U obtained with LU decomposition
										memcpy(INVU, U, N*N*sizeof(double));

										memset(IPVT, 0, N*sizeof(int));
										C2F(dgetrf)(&N, &N, INVU, &N, IPVT, &INFO_LU); // LU decomposition
										
										memset(work, 0, N*N*sizeof(double));
										C2F(dgetri)(&N, INVU, &N, IPVT, work, &N, &INFO_LU); // inv(U)
										
										C2F(dgemm)("n","n",&N,&N,&N,&alpha,INVU,&N,INVL,&N,&beta,toto,&N);

										C2F(dgemm)("n","n",&N,&N,&N,&alpha,toto,&N,E,&N,&beta,res,&N);

										l = IPNTR[1]-1;
										for (i=0; i<N; i++)
										{
											k = IPNTR[2]-1;
											sum = 0;
											for (j=i; j<N*N; j=j+N)
											{
												sum += res[j] * WORKD[k];
												k++;
											}
											WORKD[l] = sum;
											l++;
										}
									}
								}
							}
						}
					}
					else
					{
						return -7;
					}
				}
			}
		} // END WHILE

		free(INVR);
		free(INVRPRIME);
		free(work);
		free(INVL);
		free(INVU);
		free(INVE);
		free(L);
		free(U);
		free(E);

		if (eigenvector!=NULL)
		{
			RVEC = 1;	// compute eigenvalues and eigenvectors
		}
					
		if (Asym)  // DSEUPD
		{
			SELECT = (int*) malloc(ncv*sizeof(int));
			memset(SELECT, 0, ncv*sizeof(int));

			D = (double*) malloc(nev*sizeof(double));
			memset(D, 0, nev*sizeof(double));
	
			Z = (double*) malloc(N*nev*sizeof(double));
			memset(Z, 0, N*nev*sizeof(double));

			C2F(dseupd)(&RVEC, HOWMNY, SELECT, D, Z, &LDV, &SIGMAR, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO_EUPD[0], 1L, 1L, 1L, 2L);   

			if (INFO_EUPD[0]!=0)
			{
				return -8;
			}
			else
			{
				if (strcmp(which,"BE") == 0)
				{
					for (i=0; i<nev; i++)
					{
						eigenvalue[i].r = D[i];
					}
				}

				j = 0;
				for (i=nev-1; i>=0; i--)
				{
					eigenvalue[j].r = D[i];
					j++;
				}

				if (RVEC)  // if RVEC = 1 then return eigenvalues and eigenvectors
				{

					// Return matrix for eigenvector
					for (i=0;i<N*nev;i++)
					{
						eigenvector[i].r = Z[i];
					}

					k = 0;
					for (j=0;j<N; j++)
					{
						for (i = nev-1; i>=0; i--)
						{
							eigenvector[j+N*k].r = Z[i+(N-1)*i+j];
							k++;
						}
						k = 0;
					}

					if (IPARAM[6] == 1 && matB!=0)
					{
						for (i= 0;i<N*nev;i++)
						{
							toto[i] = eigenvector[i].r;
						}

						C2F(dgemm)("n","n",&N,&N,&N,&alpha,INVRPRIME,&N,toto,&N,&beta,res,&N);
						for (i=0;i<N*nev;i++)
						{
							eigenvector[i].r = res[i];
						}
					}
				}				
			}
			free(SELECT);
			free(D);
			free(Z);
		}
		else	// DNEUPD
		{
			SELECT = (int*) malloc(ncv*sizeof(int));
			memset(SELECT,0,ncv*sizeof(int));

			DR = (double*) malloc((nev)*sizeof(double));
			memset(DR,0,(ncv)*sizeof(double));

			DI = (double*) malloc((nev)*sizeof(double));
			memset(DI,0,(ncv)*sizeof(double));
	
			Z = (double*) malloc(N*(ncv)*sizeof(double));
			memset(Z,0,N*(ncv)*sizeof(double));

			WORKEV = (double*) malloc(3*N*sizeof(double));
			memset(WORKEV,0,3*N*sizeof(double));

			C2F(dneupd)(&RVEC, HOWMNY, SELECT, DR, DI, V, &LDV, &SIGMAR, &SIGMAI, WORKEV, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO_EUPD[0]);   

			if (INFO_EUPD[0]!=0)
			{
				return -8;
			}
			else
			{
				j = 0;
				if (!RVEC) // if RVEC = 0, then compute eigenvalues
				{
					k = 0;
					for (i=0; i<nev; i++)
					{
						if (DR[i] != 0 || DI[i] != 0)
						{
							eigenvalue[i-k].r = DR[i];
							eigenvalue[i-k].i = DI[i];
						}
						else
						{
							k++;
						}
					}
				}

				j = 0;
				for (i=nev-1; i>=0; i--)
				{
					eigenvalue[j].r = DR[i];
					eigenvalue[j].i = DI[i];
					j++;
				}

				if (RVEC)
				{
					sciprint("Z  = \n");
					for (i = 0; i<N; i++)
					{
						for (j =0; j<ncv; j++)
						{
							sciprint("%f ", Z[i+j*N]);
						}
						sciprint("\n");
					}
					sciprint("\n");

					sciprint("V  = \n");
					for (i = 0; i<N; i++)
					{
						for (j =0; j<ncv; j++)
						{
							sciprint("%f ", V[i+j*N]);
						}
						sciprint("\n");
					}
					sciprint("\n");

					for (i=0; i<nev; i++)
					{
						eigenvalue[i].r = DR[i];
						eigenvalue[i].i = DI[i];
					}
					
					for (i=0; i<nev*N; i++)
					{
						eigenvector[i].r = V[i];
					}

					for (i = 0; i<N; i++)
					{
						for (j =0; j<nev; j++)
						{
							sciprint("%f ", eigenvector[i+j*N].r);
						}
						sciprint("\n");
					}
					sciprint("\n");

					i = 0;
					while(i<=(nev-2))
					{
							for (j=0; j<N; j++)
							{
								eigenvector[i*N+j].r = V[i*N+j];
								eigenvector[i*N+j].i = V[(i+1)*N+j];
								eigenvector[(i+1)*N+j].r = V[i*N+j];
								eigenvector[(i+1)*N+j].i = -V[(i+1)*N+j];
							}
							i = i+2;
					}

					for (i = 0; i<N; i++)
					{
						for (j =0; j<nev; j++)
						{
							sciprint("%f  %fi", eigenvector[i+j*N].r, eigenvector[i+j*N].i);
						}
						sciprint("\n");
					}
					sciprint("\n");
				}

			}

			free(SELECT);
			free(DR);
			free(DI);
			free(Z);
		}
		free(V);
	}
	else // A or/and B complex
	{
		if(IPARAM[6] == 3)	// mode = 3
		{
			AMSBC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
			memcpy(AMSBC, AC, N*N*sizeof(doublecomplex));
			if (matB == 0)	// standart eigenvalue problem
			{
				for (i=0; i<N; i++)
				{
					AMSBC[i+i*N].r = AC[i+i*N].r - SIGMAR;
					AMSBC[i+i*N].i = AC[i+i*N].i - SIGMAI;
				}
			}
			else	// generalized eigenvalue problem
			{
				if (cholB[0])
				{
					memcpy(AMSBC, AC, N*N*sizeof(doublecomplex));
					memset(totoc, 0, N*N*sizeof(doublecomplex));
					memset(resc, 0, N*N*sizeof(doublecomplex));
					C2F(zgemm)("n","n",&N,&N,&N,&alpha,RC,&N,RCprime,&N,&beta,resc,&N);

					for (i=0; i<N*N; i++)
					{
						AMSBC[i].r = AC[i].r - (SIGMAR * resc[i].r + SIGMAI * resc[i].i);
						AMSBC[i].i = AC[i].i - (SIGMAR * resc[i].i + SIGMAI * resc[i].r);
					}

				}
				else
				{
					for (i=0; i<N*N; i++)
					{
						AMSBC[i].r = AC[i].r - (SIGMA[0].r * BC[i].r);
						AMSBC[i].i = AC[i].i - (SIGMA[0].i * BC[i].i);
					}
				}
			}

			// LU decomposition
			C2F(zgetrf)(&N, &N, AMSBC, &N, IPVT, &INFO_LU);

			// Computes the lower triangular matrix L
			LC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
			memset(LC, 0, N*N*sizeof(doublecomplex));
			for (i=0; i<N; i++)
			{
				for (j=0; j<N; j++)
				{
					if (i == j)
					{
						LC[i+j*N].r = 1;
						LC[i+j*N].i = 0;
					}
					else
					{
						if (i>j)
						{
							LC[i+j*N].r = AMSBC[i+j*N].r;
							LC[i+j*N].i = AMSBC[i+j*N].i;
						}
					}
				}
			}

			// Computes the upper triangular matrix U
			
			UC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
			memset(UC, 0, N*N*sizeof(doublecomplex));
			for (i=0; i<N; i++)
			{
				for(j=0; j<N; j++)
				{
					if (i <= j)
					{
						UC[i+j*N].r = AMSBC[i+j*N].r;
						UC[i+j*N].i = AMSBC[i+j*N].i;
					}
				}
			}	

			// Computes the permutation matrix E
			E = (double*) malloc(N*N*sizeof(double));
			memset(E, 0, N*N*sizeof(double));
			for (i=0; i<N; i++)
			{
				E[i*N+i] = 1;
			}

			C2F(dlaswp)(&N, E, &N, &k1, &N, IPVT, &k1);
		
			free(AMSBC);
		}

		LWORKL = 3*ncv*ncv+5*ncv;

		VC = (doublecomplex*) malloc(N*ncv*sizeof(doublecomplex));
		memset(VC,0,N*ncv*sizeof(doublecomplex));

		WORKLC = (doublecomplex*) malloc(LWORKL*sizeof(doublecomplex));
		memset(WORKLC,0,LWORKL*sizeof(doublecomplex));

		WORKDC = (doublecomplex*) malloc(3*N*sizeof(doublecomplex));
		memset(WORKDC,0,3*N*sizeof(doublecomplex));

		RWORK = (double*) malloc(ncv*sizeof(double));
		memset(RWORK,0,ncv*sizeof(double));

		INVRC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
		INVRCPRIME = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
		INVLC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
		INVUC = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
		INVE = (double*) malloc(N*N*sizeof(double));
	
		workc = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
		work = (double*) malloc(N*N*sizeof(double));

		while(IDO != 99)
		{
			C2F(znaupd)(&IDO, bmat, &N, which, &nev, tol, RESIDC, &ncv, VC, &LDV, IPARAM, IPNTR, WORKDC, WORKLC, &LWORKL, RWORK, &INFO[0]);

			if (INFO[0] < 0)
			{
				return -6;
			}

			if (IDO == -1 || IDO == 1 || IDO == 2)
			{
				if (IPARAM[6] == 1)  // mode = 1
					{
						if (matB == 0) // B = I
						{
							sciprint("coucou \n");
							// OP = A*x
							l = IPNTR[1]-1;
							for (i=0; i<N; i++)
							{
								k = IPNTR[0]-1;
								sumc.r = 0;
								sumc.i = 0;
								for (j=i; j<N*N; j=j+N)
								{
									sumc.r += AC[j].r * WORKDC[k].r - AC[j].i * WORKDC[k].i;
									sumc.i += AC[j].r * WORKDC[k].i + AC[j].i * WORKDC[k].r;
									k++;
								}
								WORKDC[l].r = sumc.r;
								WORKDC[l].i = sumc.i;
								l++;
							}
						}
						else
						{
							// OP = inv(R')*A*inv(R)*x
							// inv(R)
							memset(totoc, 0, N*N*sizeof(doublecomplex));
							memset(resc, 0, N*N*sizeof(doublecomplex));
							memcpy(INVRC, RC, N*N*sizeof(doublecomplex));   // copy R to invR

							memset(IPVT, 0, N*sizeof(int));
							C2F(zgetrf)(&N, &N, INVRC, &N, IPVT, &INFO_LU);	// LU decomposition
							
							memset(workc, 0, N*N*sizeof(doublecomplex));
							C2F(zgetri)(&N, INVRC, &N, IPVT, workc, &N, &INFO_LU);  // Inverse matrix R
							
							// inv(R')
							memcpy(INVRCPRIME, RCprime, N*N*sizeof(doublecomplex));
														
							memset(IPVT, 0, N*sizeof(int));
							C2F(zgetrf)(&N, &N, INVRCPRIME, &N, IPVT, &INFO_LU);	// LU decomposition
							
							memset(workc, 0, N*N*sizeof(doublecomplex));
							C2F(zgetri)(&N, INVRCPRIME, &N, IPVT, workc, &N, &INFO_LU);	// Inverse matrix R'
							C2F(zgemm)("n","n",&N,&N,&N,&alpha,INVRCPRIME,&N,AC,&N,&beta,totoc,&N);
							C2F(zgemm)("n","n",&N,&N,&N,&alpha,totoc,&N,INVRC,&N,&beta,resc,&N);
							
							// OP = inv(R')*A*inv(R)*x
							l = IPNTR[1]-1;
							for (i=0; i<N; i++)
							{
								k = IPNTR[0]-1;
								sumc.r = 0;
								sumc.i = 0;
								for (j=i; j<N*N; j=j+N)
								{
									sumc.r += resc[j].r * WORKDC[k].r - resc[j].i * WORKDC[k].i;
									sumc.i += resc[j].i * WORKDC[k].r + resc[j].r * WORKDC[k].i;
									k++;
								}
								WORKDC[l].r = sumc.r;
								WORKDC[l].i = sumc.i;
								l++;
							}
						}
					}
					else
					{
						if (IPARAM[6] == 3)  // si mode = 3
						{
							if (matB == 0)	// B = [] -> matB is empty -> standart eigenvalue problem
							{
								if (IDO == 2)
								{
									// y = B*x where B = I so workd[ipntr[1]-1:ipntr[1]+N-1] = workd[ipntr[0]-1:ipntr[0]+N-1]
									for (i=0; i<N; i++)
									{
										WORKDC[IPNTR[1]-1+i] = WORKDC[IPNTR[0]-1+i];
									}
								}
								else
								{
									// workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[0]-1:ipntr[0]+N-1]
									// inv LC -> LC obtained with LU decomposition
									memset(totoc, 0, N*N*sizeof(doublecomplex));
									memset(resc, 0, N*N*sizeof(doublecomplex));
									memset(EC,0,N*N*sizeof(doublecomplex));
									memcpy(INVLC, LC, N*N*sizeof(doublecomplex));

									memset(IPVT, 0, N*sizeof(int));
									C2F(zgetrf)(&N, &N, INVLC, &N, IPVT, &INFO_LU); // LU decomposition
									
									memset(workc, 0, N*N*sizeof(doublecomplex));
									C2F(zgetri)(&N, INVLC, &N, IPVT, workc, &N, &INFO_LU);  // inv(LC)
									
									// inv UC -> UC obtained with LU decomposition
									memcpy(INVUC, UC, N*N*sizeof(doublecomplex));

									memset(IPVT, 0, N*sizeof(int));
									C2F(zgetrf)(&N, &N, INVUC, &N, IPVT, &INFO_LU); // LU decomposition
									
									memset(workc, 0, N*N*sizeof(doublecomplex));
									C2F(zgetri)(&N, INVUC, &N, IPVT, workc, &N, &INFO_LU); // inv(UC)

									for (i=0; i<N*N; i++)
									{
										EC[i].r = E[i];
									}

									C2F(zgemm)("n","n",&N,&N,&N,&alpha,INVUC,&N,INVLC,&N,&beta,totoc,&N);
									C2F(zgemm)("n","n",&N,&N,&N,&alpha,totoc,&N,EC,&N,&beta,resc,&N);
									
									l = IPNTR[1]-1;
									for (i=0; i<N; i++)
									{
										k = IPNTR[0]-1;
										sumc.r = 0;
										sumc.i = 0;
										for (j=i; j<N*N; j=j+N)
										{
											sumc.r += resc[j].r * WORKDC[k].r - resc[j].i * WORKDC[k].i;
											sumc.i += resc[j].r * WORKDC[k].i + resc[j].i * WORKDC[k].r;
											k++;
										}
										WORKDC[l].r = sumc.r;
										WORKDC[l].i = sumc.i;
										l++;
									}
								}

							}
							else  // matB == 1 so B is not empty and bmat = 'G'-> generalized eigenvalue problem
							{
								if (IDO == 2)
								{
									sciprint("tutu \n");
									if (cholB[0])  // workd[ipntr[1]-1:ipntr[1]+N-1] = RC * RCprime * workd[ipntr[0]-1:ipntr[0]+N-1]
									{
										memset(resc, 0, N*N*sizeof(doublecomplex));
										C2F(zgemm)("n","n",&N,&N,&N,&alpha,RC,&N,RCprime,&N,&beta,resc,&N);

										l = IPNTR[1]-1;
										for (i=0; i<N; i++)
										{
											k = IPNTR[0]-1;
											sumc.r = 0;
											sumc.i = 0;
											for (j=i; j<N*N; j=j+N)
											{
												sumc.r += resc[j].r * WORKDC[k].r - resc[j].i * WORKDC[k].i;
												sumc.i += resc[j].i * WORKDC[k].r + resc[j].r * WORKDC[k].i;
												k++;
											}
											WORKDC[l].r = sumc.r;
											WORKDC[l].i = sumc.i;
											l++;
										}
									}
									else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B *workd[ipntr[0]-1:ipntr[0]+N-1]
									{
										l = IPNTR[1]-1;
										for (i=0; i<N; i++)
										{
											k = IPNTR[0]-1;
											sumc.r = 0;
											sumc.i = 0;
											for (j=i; j<N*N; j=j+N)
											{
												sumc.r += BC[j].r * WORKDC[k].r - BC[j].i * WORKDC[k].i;
												sumc.i += BC[j].r * WORKDC[k].i + BC[j].i * WORKDC[k].r;
												k++;
											}
											WORKDC[l].r = sumc.r;
											WORKDC[l].i = sumc.i;
											l++;
										}
									}
								}
								else
								{
									if (IDO == -1)
									{
										if (cholB[0])   // workd[ipntr[1]-1:ipntr[1]+N-1] = RC*RCprime*workd[ipntr[0]-1:ipntr[0]+N-1]
										{
											memset(resc, 0, N*N*sizeof(doublecomplex));
											C2F(zgemm)("n","n",&N,&N,&N,&alpha,RC,&N,RCprime,&N,&beta,resc,&N);
											l = IPNTR[1]-1;
											for (i=0; i<N; i++)
											{
												k = IPNTR[0]-1;
												sumc.r = 0;
												sumc.i = 0;
												for (j=i; j<N*N; j=j+N)
												{
													sumc.r += resc[j].r * WORKDC[k].r - resc[j].i * WORKDC[k].i;
													sumc.i += resc[j].i * WORKDC[k].r + resc[j].r * WORKDC[k].i;
													k++;
												}
												WORKDC[l].r = sumc.r;
												WORKDC[l].i = sumc.i;
												l++;
											}
										}
										else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
										{
											l = IPNTR[1]-1;
											for (i=0; i<N; i++)
											{
												k = IPNTR[0]-1;
												sumc.r = 0;
												sumc.i = 0;
												for (j=i; j<N*N; j=j+N)
												{
													sumc.r += BC[j].r * WORKDC[k].r - BC[j].i * WORKDC[k].i;
													sumc.i += BC[j].r * WORKDC[k].i + BC[j].i * WORKDC[k].r;
													k++;
												}
												WORKDC[l].r = sumc.r;
												WORKDC[l].i = sumc.i;
												l++;
											}
										}

										// compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[1]-1:ipntr[1]+N-1]

										// inv LC -> LC obtained with LU decomposition
										memcpy(INVLC, LC, N*N*sizeof(doublecomplex));

										memset(IPVT, 0, N*sizeof(int));
										C2F(zgetrf)(&N, &N, INVLC, &N, IPVT, &INFO_LU); // LU decomposition
										
										memset(workc, 0, N*N*sizeof(doublecomplex));
										C2F(zgetri)(&N, INVLC, &N, IPVT, workc, &N, &INFO_LU);  // inv(LC)
										
										// inv UC -> UC obtained with LU decomposition
										memcpy(INVUC, UC, N*N*sizeof(doublecomplex));

										memset(IPVT, 0, N*sizeof(int));
										C2F(zgetrf)(&N, &N, INVUC, &N, IPVT, &INFO_LU); // LU decomposition
										
										memset(workc, 0, N*N*sizeof(doublecomplex));
										C2F(zgetri)(&N, INVUC, &N, IPVT, workc, &N, &INFO_LU); // inv(UC)
										memset(resc, 0, N*N*sizeof(doublecomplex));
										memset(totoc, 0, N*N*sizeof(doublecomplex));
										memset(EC, 0, N*N*sizeof(doublecomplex));
										for (i=0; i<N*N; i++)
										{
											EC[i].r = E[i];
										}
										C2F(zgemm)("n","n",&N,&N,&N,&alpha,INVUC,&N,INVLC,&N,&beta,totoc,&N);
										C2F(zgemm)("n","n",&N,&N,&N,&alpha,totoc,&N,EC,&N,&beta,resc,&N);
										
										l = IPNTR[1]-1;
										for (i=0; i<N; i++)
										{
											k = IPNTR[1]-1;
											sumc.r = 0;
											sumc.i = 0;
											for (j=i; j<N*N; j=j+N)
											{
												sumc.r += resc[j].r * WORKDC[k].r - resc[j].i * WORKDC[k].i;
												sumc.i += resc[j].i * WORKDC[k].r + resc[j].r * WORKDC[k].i;
												k++;
											}
											WORKDC[l].r = sumc.r;
											WORKDC[l].i = sumc.i;
											l++;
										}
									}
									else
									{
										if (IDO == 1)
										{
											// compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[2]-1:ipntr[2]+N-1]
											// inv LC-> LC obtained with LU decomposition
											memcpy(INVLC, LC, N*N*sizeof(doublecomplex));

											memset(IPVT, 0, N*sizeof(int));
											C2F(zgetrf)(&N, &N, INVLC, &N, IPVT, &INFO_LU); // LU decomposition
											
											memset(workc, 0, N*N*sizeof(doublecomplex));
											C2F(zgetri)(&N, INVLC, &N, IPVT, workc, &N, &INFO_LU);  // inv(LC)
											
											// inv UC -> UC obtained with LU decomposition
											memcpy(INVUC, UC, N*N*sizeof(doublecomplex));

											memset(IPVT, 0, N*sizeof(int));
											C2F(zgetrf)(&N, &N, INVUC, &N, IPVT, &INFO_LU); // LU decomposition
											
											memset(workc, 0, N*N*sizeof(doublecomplex));
											C2F(zgetri)(&N, INVUC, &N, IPVT, workc, &N, &INFO_LU); // inv(UC)

											memset(resc, 0, N*N*sizeof(doublecomplex));
											memset(totoc, 0, N*N*sizeof(doublecomplex));
											memset(EC, 0, N*N*sizeof(doublecomplex));
											for (i=0; i<N*N; i++)
											{
												EC[i].r = E[i];
											}
											C2F(zgemm)("n","n",&N,&N,&N,&alpha,INVUC,&N,INVLC,&N,&beta,totoc,&N);
											C2F(zgemm)("n","n",&N,&N,&N,&alpha,totoc,&N,EC,&N,&beta,resc,&N);
											
											l = IPNTR[1]-1;
											for (i=0; i<N; i++)
											{
												k = IPNTR[2]-1;
												sumc.r = 0;
												sumc.i = 0;
												for (j=i; j<N*N; j=j+N)
												{
													sumc.r += resc[j].r * WORKDC[k].r - resc[j].i * WORKDC[k].i;
													sumc.i += resc[j].i * WORKDC[k].r + resc[j].r * WORKDC[k].i;
													k++;
												}
												WORKDC[l].r = sumc.r;
												WORKDC[l].i = sumc.i;
												l++;
											}
										}
									}
								}
							}
						}
						else
						{
							return -7;
						}
					}
				}
			} // END WHILE
			free(INVRC);
			free(INVRCPRIME);
			free(LC);
			free(UC);
			free(INVLC);
			free(INVUC);
			free(INVE);
			free(work);
			free(workc);

			SELECT = (int*) malloc(ncv*sizeof(int));
			memset(SELECT,0,ncv*sizeof(int));

			DC = (doublecomplex*) malloc((nev)*sizeof(doublecomplex));
			memset(DC,0,(nev+1)*sizeof(doublecomplex));

			ZC = (doublecomplex*) malloc(N*nev*sizeof(doublecomplex));
			memset(ZC,0,N*nev*sizeof(doublecomplex));

			WORKEVC = (doublecomplex*) malloc(2*N*sizeof(doublecomplex));
			memset(WORKEVC,0,2*N*sizeof(doublecomplex));

			if (eigenvector!=NULL)
			{
				RVEC = 1;	// compute eigenvalues and eigenvectors
			}
			C2F(zneupd)(&RVEC, HOWMNY, SELECT, DC, ZC, &LDV, SIGMA, WORKEVC, bmat, &N, which, &nev, tol, RESIDC, &ncv, VC, &LDV, IPARAM, IPNTR, WORKDC, WORKLC, &LWORKL, RWORK, &INFO_EUPD[0]);   

			if (INFO_EUPD[0]!=0)
			{
				return -8;
			}
			else
			{
				if (!RVEC)
				{
					j = 0;
					for (i=nev-1; i>=0; i--)
					{
						eigenvalue[j].r = DC[i].r;
						eigenvalue[j].i = DC[i].i;
						j++;
					}
				}
				else  // return eigenvalues and eigenvectors
				{
					memcpy(eigenvalue, DC, nev*sizeof(doublecomplex));
					memcpy(eigenvector, ZC, N*nev*sizeof(doublecomplex));
				}	
			}
				
			free(SELECT);
			free(DC);
			free(ZC);
			free(WORKEVC);

			free(VC);
			free(WORKDC);
			free(WORKLC);
			free(RWORK);
		}
	
	free(IPNTR);	
	free(IPVT); 
	free(IPARAM);

	free(TRIB);
	free(TRIBC);
	free(R);
	free(Rprime);
	free(RC);
	free(RCprime);
	
	return 0;
}