/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.hxx"

extern "C"
{
#include "sci_types.h"
#include "api_scilab.h"
}

matvar_t *GetIntegerVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{
    types::GatewayStruct* pStr = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pStr->m_pIn;

    if (in[iVar - 1]->isInt() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: Integer matrix expected.\n"), "GetIntegerVariable");
        return NULL;
    }

    types::InternalType* pIT = in[iVar - 1]->getAs<types::InternalType>();

    return GetMat_VarInteger(pIT, name);
}

matvar_t* GetMat_VarInteger(types::InternalType* pITIn, const char *name)
{
    int Dims = pITIn->getAs<types::GenericType>()->getDims();
    int* pDims = pITIn->getAs<types::GenericType>()->getDimsArray();
    matvar_t * pMatVarOut = NULL;

    size_t* psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (psize_t == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetMat_VarInteger");
        return NULL;
    }

    for (int i = 0; i < Dims; i++)
    {
        psize_t[i] = (int)pDims[i];
    }

    switch (pITIn->getType())
    {
        case types::GenericType::ScilabInt8:
        {
            types::Int8* pInt8 = pITIn->getAs<types::Int8>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, Dims, psize_t, pInt8->get(), 0);
        }
        break;
        case types::GenericType::ScilabUInt8:
        {
            types::UInt8* pUInt8 = pITIn->getAs<types::UInt8>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, Dims, psize_t, pUInt8->get(), 0);
        }
        break;
        case types::GenericType::ScilabInt16:
        {
            types::Int16* pInt16 = pITIn->getAs<types::Int16>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, Dims, psize_t, pInt16->get(), 0);
        }
        break;
        case types::GenericType::ScilabUInt16:
        {
            types::UInt16* pUInt16 = pITIn->getAs<types::UInt16>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, Dims, psize_t, pUInt16->get(), 0);
        }
        break;
        case types::GenericType::ScilabInt32:
        {
            types::Int32* pInt32 = pITIn->getAs<types::Int32>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, Dims, psize_t, pInt32->get(), 0);
        }
        break;
        case types::GenericType::ScilabUInt32:
        {
            types::UInt32* pUInt32 = pITIn->getAs<types::UInt32>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, Dims, psize_t, pUInt32->get(), 0);
        }
        break;
#ifdef __SCILAB_INT64__
        case types::GenericType::ScilabInt64:
        {
            types::Int64* pInt64 = pITIn->getAs<types::Int64>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT64, MAT_T_INT64, Dims, psize_t, pInt64->get(), 0);
        }
        break;
        case types::GenericType::ScilabUInt64:
        {
            types::UInt64* pUInt64 = pITIn->getAs<types::UInt64>();
            pMatVarOut = Mat_VarCreate(name, MAT_C_INT64, MAT_T_INT64, Dims, psize_t, pUInt64->get(), 0);
        }
#endif
        default:
            Scierror(999, _("%s: Wrong type for first input argument: Integer matrix expected.\n"), "GetMat_VarInteger");
            return NULL;
            break;
    }

    FREE(psize_t);
    return pMatVarOut;
}