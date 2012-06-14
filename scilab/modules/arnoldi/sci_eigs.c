/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include <string.h>
#include "stack-c.h"
#include "core_math.h"
#include "gw_arnoldi.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "stdio.h"
#include "stdlib.h"
#include "sciprint.h"
#include "doublecomplex.h"
#include "eigs.h"

int sci_eigs(char *fname,unsigned long fname_len)
{
	int *piAddressVarOne	= NULL;
	int TypeVarOne			= 0;
	int RowsOne = 0, ColsOne = 0;
	double elemt1 = 0, elemt2 = 0;
	double* Areal			  = NULL;	
	doublecomplex* Acplx	  = NULL;
	int Asym	= 1;
	int Acomplex	= 0;
	int N			= 0;

	int *piAddressVarTwo	= NULL;
	int TypeVarTwo			= 0;
	int RowsTwo = 0, ColsTwo = 0;
	double* Breal			 = NULL;
	doublecomplex* Bcplx	 = NULL;
	int Bcomplex			 = 0;
	int matB				 = 0;

	int *piAddressVarThree	 = NULL;
	int TypeVarThree		 = 0;
	int RowsThree = 0, ColsThree = 0;
	double* NEV				 = NULL;

	int *piAddressVarFour	 = NULL;
	int TypeVarFour		 = 0;
	int RowsFour = 0, ColsFour = 0;
	int iCurLen				= 0;
	int* piLen				= NULL;
	char** pstData			= NULL;
	char* pstOut			=  "LM";
	doublecomplex* SIGMA	= (doublecomplex*) malloc(1 * sizeof(doublecomplex));

	int *piAddressVarFive	 = NULL;
	int TypeVarFive			 = 0;
	int RowsFive = 0, ColsFive = 0;
	double* MAXITER			 = NULL;

	int *piAddressVarSix	 = NULL;
	int TypeVarSix			 = 0;
	int RowsSix = 0, ColsSix = 0;
	double* TOL				 = NULL;

	int *piAddressVarSeven	 = NULL;
	int TypeVarSeven			 = 0;
	int RowsSeven = 0, ColsSeven = 0;
	double* NCV				 = NULL;

	int *piAddressVarEight	 = NULL;
	int TypeVarEight			 = 0;
	int RowsEight = 0, ColsEight = 0;
	double* cholB				 = NULL;

	int *piAddressVarNine	 = NULL;
	int TypeVarNine			 = 0;
	int RowsNine = 0, ColsNine = 0;
	double* RESID			 = NULL;
	doublecomplex* RESIDC	 = NULL;

	int *piAddressVarTen	 = NULL;
	int RowsTen = 0, ColsTen = 0;
	int *INFO				 = NULL;	

	// Output arguments
	doublecomplex* eigenvalue	= NULL;
	doublecomplex* mat_eigenvalue = NULL;
	doublecomplex* eigenvector  = NULL;
	int* INFO_EUPD				= NULL;
	int error	= 0;

	SciErr sciErr;
	int i = 0, j = 0;

	CheckRhs(1,10);
	CheckLhs(0,2);

	/****************************************
	*    	First variable : A    		*
	*****************************************/

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
		return 0;
	}

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &RowsOne, &ColsOne);

	//check if A is a square matrix
	if(RowsOne*ColsOne==1 || RowsOne!=ColsOne)
	{
		Scierror(999, _("%s: Wrong dimension for input argument #%d: A square matrix expected.\n"), fname, 1);
		return 0;
	}
	
	N = RowsOne;

	//check complexity
	if(isVarComplex(pvApiCtx, piAddressVarOne))
	{
		sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarOne, &RowsOne, &ColsOne, &Acplx);
		Acomplex = 1;
			/*for(i=0; i<(RowsOne*ColsOne); i++){
				sciprint("get a complex matrix of double : %d, %d, %f, %f \n",RowsOne, ColsOne, Acplx[i].r, Acplx[i].i);
			}*/

			/*for (i=0;i<iRows;i++){
				for (j=0;j<iCols;j++){
					if(Acplx[i+j*iCols].r == 0 && Acplx[i+j*iCols].i == 0){
						sciprint("%f	", 0);
					}
					else{
						if(Acplx[i+j*iCols].r != 0 && Acplx[i+j*iCols].i == 0){
							sciprint("%f	", Acplx[i+j*iCols].r);
						}
						else{
							if (Acplx[i+j*iCols].r == 0 && Acplx[i+j*iCols].i != 0){
								sciprint("%fi	", Acplx[i+j*iCols].i);
							}
							else{
								if (Acplx[i+j*iCols].r !=0 && Acplx[i+j*iCols].i > 0){
									sciprint("%f + %fi	", Acplx[i+j*iCols].r, Acplx[i+j*iCols].i);
								}
								else{
									sciprint("%f - %fi	", Acplx[i+j*iCols].r, fabs(Acplx[i+j*iCols].i));
								}
							}
						}
					}
				}
				sciprint("\n");
			}*/
		/*if(TypeVarOne == sci_sparse)
		{
			sciErr = getComplexSparseMatrix(pvApiCtx, piAddressVarOne, &RowsOne, &ColsOne, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);*/
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &RowsOne, &ColsOne, &Areal);
		/*for(i=0; i<(RowsOne*ColsOne); i++){
				sciprint("get a  matrix of double : %d, %d, %f \n", RowsOne, ColsOne, Areal[i]);
		}*/

		for(i=0; i<ColsOne; i++)
		{
			sciprint("i :%d \n",i);
			for(j=0; j<i; j++)
			{
				sciprint("j :%d \n",j);
				elemt1 = Areal[j+i*ColsOne];
				elemt2 = Areal[j*ColsOne+i];
				if(fabs(elemt1-elemt2)>0)
				{
					Asym = 0;
					sciprint("A is a real non symmetric matrix. \n");
					break;
				}					
			}
			if (Asym==0)
			{
				break;
			}
			sciprint("yop2 \n");
		}
		
		// Check symmetry
		if(Asym)
		{
			sciprint("A is a real symmetric matrix. \n");
		}

		/*for (i=0;i<iRows;i++){
			for (j=0;j<iCols;j++){
				sciprint("%f ", Areal[j+i*iCols]);
			}
			sciprint("\n");
		}*/
	}

	/****************************************
	*    	Second variable : B    		*
	*****************************************/
	/*if(Rhs>=2)
	{*/
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarTwo, &TypeVarTwo);
	if(sciErr.iErr || TypeVarTwo != sci_matrix)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Wrong type for input argument #%d: A B matrix expected\n"), fname, 2);
		return 0;
	}

	/*if(isVarMatrixType(pvApiCtx, piAddressVarTwo))
	{*/
		sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &RowsTwo, &ColsTwo);
		if(RowsTwo*ColsTwo==1 || RowsTwo!=ColsTwo)
		{
			Scierror(999, _("%s: Wrong dimension for input argument #%d: B must be a same size as A. \n"), fname, 2);
			return 0;
		}
	//}
		
	matB = RowsTwo * ColsTwo;
	if(isVarComplex(pvApiCtx, piAddressVarTwo))
	{
		sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarTwo, &RowsTwo, &ColsTwo, &Bcplx);
		Bcomplex = 1;
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &RowsTwo, &ColsTwo, &Breal);
	}
	sciprint(" MATB : %d \n",matB);

	if (matB != 0)
	{
		// If B is real but A complex
		if (Acomplex  && !Bcomplex)
		{
			Bcplx = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
			Bcomplex = 1;
			for (i=0; i<N*N; i++)
			{
				Bcplx[i].r = Breal[i];
				Bcplx[i].i = 0;
			}
		}
		// If A is real but B complex
		if (!Acomplex && Bcomplex)
		{
			Acplx = (doublecomplex*) malloc(N*N*sizeof(doublecomplex));
			Acomplex = 1;
			for (i=0; i<N*N; i++)
			{
				Acplx[i].r = Areal[i];
				Acplx[i].i = 0;
			}
		}
	}

	//}

	/****************************************
	*    			NEV   			*
	*****************************************/
	//if (Rhs>3)
	//{
	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarThree, &TypeVarThree);
	if(sciErr.iErr || TypeVarThree != sci_matrix)
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected. \n"), fname, 3);
		return 0;
	}
	else
	{	
		if(isVarComplex(pvApiCtx, piAddressVarThree))
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected. \n"), fname, 3);
			return 0;
		}
		else
		{
			sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarThree, &RowsThree, &ColsThree, &NEV);
			if(RowsThree*ColsThree!=1)
			{
				Scierror(999, _("%s: Wrong dimension for input argument #%d: NEV must be 1 by 1 size. \n"), fname, 3);
				return 0;
			}
			if(NEV[0] != floor(NEV[0]))
			{
				Scierror(999, _("%s: Wrong type for input argument #%d: NEV must be an integer scalar. \n"), fname, 3);
				return 0;
			}

			/*for (i=0;i<(RowsThree*ColsThree);i++)
			{
				sciprint("nev : %d, %d, %f \n", RowsThree, ColsThree, NEV[i]);
			}*/
		}
	}
	//}

	/****************************************
	*    		SIGMA AND WHICH    			*
	*****************************************/
	//if(Rhs>=4)
	//{
	sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarFour, &TypeVarFour);
	if(sciErr.iErr || (TypeVarFour != sci_matrix && TypeVarFour !=sci_strings))
	{
		Scierror(999, _("%s: Wrong type for input argument #%d. \n"), fname, 4);
		return 0;
	}
	
	if (TypeVarFour == sci_strings)
	{
		sciErr = getVarDimension(pvApiCtx, piAddressVarFour, &RowsFour, &ColsFour);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
			return 0;
		}

		if (RowsFour * ColsFour != 1)
		{
			Scierror(999, _("%s: Wrong dimension for input argument #%d.\n"), fname, 4);
			return 0;
		}

		piLen = (int*)malloc(sizeof(int) * RowsFour * ColsFour);
		sciErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &RowsFour, &ColsFour, piLen, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
			return 0;
		}

		pstData = (char**)malloc(sizeof(char*) * RowsFour * ColsFour);
		for(i = 0 ; i < RowsFour * ColsFour ; i++)
		{
			pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));
		}
		sciErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &RowsFour, &ColsFour, piLen, pstData);	
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
			return 0;
		}
	
		sciprint("%c \n",pstData[0][0]);
		sciprint("%c \n",pstData[0][1]);


		////computer length of all strings
		//for(i = 0 ; i < RowsFour * ColsFour ; i++)
		//{
		//	iLen += piLen[i];
		//}

		//alloc output variable
		pstOut = (char*)malloc(sizeof(char) * (piLen[0] + RowsFour * ColsFour));
		//initialize string to 0x00
		memset(pstOut, 0x00, sizeof(char) * (piLen[0] + RowsFour * ColsFour));

		//concat input strings in output string
		for(i = 0 ; i < RowsFour ; i++)
		{
			for(j = 0 ; j < ColsFour ; j++)
			{
				iCurLen = (int) strlen(pstOut);
				if(iCurLen)
				{
					strcat(pstOut, " ");
				}
				strcpy(pstOut + strlen(pstOut), pstData[j * RowsFour + i]);
			}
		}

		for (i=0; i<piLen[0]; i++)
		{
			sciprint("get a string matrix : %d, %d, %d, %c \n",RowsFour,ColsFour,piLen[0], pstOut[i]);
		}

		//if(strcmp(pstOut,"LM")==0)
		//	sciprint("OK \n");

		if (strcmp(pstOut,"LM")!=0 && strcmp(pstOut,"SM")!=0  && strcmp(pstOut,"LR")!=0 && strcmp(pstOut,"SR")!=0 && strcmp(pstOut,"LI")!=0 && strcmp(pstOut,"SI")!=0 && strcmp(pstOut,"LA")!=0 && strcmp(pstOut,"SA")!=0 && strcmp(pstOut,"BE")!=0)
		{
			if (Acomplex==0 && Bcomplex==0 && Asym==1)
			{
				Scierror(999, _("%s: Wrong input argument #%d : Unrecognized sigma value.\n Sigma must be one of 'LM', 'SM','LA', 'SA' or 'BE'. \n" ), fname, 4);
				return 0;
			}
			else
			{
				Scierror(999, _("%s: Wrong input argument #%d : Unrecognized sigma value.\n Sigma must be one of 'LM', 'SM','LR', 'SR', 'LI' or 'SI'.\n " ), fname, 4);
				return 0;
			}
			

		}

		if  ((Acomplex==1 || Bcomplex == 1 || Asym==0) && (strcmp(pstOut,"LA")==0 || strcmp(pstOut,"SA")==0 || strcmp(pstOut,"BE")==0))
		{
			Scierror(999, _("%s: Invalid sigma value for complex or non symmetric problem.\n"), fname, 4);
			return 0;
		}

		if (Acomplex==0 && Bcomplex==0 && Asym==1 && (strcmp(pstOut,"LR")==0 || strcmp(pstOut,"SR")==0 || strcmp(pstOut,"LI")==0 || strcmp(pstOut,"SI")==0))
		{
			Scierror(999, _("%s: Invalid sigma value for real symmetric problem.\n"), fname, 4);
			return 0;
		}
		SIGMA[0].r = 0;
		SIGMA[0].i = 0;

		for(i = 0 ; i < RowsFour * ColsFour ; i++)
		{
			free(pstData[i]);
		}
		free(pstData);
	}

	if (TypeVarFour == sci_matrix)
	{
		sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarFour, &RowsFour, &ColsFour, &SIGMA);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
			return 0;
		}

		/*for (i=0; i<(RowsFour*ColsFour); i++)
		{
				sciprint("get a complex matrix of double : %d, %d, %f, %f \n", RowsFour, ColsFour, SIGMA[i].r, SIGMA[i].i);
		}*/

		/*for (i=0;i<iRows;i++){
			for (j=0;j<iCols;j++){
				if(SIGMA[j+i*iCols].r == 0 && SIGMA[j+i*iCols].i == 0){
					sciprint("%f	", 0);
				}
				else{
					if(SIGMA[j+i*iCols].r != 0 && SIGMA[j+i*iCols].i == 0){
						sciprint("%f	", SIGMA[j+i*iCols].r);
					}
					else{
						if (SIGMA[j+i*iCols].r == 0 && SIGMA[j+i*iCols].i != 0){
							sciprint("passe \n");
							sciprint("%fi	", SIGMA[j+i*iCols].i);
						}
						else{
							if (SIGMA[j+i*iCols].r !=0 && SIGMA[j+i*iCols].i > 0){
								sciprint("%f + %fi	", SIGMA[j+i*iCols].r, SIGMA[j+i*iCols].i);
							}
							else{
								sciprint("%f - %fi	", SIGMA[j+i*iCols].r, fabs(SIGMA[j+i*iCols].i));
							}
						}
					}
				}
			}
			sciprint("\n");
		}*/
	}
	//}

	/****************************************
	*    			MAXITER    				*
	*****************************************/
	//if(Rhs>=5)
	//{
	sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarFive);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarFive, &TypeVarFive);
	if(sciErr.iErr || TypeVarFive != sci_matrix)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Wrong type for input argument #%d: A integer value expected.\n"), fname, 5);
		return 0;
	}

	if(isVarComplex(pvApiCtx, piAddressVarFive))
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A integer value expected.\n"), fname, 5);
		return 0;
	}
	else
	{	
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarFive, &RowsFive, &ColsFive, &MAXITER);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
			return 0;
		}
		if (RowsFive*ColsFive!=1)
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A integer value expected.\n"), fname, 5);
			return 0;
		}
		
		//sciprint("%f, %f \n",maxiter[0],floor(maxiter[0]));
		if (MAXITER[0]!=floor(MAXITER[0]))
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A integer value expected.\n"), fname, 5);
			return 0;
			//maxiter[0] = floor(maxiter[0]);
		}
		
		/*for (i=0;i<(RowsFive*ColsFive);i++)
		{
			sciprint("Maxiter : %d, %d, %f \n", RowsFive, ColsFive, MAXITER[i]);
		}*/
	}
	//}

	/****************************************
	*    				TOL	    			*
	*****************************************/
	//if(Rhs>=6)
	//{

	sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddressVarSix);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 6);
		return 0;
	}


	sciErr = getVarType(pvApiCtx, piAddressVarSix, &TypeVarSix);
	if(sciErr.iErr || TypeVarSix != sci_matrix)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Wrong type for input argument #%d: A  real scalar expected.\n"), fname, 6);
		return 0;
	}

	if(isVarComplex(pvApiCtx, piAddressVarSix))
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 6);
		return 0;
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarSix, &RowsSix, &ColsSix, &TOL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 6);
			return 0;
		}

		if (RowsSix*ColsSix!=1)
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 6);
			return 0;
		}
		
		/*for (i=0; i<(RowsSix*ColsSix); i++)
		{
			sciprint("Tol : %d, %d, %f \n", RowsSix, ColsSix, TOL[i]);
		}*/
	}
	//}

	/****************************************
	*    				NCV	    			*
	*****************************************/

	//if(Rhs>=7)
	//{
	sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddressVarSeven);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarSeven, &TypeVarSeven);
	if(sciErr.iErr || TypeVarSeven != sci_matrix)
	{
		sciprint("passe \n");
		printError(&sciErr, 0);
		Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 7);
		return 0;
	}
	else
	{
		if(isVarComplex(pvApiCtx, piAddressVarSeven))
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 7);
			return 0;
		}
		else
		{
			sciErr = getVarDimension(pvApiCtx, piAddressVarSeven, &RowsSeven, &ColsSeven);
			if(RowsSeven*ColsSeven!=1 && RowsSeven*ColsSeven!=0)
			{
				sciprint("nok \n");
				Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 7);
				return 0;
			}

			if (RowsSeven*ColsSeven == 1)
			{
				sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarSeven, &RowsSeven, &ColsSeven, &NCV);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
					return 0;
				}

				if(NCV[0]!=floor(NCV[0]))
				{
					sciprint("olala \n");
					Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 7);
					return 0;
				}
			}
				
			//sciprint("NCV : %d, %d", RowsSeven, ColsSeven);
		}
	}
	//}

	/****************************************
	*    			CHOLB    			*
	*****************************************/

	//if(Rhs >= 8)
	//{
	
	sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddressVarEight);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 8);
		return 0;
	}
	
	sciErr = getVarType(pvApiCtx, piAddressVarEight, &TypeVarEight);
	if(sciErr.iErr || TypeVarEight != sci_matrix)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Wrong type for input argument #%d: A scalar of integer value expected.\n"), fname, 8);
		return 0;
	}
	if(isVarComplex(pvApiCtx, piAddressVarEight))
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A scalar of integer value expected.\n"), fname, 8);
		return 0;
	}

	sciErr = getVarDimension(pvApiCtx, piAddressVarEight, &RowsEight, &ColsEight);
	if(RowsEight*ColsEight!=1)
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A scalar of integer value expected.\n"), fname, 8);
		return 0;
	}
		
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarEight, &RowsEight, &ColsEight, &cholB);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 8);
		return 0;
	}

	if(cholB[0] != floor(cholB[0]))
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: A scalar of integer value expected.\n"), fname, 8);
		return 0;
	}

	if(cholB[0] != 0 && cholB[0] != 1)
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: cholB must be 0 or 1.\n"), fname, 8);
		return 0;
	}

	/*for (i=0; i<(RowsEight*ColsEight); i++)
	{
		sciprint("cholB : %d, %d, %f \n", RowsEight, ColsEight, cholB[i]);
	}*/
			
	//}

	/****************************************
	*    			RESID    			*
	*****************************************/
	//if(Rhs >= 9)
	//{
	sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddressVarNine);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddressVarNine, &TypeVarNine);
	if(sciErr.iErr || TypeVarNine != sci_matrix)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), fname, 9);
		return 0;
	}
	else
	{
		sciErr = getVarDimension(pvApiCtx, piAddressVarNine, &RowsNine, &ColsNine);
		if(RowsNine*ColsNine==1 || RowsNine*ColsNine!=N)
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: Start vector opts.resid must be N by 1.\n"), fname, 9);
			return 0;
		}
	}

	if(!Acomplex && !Bcomplex)
	{
		if(isVarComplex(pvApiCtx, piAddressVarNine))
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: Start vector opts.resid must be real for real problems.\n"), fname, 9);
			return 0;
		}
		else
		{
			sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarNine, &RowsNine, &ColsNine, &RESID);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
				return 0;
			}
		}
	}
	else
	{
		sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarNine, &RowsNine, &ColsNine, &RESIDC);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
			return 0;
		}
	}
	//}

	/****************************************
	*    			INFO    			*
	*****************************************/
	//if(Rhs >= 10)
	//{
	sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddressVarTen);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
		return 0;
	}
	sciErr = getMatrixOfInteger32(pvApiCtx, piAddressVarTen, &RowsTen, &ColsTen, &INFO);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
		return 0;
	}
	//}

	// Initialization output arguments
	eigenvalue = (doublecomplex*) malloc((int) NEV[0] * sizeof(doublecomplex));
	memset(eigenvalue, 0, (int) NEV[0] * sizeof(doublecomplex));

	INFO_EUPD = (int*) malloc(1*sizeof(int));
	memset(INFO_EUPD, 0, 1*sizeof(int));

	if(Lhs>1)
	{
		mat_eigenvalue = (doublecomplex*) malloc((int)NEV[0] * (int)NEV[0] * sizeof(doublecomplex));
		memset(mat_eigenvalue, 0, (int)NEV[0] * (int)NEV[0] * sizeof(doublecomplex));

		eigenvector = (doublecomplex*) malloc(N * (int) NEV[0] * sizeof(doublecomplex));
		memset(eigenvector, 0, N * (int) NEV[0] * sizeof(doublecomplex));
	}	

	error = eigs(Areal, Acplx, N, Acomplex, Asym, Breal, Bcplx, Bcomplex, matB, NEV, SIGMA, pstOut, MAXITER, TOL, NCV, RESID, RESIDC, INFO, cholB, INFO_EUPD, eigenvalue, eigenvector);

	switch (error)
	{
	case -1 :
		sciprint("For real symmetric problems, NCV must be NEV<NCV<=N. \n");
		sciprint("For real non symmetric problems, NCV must be NEV+2<NCV<=N. \n");
		sciprint("For complex problems, NCV must be NEV+1<NCV<=N. \n");
		PutLhsVar();
		return 0;

	case -2 :
		Scierror(999, _("%s : For real symmetric problems, NEV must be NEV<N.\n For real non symmetric or complex problems, NEV must be NEV<N-1.\n  Use the spec function instead. \n "), fname);
		/*Scierror(999, _("%s : For real non symmetric or complex problems, NEV must be NEV<N-1.\n"), fname);
		Scierror(999, _("%s : Use the spec function instead.\n"), fname);*/
		//sciprint("For real symmetric problems, NEV must be NEV<N \n");
		/*sciprint("For real non symmetric or complex problems, NEV must be NEV<N-1 \n");
		sciprint("Use the spec function instead. \n");*/
		/*LhsVar(1) = 0;
		PutLhsVar();*/
		return 0;

	case -3 :
		sciprint("If cholB == 1 then the generalized matrix B must be a Cholesky factor. \n");
		PutLhsVar();
		return 0;

	case -4:
		sciprint("B is a real or complex non symmetric matrix. \n");
		PutLhsVar();
		return 0;

	case -5 : 
		sciprint("The matrix B is not positive definite \n");
		PutLhsVar();
		return 0;

	case -6 :
		if(!Acomplex && !Bcomplex)
		{
			if(Asym)
			{
				sciprint("Error with DSAUPD, info = %d \n",INFO[0]);
			}
			else
			{
				sciprint("Error with DNAUPD, info = %d \n",INFO[0]);
			}
		}
		else
		{
			sciprint("Error with ZNAUPD, info = %d \n",INFO[0]);
		}
		PutLhsVar();
		return 0;

	case -7 :
		if(!Acomplex && !Bcomplex)
		{
			if(Asym)
			{
				sciprint("Error with DSAUPD : unknown mode returned. \n");
			}
			else
			{
				sciprint("Error with DNAUPD : unknown mode returned. \n");
			}
		}
		else
		{
			sciprint("Error with ZNAUPD : unknown mode returned. \n");
		}
		PutLhsVar();
		return 0;

	case -8 :
		if(!Acomplex && !Bcomplex)
		{
			if(Asym)
			{
				sciprint("Error with DSEUPD, info = %d \n", INFO_EUPD[0]);
			}
			else
			{
				sciprint("Error with DNEUPD, info = %d \n", INFO_EUPD[0]);
			}
		}
		else
		{
			sciprint("Error with ZNEUPD, info = %d \n", INFO_EUPD[0]);
		}
		PutLhsVar();
		return 0;
	}

	if(Lhs<=1)
	{
		sciErr = createComplexZMatrixOfDouble(pvApiCtx, Rhs+1, (int) NEV[0], 1, eigenvalue);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999,_("%s: Memory allocation error.\n"), fname);
			return 0;
		}

		LhsVar(1) = Rhs+1;
	}
	else
	{
		// create a matrix which contains the eigenvalues
		for (i=0; i<NEV[0]; i++)
		{
			mat_eigenvalue[i*(int) NEV[0]+i].r = eigenvalue[i].r;
			mat_eigenvalue[i*(int) NEV[0]+i].i = eigenvalue[i].i;
		}

		sciErr = createComplexZMatrixOfDouble(pvApiCtx, Rhs+1, (int) NEV[0], (int) NEV[0], mat_eigenvalue);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999,_("%s: Memory allocation error.\n"), fname);
			return 0;
		}

		sciErr = createComplexZMatrixOfDouble(pvApiCtx, Rhs+2, N, (int) NEV[0], eigenvector);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			Scierror(999,_("%s: Memory allocation error.\n"), fname);
			return 0;
		}

		LhsVar(1) = Rhs+1;
		LhsVar(2) = Rhs+2;
	}

	/*free(piLen);
	free(pstOut);*/

	//LhsVar(1) = 1;
	PutLhsVar();
    return 0;
}
