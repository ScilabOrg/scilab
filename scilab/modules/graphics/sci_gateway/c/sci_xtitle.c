/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xtitle.c                                                     */
/* desc : interface for xtitle routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"

#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
int sci_xtitle( char * fname, unsigned long fname_len )
{
    int  narg;
    int  nbLabels; /* number of modified labels */
    int  box = 0;
    BOOL isBoxSpecified = FALSE;
    char * psubwinUID = NULL;
    static rhs_opts opts[] = { {-1,"boxed","i" ,0,0,0},
    {-1,NULL   ,NULL,0,0,0} };

    if (Rhs <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckRhs(1,5);


    nbLabels = Rhs;

    /* get the given options from the name in opts */
    if ( !get_optionals(fname,opts) )
    {
        /* error */
        return 0;
    }

    /* compatibility with previous version in which box was put */
    /* at the fourth position */

    if ( Rhs == 4 )
    {
        int type = GetType(4);
        if ( type == 1 || type == 8 )/* double or int */
        {
            int n,m;
            int boxPtr   = -1 ; /* pointer of box on the stack */
            GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&m,&n,&boxPtr);
            CheckScalar(4,m,n);
            box = *istk( boxPtr );
            nbLabels--; /* it is not a label text */
            isBoxSpecified = TRUE;
        }
    }

    if ( opts[0].position != -1 && !isBoxSpecified )
    {
        /* check if "box" is in the options */
        box = *istk(opts[0].l) ;
        if ( opts[0].m * opts[0].n != 1 )
        {
            /* check size */
            Scierror( 999, _("%s: Wrong type for input argument: Scalar expected.\n"), fname );
            return 1;
        }
        nbLabels--; /* it is not a label text */
    }

    psubwinUID = getOrCreateDefaultSubwin();

    for ( narg = 1 ; narg <= nbLabels ; narg++)
    {
        int m,n;
        char **Str;
        char * modifiedLabel = NULL;

        GetRhsVar(narg,MATRIX_OF_STRING_DATATYPE,&m,&n,&Str);
        if ( m*n == 0 )
        {
            continue;
        }

        switch(narg)
        {
            case 1:
                getGraphicObjectProperty(psubwinUID, __GO_TITLE__, jni_string, &modifiedLabel);
                break;
            case 2:
                getGraphicObjectProperty(psubwinUID, __GO_X_AXIS_LABEL__, jni_string, &modifiedLabel);
            break;
            case 3:
                getGraphicObjectProperty(psubwinUID, __GO_Y_AXIS_LABEL__, jni_string, &modifiedLabel);
                break;
            case 4:
                getGraphicObjectProperty(psubwinUID, __GO_Z_AXIS_LABEL__, jni_string, &modifiedLabel);
                break;
            default:
                break;
        }

#if 0
        startFigureDataWriting(pFigure);
#endif

        sciSetText(modifiedLabel, Str, m, n);

        setGraphicObjectProperty(modifiedLabel, __GO_FILL_MODE__, &box, jni_bool, 1);

#if 0
        endFigureDataWriting(pFigure);
#endif

        freeArrayOfString(Str,m*n);
    }

    setCurrentObject(psubwinUID);
#if 0
    sciDrawObj(pFigure);
#endif

    LhsVar(1)=0;
    C2F(putlhsvar)();
    return 0;
}

/*--------------------------------------------------------------------------*/
