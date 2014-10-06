/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_OPTIMIZED_HXX__
#define __TYPES_OPTIMIZED_HXX__

#include "generic_operations.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "string.hxx"

extern "C"
{
#include "os_swprintf.h"
}

void fillDAXPYFunction();

//define arrays on operation functions
typedef types::InternalType*(*daxpy_function)(types::InternalType*, types::InternalType*, types::InternalType*);

types::InternalType* daxpy_S_M_M(types::Double*, types::Double*, types::Double*);
types::InternalType* daxpy_M_S_M(types::Double*, types::Double*, types::Double*);
types::InternalType* daxpy_S_S_S(types::Double*, types::Double*, types::Double*);

#endif /* !__TYPES_OPTIMIZED_HXX__ */