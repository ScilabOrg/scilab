/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#include "ScilabView.hxx"

extern "C"
{
#include <string.h>

#include "BOOL.h"
#include "CurrentFigure.h"
}

int getCurrentFigure(void)
{
    return ScilabView::getCurrentFigure();
}

void setCurrentFigure(int UID)
{
    ScilabView::setCurrentFigure(UID);
}

BOOL isCurrentFigure(int UID)
{
    if (UID == ScilabView::getCurrentFigure())
    {
        return TRUE;
    }
    return FALSE;
}
