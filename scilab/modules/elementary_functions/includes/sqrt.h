/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#ifndef __SQRT_H__
#define __SQRT_H__

#include "dynlib_elementary_functions.h"

ELEMENTARY_FUNCTIONS_IMPEXP double dsqrts(double _dblVal);
ELEMENTARY_FUNCTIONS_IMPEXP void zsqrts(double _dblRealIn, double _dblImgIn, double* _pdblRealOut, double *_pdblImgOut);

#endif