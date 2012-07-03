/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: sci_show_window.c                                                */
/* desc : interface for show_window routine                               */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "gw_graphics.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "stack-c.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "localization.h"
#include "Interaction.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"
/*--------------------------------------------------------------------------*/
int sci_show_window( char *fname,unsigned long fname_len )
{
    char* pFigureUID = NULL;

    CheckRhs(0,1);
    CheckLhs(0,1);

    if ( Rhs == 1 )
    {
        /* the window to show is specified */
        int paramType    = VarType(1);
        int nbRow        = 0 ;
        int nbCol        = 0 ;
        size_t stackPointer = 0 ;
        char *type = NULL;

        if ( isParameterHandle( paramType ) )
        {
            /* by tis handle */
            GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer );

            if ( nbRow * nbCol != 1 )
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname, 1, "Figure");
                return -1 ;
            }

            pFigureUID = (char*)getObjectFromHandle( getHandleFromStack(stackPointer) );

            if (pFigureUID == NULL)
            {
                Scierror(999, _("%s: Handle does not or no longer exists.\n"),fname);
                return -1 ;
            }

            getGraphicObjectProperty(pFigureUID, __GO_TYPE__, jni_string, (void **) &type);
            if (strcmp(type, __GO_FIGURE__) != 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname, 1, "Figure");
                return -1 ;
            }

        }
        else if ( isParameterDoubleMatrix( paramType ) )
        {
            /* by its number */
            int winNum = 0;
            GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stackPointer );
            if ( nbRow * nbCol != 1 )
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname, 1, "Figure");
                return -1 ;
            }
            winNum = (int) getDoubleFromStack(stackPointer);
            pFigureUID = (char*)getFigureFromIndex(winNum);

            if (pFigureUID == NULL)
            {
                /* No window with this number, create one */
                if(sciSetUsedWindow(winNum) < 0)
                {
                    Scierror(999, _("%s: Unable to create requested figure: No more memory.\n"), fname);
                    return -1;
                }
                pFigureUID = (char*)getCurrentFigure();
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname,1, "Figure");
            return -1;
        }
    }
    else
    {
        /* Rhs == 0 */
        /* raise current figure */
        getOrCreateDefaultSubwin();
        pFigureUID = (char*)getCurrentFigure();
    }

    /* Check that the requested figure really exists */
    if ( pFigureUID == NULL )
    {
        Scierror(999, _("%s: '%s' handle does not or no longer exists.\n"),fname,"Figure");
        return -1 ;
    }

    /* Actually show the window */
    showWindow(pFigureUID);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
