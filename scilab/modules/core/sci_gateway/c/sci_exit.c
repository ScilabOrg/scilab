/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "exitCodeValue.h"
/*--------------------------------------------------------------------------*/
#define QUIT "quit"
/*--------------------------------------------------------------------------*/
extern int C2F(compil)(int *, int *, int *, int *, int *);
/*--------------------------------------------------------------------------*/
static void decreasePauseLevel(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_exit)(char *fname,unsigned long fname_len)
{
    if (strcmp(fname, QUIT) == 0)
    {
        int code = 17; /* see compil.f for code value*/
        int zero = 0;

        /* special compilation mode skip commands */
        if (C2F(com).comp[2] == 1)
        {
            C2F(com).fun = 0;
            C2F(com).fin = 2;

            LhsVar(1) = 0; 
            C2F(putlhsvar)();

            return 0;
        }

        /* compilation quit with code 17 */
        if (C2F(compil)(&code, &zero, &zero, &zero, &zero))
        {
            LhsVar(1) = 0; 
            C2F(putlhsvar)();
            return 0;
        }

        if (C2F(recu).paus != 0)
        {
            /* quit in a pause */
            /* decrease pause level */
            decreasePauseLevel();

            /* recall macro to terminate the pause level */
            C2F(com).fun = 0;
            C2F(com).fin = 2;
        }
        else
        {
            setExitCodeValue(0);
            C2F(com).fun = -999;
        }
    }
    else
    {
        SciErr sciErr;

        CheckLhs(1,1);
        CheckRhs(0,1);

        if (Rhs == 0)
        {
            setExitCodeValue(0);
        }
        else
        {
            int iExit = 0;
            int m1 = 0, n1 = 0;
            int iType1 = 0;
            int *piAddressVarOne = NULL;
            double *pdVarOne = NULL;

            /* get Address of inputs */
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            /* check input type */
            if ( iType1 != sci_matrix )
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname,1);
                return 0;
            }

            sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne,&m1,&n1,&pdVarOne);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if( n1 != 1 || m1 != 1)
            {
                Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
                return 0;
            }

            iExit = (int) *pdVarOne;

            if (*pdVarOne != (double)iExit)
            {
                Scierror(999,_("%s: Wrong value for input argument #%d: A integer expected.\n"),fname,1);
                return 0;
            }

            setExitCodeValue(iExit);
        }

        /* this value do quit in scirun */
        C2F(com).fun = -999;
    }

    LhsVar(1) = 0; 
    C2F(putlhsvar)();

    return 0;
}
/*--------------------------------------------------------------------------*/
static void decreasePauseLevel(void)
{
    /* decrease pause level */
    C2F(recu).pt--;

    /* suppress loop variables if any */
    if (C2F(recu).rstk[C2F(recu).pt - 1] == 802 || 
        C2F(recu).rstk[C2F(recu).pt - 1] == 612 || 
        C2F(recu).rstk[C2F(recu).pt - 1] == 805 && 
        C2F(recu).ids[C2F(recu).pt * 6 - 6] == 3 || 
        C2F(recu).rstk[C2F(recu).pt - 1] == 616 &&
        C2F(recu).pstk[C2F(recu).pt - 1] == 10)
    {
        C2F(vstk).top--;
    }

    if (C2F(recu).rstk[C2F(recu).pt - 1] != 503)
    {
        decreasePauseLevel();
    }
}
/*--------------------------------------------------------------------------*/
