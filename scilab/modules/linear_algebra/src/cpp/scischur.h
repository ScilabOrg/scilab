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

#include "doublecomplex.h"

int schur_dgees(double* real, double* complex);
int schur_zgees(doublecomplex* complex);
int schur_dgges(double* real, double* complex, double* beta);
int schur_zgges(doublecomplex* alpha, doublecomplex* beta);
