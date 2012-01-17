/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_io.h"
#include "stack-c.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsave)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int sci_save(char *fname,unsigned long fname_len)
{
    SciErr sciErr;

    int iOldSave    = FALSE;

    int* piAddr1    = NULL;
    int iType1      = 0;
    int iRows1      = 0;
    int iCols1      = 0;

    CheckRhs(1, 100000);
    CheckLhs(0, 1);

    //filename or file descriptor
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if(iType1 == sci_strings)
    {
        int* piAddrI    = NULL;
        int* piAddrI2   = NULL;
        int iTypeI      = 0;
        int iRowsI      = 0;
        int iColsI      = 0;
        char* pstVarI   = NULL;

        int i = 0;
        for(i = 2 ; i <= Rhs ; i++)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrI);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            sciErr = getVarType(pvApiCtx, piAddrI, &iTypeI);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if(iTypeI != sci_strings)
            {
                iOldSave = TRUE;
                break;
            }

            sciErr = getVarDimension(pvApiCtx, piAddrI, &iRowsI, &iColsI);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if(iRowsI != 1 || iColsI != 1)
            {
                iOldSave = TRUE;
                break;
            }

            if(getAllocatedSingleString(pvApiCtx, piAddrI, &pstVarI))
            {
                return 1;
            }

            //try to get varaible by name
            sciErr = getVarAddressFromName(pvApiCtx, pstVarI, &piAddrI2);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if(piAddrI2 == 0)
            {
                iOldSave = TRUE;
                break;
            }

            freeAllocatedSingleString(pstVarI);

            {
                int lw = 0;
                //call "overload" to prepare data to export_to_hdf5 function.
                C2F(overload) (&lw, "save", (unsigned long)strlen("save"));
            }

        }
    }
    else
    {
        iOldSave = TRUE;
    }


    //new -> %_export_to_hdf5
    //old
    if(iOldSave)
    {
	    C2F(intsave)();
    }

	return 0;
}
/*--------------------------------------------------------------------------*/
