/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#ifndef __MAX_H__
#define __MAX_H__

#include <vector>
#include "double.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

#ifdef _MSC_VER
#undef max //to remove ~define from windows header
#endif

ELEMENTARY_FUNCTIONS_IMPEXP void max(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut);

#endif /* __MAX_H__ */
