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

#ifndef __TYPES_POWER_HXX__
#define __TYPES_POWER_HXX__

#include "dynlib_operations.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "polynom.hxx"

using namespace types;

//power operations
EXTERN_OP InternalType* GenericPower(InternalType *_pLeftOperand, InternalType *_pRightOperand);
int PowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut);
int PowerPolyByDouble(Polynom* _pPoly, Double* _pDouble, InternalType** _pOut);

//dot power operations
EXTERN_OP InternalType* GenericDotPower(InternalType *_pLeftOperand, InternalType *_pRightOperand);
int DotPowerDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut);

extern "C"
{
/*declare extern fortran functions*/
extern void C2F(dpmul1)(double* _pdbl1, int* _piRank1, double* _pdbl2, int* _piRank2, double* _pdblOut);
extern void C2F(wpmul1)(double* _pdblR1, double* _pdblI1, int* _piRank1, double* _pdblR2, double* _pdblI2, int* _piRank2, double* _pdblOutR, double* _pdblOutI);
}
#endif /* __TYPES_POWER_HXX__ */
