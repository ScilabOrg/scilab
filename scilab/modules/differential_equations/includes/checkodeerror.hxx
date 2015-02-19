/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __CHECK_ODE_ERROR__
#define __CHECK_ODE_ERROR__

#include <string>
#include "dynlib_differential_equations.h"

DIFFERENTIAL_EQUATIONS_IMPEXP int checkOdeError(int meth, int istate);
DIFFERENTIAL_EQUATIONS_IMPEXP int checkError(int idid, std::string strName);

#endif // __CHECK_ODE_ERROR__
