/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Thread_Wrapper.h"
#include "gw_io.h"
#include "stack-c.h"
#include "systemc.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/

//exchange structure between thread and gateway
typedef struct __HOST_PARAM__
{
    char* pstCommand;
    int iReturn;
}HostParam;

//threaded function to manage timeout
static void* hostThreaded(void* _pvArg)
{
    HostParam* pParam = (HostParam*)_pvArg;
    C2F(systemc)(pParam->pstCommand, &pParam->iReturn);
}

int sci_host(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr1        = 0;
    int iTimeOut        = 0;
    __threadId hostThread;
    HostParam hostParam;

    CheckRhs(1,2);
	CheckLhs(1,1);

    //optinal parameter 2 ( timeout in second)
    if(Rhs == 2)
    {
        double dbl;
        int* piAddr2 = NULL;
        
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

        if(!isDoubleType(pvApiCtx, piAddr2) || !isScalar(pvApiCtx, piAddr2))
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 2);
            return 0;
        }

        if(getScalarDouble(pvApiCtx, piAddr2, &dbl))
        {
            return 0;
        }
        iTimeOut = (int)dbl;
    }

    //Command
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if(!isStringType(pvApiCtx, piAddr1) || !isScalar(pvApiCtx, piAddr1))
    {
        Scierror(55,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
        return 0;
    }

    if(getAllocatedSingleString(pvApiCtx, piAddr1, &hostParam.pstCommand))
    {
        return 0;
    }

    __CreateThreadWithParams(&hostThread, hostThreaded, (void*)&hostParam);

    if(Rhs == 2)
    {
        if(__WaitTimeoutThread(hostThread, iTimeOut))
        {//timeout
            if(createScalarDouble(pvApiCtx, Rhs + 1, (double)-1))
            {
                return 0;
            }

            LhsVar(1)= Rhs + 1;
            PutLhsVar();
            return 0;
        }
    }
    else
    {
        __WaitThreadDie(hostThread);
    }

    if(createScalarDouble(pvApiCtx, Rhs + 1, (double)hostParam.iReturn))
    {
        return 0;
    }

	LhsVar(1)= Rhs + 1;
	PutLhsVar();
	FREE(hostParam.pstCommand);
	return 0;
}
/*--------------------------------------------------------------------------*/
