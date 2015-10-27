/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_argn(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    symbol::Context* pContext = symbol::Context::getInstance();

    static symbol::Variable* nargin = pContext->getOrCreate(symbol::Symbol(L"nargin"));
    static symbol::Variable* nargout = pContext->getOrCreate(symbol::Symbol(L"nargout"));

    int iRhs = static_cast<int>(in.size());
    //check input arguments
    if (iRhs > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "argn", 0, 1);
        return types::Function::Error;
    }

    //check output arguments
    if (iRhs == 0 && _iRetCount > 2)
    {
        Scierror(41, _("%s: Wrong number of output arguments: %d expected.\n"), "argn", 2);
        return types::Function::Error;
    }

    types::InternalType* pIn = nargin->empty() ? nullptr : nargin->top()->m_pIT;
    types::InternalType* pOut = nargout->empty() ? nullptr : nargout->top()->m_pIT;

    if (pIn == nullptr || pOut == nullptr)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        switch (iRhs)
        {
            case 0:
            {
                if (_iRetCount == 1)
                {
                    //argn() returns lhs
                    out.push_back(pOut);
                }
                else// if (_iRetCount == 2)
                {
                    //[a,b] = argn() returns lhs and rhs
                    out.push_back(pOut);
                    out.push_back(pIn);
                }

                break;
            }
            case 1:
            {
                //check input argument types
                types::Double* input = in[0]->getAs<types::Double>();
                if (input->getId() != types::InternalType::IdScalarDouble)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "argn", 1);
                    return types::Function::Error;
                }

                int val = static_cast<int>(input->get()[0]);
                switch (val)
                {
                    case 1:
                    {
                        //returns lhs
                        out.push_back(pOut);
                        break;
                    }
                    case 2:
                    {
                        //returns rhs
                        out.push_back(pIn);
                        break;
                    }
                    case 0:
                    {
                        //returns lhs
                        out.push_back(pOut);
                        if (_iRetCount == 2)
                        {
                            //[a,b] = argn(0) returns lhs ans rhs
                            out.push_back(pIn);
                        }
                        break;
                    }
                    default:
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s'.\n"), "argn", 1, "0", "1", "2");
                        return types::Function::Error;
                    }
                }
            }
        }
    }

    return types::Function::OK;
}
