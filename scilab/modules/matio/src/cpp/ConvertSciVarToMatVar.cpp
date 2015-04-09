/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConvertSciVarToMatVar.hxx"
#include "GetMatlabVariable.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
}

using namespace types;

matvar_t *ConvertSciVarToMatVar(InternalType* pIT, const char *name, int matfile_version)
{
    int Dims = pIT->getAs<GenericType>()->getDims();
    int* pDims = pIT->getAs<GenericType>()->getDimsArray();
    int isize = pIT->getAs<GenericType>()->getSize();
    size_t* psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    struct mat_complex_split_t mat5ComplexData;

    for (int i = 0; i < Dims; i++)
    {
        psize_t[i] = (int)pDims[i];
    }

    switch (pIT->getType())
    {
        case GenericType::ScilabDouble:
        {
            return GetMat_VarDouble(pIT->getAs<Double>(), name, matfile_version);
        }
        break;
        case GenericType::ScilabInt8:
        case GenericType::ScilabUInt8:
        case GenericType::ScilabInt16:
        case GenericType::ScilabUInt16:
        case GenericType::ScilabInt32:
        case GenericType::ScilabUInt32:
            break;
#ifdef __SCILAB_INT64__
        case GenericType::ScilabInt64:
        case GenericType::ScilabUInt64:
#endif
        {
            return GetMat_VarInteger(pIT, name);
        }
        break;
        case GenericType::ScilabString:
        {
            return GetMat_VarChar(pIT->getAs<String>(), name);
        }
        break;
        case GenericType::ScilabSparse:
        {
            return GetMat_VarSparse(pIT->getAs<Sparse>(), name);
        }
        break;
        case GenericType::ScilabCell:
        {
            return GetMat_VarCell(pIT->getAs<Cell>(), name, matfile_version);
        }
        break;
        case GenericType::ScilabStruct:
        {
            return GetMat_VarStruct(pIT->getAs<Struct>(), name, matfile_version);
        }
        break;
        default:
            break;
    }
    Scierror(999, "Do not known how to get variable of type %d\n", pIT->getType());
    return NULL;
}
