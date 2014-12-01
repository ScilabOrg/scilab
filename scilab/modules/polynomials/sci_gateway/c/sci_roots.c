/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "api_scilab.h"
#include "machine.h"
#include "gw_polynomials.h"
#include "Scierror.h"
#include "localization.h"
#include "jenkins_traub.h"
/*--------------------------------------------------------------------------*/
extern int C2F(introots)(int *id);
/*--------------------------------------------------------------------------*/
int sci_roots(char *fname, unsigned long fname_len)
{
    int *piAddr1;
    int *piAddr2;
    SciErr sciErr;
    int n1, m1;
    int nbCoeff;
    int iErr;
    double* realCoeff;
    char* rootFindingMethod;
    char jenkinsTraubMethod[] = "l";
    BOOL isRealPoly = FALSE;
    BOOL isJenkinsTraubMethod = FALSE;
    double* res_real;
    double* res_img;
    int nbRoots;
    int iRhs = nbInputArgument(pvApiCtx);

    static int id[6];
    C2F(introots)(id);

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);


    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isPolyType(pvApiCtx, piAddr1))
    {
        if (isScalar(pvApiCtx, piAddr1))
        {
            if (!isVarComplex(pvApiCtx, piAddr1))
            {
            getAllocatedSinglePoly(pvApiCtx, piAddr1, &nbCoeff, &realCoeff);
            isRealPoly = TRUE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 1, 1, 1);
            return 1;
        }
    }
    else if (isDoubleType(pvApiCtx, piAddr1))
    {
        if (!isVarComplex(pvApiCtx, piAddr1))
        {
            getMatrixOfDouble(pvApiCtx, piAddr1, &n1, &m1, &realCoeff);
            if ((n1 == 1) || (m1 == 1))
            {
                nbCoeff = n1 * m1;
                isRealPoly = TRUE;
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 matrix expected.\n"), fname, 1);
                return 1;
            }
        }
    } 
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d:  Polynomial expected.\n"), fname, 1);
        return 1;
    }

    if(iRhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Second argument is a 1 by 1 matrix of string
        if (isStringType(pvApiCtx, piAddr2))
        {
            if (isScalar(pvApiCtx, piAddr2))
            {
                iErr = getAllocatedSingleString(pvApiCtx, piAddr2, &rootFindingMethod);
                if (strcmp(rootFindingMethod, jenkinsTraubMethod) == 0)
                {
                    isJenkinsTraubMethod = TRUE;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), fname, 2, 1, 1);
                return 1;
            }
        } 
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
            return 1;
        }
    }

    if (isJenkinsTraubMethod && isRealPoly)
    {
        // The number of roots is at most the degree of the polynomials
        res_real = (double*) malloc (sizeof (double) * nbCoeff);
        res_img = (double*) malloc (sizeof (double) * nbCoeff);
       
        // Jenkins Traub Method
        iErr = jenkins_traub(realCoeff, nbCoeff - 1, res_real, res_img, &nbRoots);
        sciErr = createComplexMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRoots, 1, res_real, res_img);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {    
        // Call Fortran method
        C2F(introots)(id);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
