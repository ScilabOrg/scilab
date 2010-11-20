/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_show_pixmap.c                                                */
/* desc : interface for show_pixmap routine                               */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "DrawObjects.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_show_pixmap(char *fname, unsigned long fname_len)
{
    /* call show_pixmap */

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    /* call show_pixmap function */
    showPixmap(sciGetCurrentFigure());

    LhsVar(1) = 0;
    C2F(putlhsvar) ();
    return 0;
}

/*--------------------------------------------------------------------------*/
