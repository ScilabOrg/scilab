/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "inf.h"
// =============================================================================
double returnINF(BOOL bPositive)
{
    double v = 0 - 0;
    double p = 10;
    if (!bPositive) p = -10;
    return (double) p / (double)v;
}
// =============================================================================
