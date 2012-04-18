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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_unzoom.h                                                     */
/* desc : interface for unzoom routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
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
int sci_unzoom(char *fname,unsigned long fname_len)
{
    /* number of object to unzoom */
    int nbObjects = 0;

    /* ids of object to unzoom */
    char** objectsId = NULL;

    char* objectUID;

    /* object type */
    char *pstType;

    CheckRhs(0,1) ;
    CheckLhs(0,1) ;
    if ( Rhs == 0 )
    {
        objectUID = getCurrentFigure();
        if (objectUID != NULL)
        {
            sciUnzoomFigure(objectUID);
        }
    }
    else
    {
        int m,n,i;
        size_t stackPointer = 0;
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &m, &n, &stackPointer);

        nbObjects = m * n;
        objectsId = MALLOC(nbObjects * sizeof(char*));
        if (objectsId == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"),fname);
            return -1;
        }

        /* first pass, check that all the handles are subwindows or figures */
        /* and copy them into an array of objects */
        startCurrentFigureDataWriting();
        for (i = 0; i < nbObjects; i++ )
        {
            objectUID = getObjectFromHandle(getHandleFromStack(stackPointer + i));
            getGraphicObjectProperty(objectUID, __GO_TYPE__, jni_string, (void **) &pstType);
            if ((strcmp(pstType, __GO_FIGURE__) != 0) && (strcmp(pstType, __GO_AXES__) != 0))
            {
                FREE(objectsId);
                Scierror(999, _("%s: Wrong type for input argument: Vector of Axes and Figure handles expected.\n"),fname);
                endCurrentFigureDataWriting();
                return -1;
            }
            objectsId[i] = objectUID;
        }

        /* second pass un zoom the objects */
        sciUnzoomArray(objectsId, nbObjects);
        endCurrentFigureDataWriting();

        FREE(objectsId);
    }


    LhsVar(1)=0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
