/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdlib.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "vect_and.h"
#include "api_scilab.h"
#include "api_oldstack.h"
/*--------------------------------------------------------------------------*/
/* SCILAB function : and */
/*--------------------------------------------------------------------------*/
int sci_and(char *fname, int* _piKey)
{
	SciErr sciErr;
	int iRet        = 0;
	int iMode       = 0;

	int iRows       = 0;
	int iCols       = 0;

	int* piAddr1    = NULL;
	int* piAddr2    = NULL;

	int* piBool1    = NULL;
	int* piBool3    = NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(!isBooleanType(_piKey, piAddr1))
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 2)
	{
		sciErr = getProcessMode(_piKey, 2, piAddr1, &iMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	sciErr = getMatrixOfBoolean(_piKey, piAddr1, &iRows, &iCols, &piBool1);
	if(iRet)
	{
		return 1;
	}

	switch(iMode)
	{
	case BY_ALL : 
		piBool3 = (int*)MALLOC(sizeof(int));
        memset(piBool3, 0x00, sizeof(int));
		break;
	case BY_ROWS : 
		piBool3 = (int*)MALLOC(sizeof(int) * iCols);
        memset(piBool3, 0x00, sizeof(int)* iCols);
		break;
	case BY_COLS : 
		piBool3 = (int*)MALLOC(sizeof(int) * iRows);
        memset(piBool3, 0x00, sizeof(int) * iRows);
		break;
	}

	vect_and(piBool1, iRows, iCols, piBool3, iMode);

	switch(iMode)
	{
	case BY_ALL : 
		iRet = createScalarBoolean(_piKey, Rhs + 1, *piBool3);
		if(iRet)
		{
			return 0;
		}
		break;
	case BY_ROWS : 
		sciErr = createMatrixOfBoolean(_piKey, Rhs + 1, 1, iCols, piBool3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		break;
	case BY_COLS : 
		sciErr = createMatrixOfBoolean(_piKey, Rhs + 1, iRows, 1, piBool3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		break;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1)= Rhs + 1;
	PutLhsVar();
	return 0;

/*
int m1,n1,l1,mm2,l2,lo,nopt=0,lw;
    int opt=0;

    CheckRhs(1,2);
    CheckLhs(1,1);
    if (VarType(1)!=sci_boolean) {
        lw = 1 + Top - Rhs;
        C2F(overload)(&lw,"and",3L);
        return 0;}

    if (Rhs==2) {
        nopt=1;
        if (VarType(2)==sci_strings) {
            GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&lo);
            if (*cstk(lo)=='r')
                opt=1;
            else if (*cstk(lo)=='*')
                opt=0;
            else if (*cstk(lo)=='c')
                opt=2;
            else {
                Err=2;
                SciError(44);
            }

        }
        else {
            GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&lo);
            opt= (int) *stk(lo);
            if (opt>2||opt<1) {
                Err=2;
                SciError(44);
            }
        }
    }  //(Rhs==2) 

    //  checking variable a
    GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);

    if (Rhs==2 && m1*n1==0) {
        LhsVar(1)= 1;
        C2F(putlhsvar)();
        return 0;
    }

    // cross variable size checking
    mm2=1;
    l2 = 0;
    if (opt==0) 
    {
        CreateVar(2+nopt,MATRIX_OF_BOOLEAN_DATATYPE,&mm2,&mm2,&l2);
    }// named: x
    else if (opt==1) 
    {
        CreateVar(2+nopt,MATRIX_OF_BOOLEAN_DATATYPE,&mm2,&n1,&l2);
    }// named: x
    else if (opt==2) 
    {
        CreateVar(2+nopt,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&mm2,&l2);
    }// named: x
    vect_and(istk(l1),m1,n1,istk(l2),opt);
    LhsVar(1)= 2+nopt;
    C2F(putlhsvar)();
    return 0;
*/
}
/*--------------------------------------------------------------------------*/
