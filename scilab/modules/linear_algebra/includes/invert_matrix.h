/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _INVERT_MATRIX_H_
#define _INVERT_MATRIX_H_

#include "dynlib_linear_algebra.h"
// TODO : add comments
LINEAR_ALGEBRA_IMPEXP int iInvertMatrixM(int iRows, int iCols, double* pData, int complexArg
		   , double* pdblRcond);

// TODO : add comments

#endif /* !_INVERT_MATRIX_H_ */
