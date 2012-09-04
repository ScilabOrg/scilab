/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2012 - DIGITEO - cedric delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GSORT_H__
#define __GSORT_H__

#include "double.hxx"
#include "sparse.hxx"
#include "string.hxx"
#include "int.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP types::Double* gsort(types::Double* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);

ELEMENTARY_FUNCTIONS_IMPEXP types::String* gsort(types::String* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);

ELEMENTARY_FUNCTIONS_IMPEXP types::Int8* gsort(types::Int8* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::Int16* gsort(types::Int16* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::Int32* gsort(types::Int32* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::Int64* gsort(types::Int64* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt8* gsort(types::UInt8* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt16* gsort(types::UInt16* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt32* gsort(types::UInt32* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);
ELEMENTARY_FUNCTIONS_IMPEXP types::UInt64* gsort(types::UInt64* pIn, types::Double* pInd, std::wstring wstrWay, std::wstring wstrProcess);

#endif /* __GSORT_H__ */
