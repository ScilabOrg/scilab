/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_unzoom.h                                                     */
/* desc : interface for unzoom routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "axesScale.h"
#include "localization.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"

#include "CurrentFigure.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_unzoom(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrstackPointer = NULL;
    long long* stackPointer = NULL;

    /* number of object to unzoom */
    int nbObjects = 0;

    /* object type */
    int iType = -1;
    int *piType = &iType;

    /* ids of object to unzoom */
    int* iObjectsId = NULL;
    int iObjectUID = 0;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);
    if (nbInputArgument(pvApiCtx) == 0)
    {
        iObjectUID = getCurrentFigure();
        if (iObjectUID != 0)
        {
            sciUnzoomFigure(iObjectUID);
        }
    }
    else
    {
        int m = 0, n = 0, i = 0;
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrstackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of handle at position 1.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrstackPointer, &m, &n, &stackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }

        nbObjects = m * n;
        iObjectsId = (int*)MALLOC(nbObjects * sizeof(int));
        if (iObjectsId == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return -1;
        }

        /* first pass, check that all the handles are subwindows or figures */
        /* and copy them into an array of objects */
        for (i = 0; i < nbObjects; i++)
        {
            iObjectUID = getObjectFromHandle((long int)stackPointer[i]);
            getGraphicObjectProperty(iObjectUID, __GO_TYPE__, jni_int, (void **) &piType);
            if (iType != __GO_FIGURE__ && iType != __GO_AXES__)
            {
                FREE(iObjectsId);
                Scierror(999, _("%s: Wrong type for input argument: Vector of Axes and Figure handles expected.\n"), fname);
                return -1;
            }

            iObjectsId[i] = iObjectUID;
        }

        /* second pass un zoom the objects */
        sciUnzoomArray(iObjectsId, nbObjects);
        FREE(iObjectsId);
    }


    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
