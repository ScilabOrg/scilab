/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
 * Copyright (C) 2014 - Scilab Enterprises - Anais Aubert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "double.hxx"
#include "context.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_zeros(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOut = NULL;

    int iDims = 0;
    int* piDims = NULL;
    bool alloc = false;

    bool ret = getDimsFromArguments(in, "zeros", &iDims, &piDims, &alloc);
    if (ret == false)
    {
        switch (iDims)
        {
            case -1:
                Scierror(21, _("Invalid index.\n"));
                break;
            case 1:
            {
                //call overload
                ast::ExecVisitor exec;
                return Overload::generateNameAndCall(L"zeros", in, _iRetCount, out, &exec);
            }
        }

        return types::Function::Error;
    }

    pOut = new Double(iDims, piDims);
    if (alloc)
    {
        delete[] piDims;
    }

    pOut->setZeros();
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
