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
/* file: sci_drawlater.c                                                  */
/* desc : interface for drawlater routine                                 */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "GraphicSynchronizerInterface.h"
#include "SetProperty.h"

/*--------------------------------------------------------------------------*/
int sci_drawlater( char * fname, unsigned long fname_len )
{
	sciPointObj *pfigure = NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	if (Rhs <= 0)
	{
		startGraphicDataWriting();
		pfigure = sciGetCurrentFigure ();
		sciSetImmediateDrawingMode(pfigure, FALSE);
		endGraphicDataWriting();
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
