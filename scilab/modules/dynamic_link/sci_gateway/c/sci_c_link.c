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

/*--------------------------------------------------------------------------*/
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_c_link(char *fname, int* _piKey)
{
    SciErr sciErr;
    int iRet = 0;
	int ilib = 0;
    int* piAddress;
    int iType;

	CheckRhs(1,2);
	CheckLhs(1,2);

	/* [test,ilib]=c_link(routine-name)  */
	/* test = c_link(routine-name)  */
	/* test = c_link(routine-name,num)  */

	if (Rhs == 2)
	{
        sciErr = getVarAddressFromPosition(_piKey, 2, &piAddress);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        sciErr = getVarType(_piKey, piAddress, &iType);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        if (iType == sci_matrix)
		{
			int m1 = 0, n1 = 0, l1 = 0;
            double* pdblReal = NULL;
            sciErr = getMatrixOfDouble(_piKey, piAddress, &m1, &n1, &pdblReal);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }
			if ( (m1 == n1) && (n1 == 1) )
			{
				ilib =  (int) pdblReal[0];
			}
			else
			{
				Scierror(999,_("%s : second argument must be a unique id of a shared library.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s : second argument must be a unique id of a shared library.\n"),fname);
			return 0;
		}
	}
	else ilib = -1;

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddress);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }
    sciErr = getVarType(_piKey, piAddress, &iType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

	if (iType == sci_strings)
	{
		char **routinename = NULL;
		int m1 = 0, n1 = 0;
        iRet = getAllocatedMatrixOfString(_piKey, piAddress, &m1, &n1, &routinename);
        if(iRet)
        {
            freeAllocatedMatrixOfString(m1, n1, routinename);
            return iRet;
        }

		if ( (m1 == 1) && (n1 == 1) )
		{
			int paramoutINT;
			BOOL FindFunction = FALSE;

			FindFunction = c_link(routinename[0],&ilib);
			if (routinename)
            {
                freeAllocatedMatrixOfString(m1, n1, routinename);
                routinename = NULL;
            }

			if ( FindFunction )
			{
				paramoutINT=(int)(TRUE);
			}
			else
			{
				paramoutINT=(int)(FALSE);
			}

            iRet = createScalarBoolean(_piKey, Rhs + 1, paramoutINT);
            if(iRet)
            {
                return 0;
            }
			LhsVar(1) = Rhs + 1;

			if (Lhs == 2)
			{
                iRet = createScalarInteger32(_piKey, Rhs + 2, (int) ilib);
                if(iRet)
                {
                    return 0;
                }
				LhsVar(2) = Rhs + 2;
			}

			PutLhsVar();
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname,1);
		}
	}
	else
	{
		SciError(201);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
