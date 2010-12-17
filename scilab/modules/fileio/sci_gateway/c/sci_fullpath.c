/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <stdlib.h>
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "fullpath.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_fullpath(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char **pStVarOne = NULL;
    int *lenStVarOne = NULL;
    int mOne = 0, nOne = 0;
    int mnOne = 0;

    char **pStFullPath = NULL;
    char fullpathtmp[PATH_MAX*4];

    int i = 0;

    Rhs = Max(Rhs, 0);
    CheckRhs(1,1);
    CheckLhs(0,1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
        {
            createEmptyMatrix(pvApiCtx, Rhs + 1);
            LhsVar(1) = Rhs + 1;
            PutLhsVar()
        }
        else
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        }
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne, &nOne, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    mnOne = mOne * nOne;

    lenStVarOne = (int*)MALLOC(sizeof(int) * mnOne);
    if (lenStVarOne == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne, &nOne, lenStVarOne, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pStVarOne = (char**) MALLOC(sizeof(char*) * mnOne);
    if (pStVarOne == NULL)
    {
        if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < mnOne; i++)
    {
        pStVarOne[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
        if (pStVarOne[i] == NULL)
        {
            freeArrayOfString(pStVarOne, i);
            if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, pStVarOne);
    if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    if(sciErr.iErr)
    {
        freeArrayOfString(pStVarOne, mnOne);
        printError(&sciErr, 0);
        return 0;
    }

    pStFullPath = (char**)MALLOC(sizeof(char*) * mnOne);
    if (pStFullPath == NULL)
    {
        freeArrayOfString(pStVarOne, mnOne);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < mnOne; i++)
    {
        if( get_full_path( fullpathtmp, pStVarOne[i], PATH_MAX*4 ) != NULL )
        {
            pStFullPath[i] = strdup(fullpathtmp);
        }
        else
        {
            Scierror(999,_("%s: Wrong value for input argument #%d: '%s' is an invalid path.\n"),fname,1, pStVarOne[i]);
            freeArrayOfString(pStVarOne, mnOne);
            freeArrayOfString(pStFullPath, mnOne);
            return 0;
        }
    }

    freeArrayOfString(pStVarOne, mnOne);
    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1 , mOne, nOne, pStFullPath);
    freeArrayOfString(pStFullPath, mnOne);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar()
        return 0;
}
/*--------------------------------------------------------------------------*/

