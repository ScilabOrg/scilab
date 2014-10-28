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

#ifndef __TYPES_DOTPOWER_HXX__
#define __TYPES_DOTPOWER_HXX__

#include "generic_operations.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

// FIXME : remove using namespace.
using namespace types;

//dot power operations
int DotPowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut);
int DotPowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, InternalType** _pOut);
int DotPowerSpaseByDouble(Sparse* _pSp, Double* _pDouble, InternalType** _pOut);

#endif /* __TYPES_DOTPOWER_HXX__ */
