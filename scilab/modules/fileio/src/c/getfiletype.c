/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "getfiletype.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
void C2F(getfiletype)(int *fd, int *type, int *ierr)
{
    if (*fd < 0 || *fd >= GetMaximumFileOpenedInScilab() )
    {
        *ierr = 1;
        return;
    }
    if ( GetFileTypeOpenedInScilab(*fd) == 0 )
    {
        *ierr = 2;
        return;
    }
    *type = GetFileTypeOpenedInScilab(*fd);
    *ierr = 0;
}
/*--------------------------------------------------------------------------*/
