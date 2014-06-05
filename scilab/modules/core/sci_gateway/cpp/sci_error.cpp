/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "types.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
}

#define DEFAULT_ERROR_CODE 10000

using namespace types;

Function::ReturnValue sci_error(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "error", 1);
        return Function::Error;
    }

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "error", 1, 2);
        return Function::Error;
    }

    if (in.size() == 1)
    {
        // RHS == 1
        if (in[0]->isString() == false && in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "error", 1);
            return Function::Error;
        }

        if (in[0]->isString() == true)
        {
            types::String* pStrError = in[0]->getAs<types::String>();
            std::string strErr = "";
            char* pstError = NULL;
            for (int i = 0; i < pStrError->getSize() - 1; i++)
            {
                pstError = wide_string_to_UTF8(pStrError->get(i));
                strErr = strErr + std::string(pstError) + std::string("\n");
                FREE(pstError);
            }

            pstError = wide_string_to_UTF8(pStrError->get(pStrError->getSize() - 1));
            strErr = strErr + std::string(pstError);
            FREE(pstError);

            Scierror(DEFAULT_ERROR_CODE, "%s", strErr.c_str());
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "error", 1);
            return Function::Error;
        }
    }
    else
    {
        types::Double* pDbl = NULL;
        types::String* pStr = NULL;
        int iPosDouble = 1;
        int iPosString = 1;
        // RHS = 2 according to previous check.
        if (in[0]->isDouble() == false && in[0]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "error", 1);
            return Function::Error;
        }

        if (in[1]->isString() == false && in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "error", 2);
            return Function::Error;
        }

        if (in[0]->isDouble() == false && in[1]->isDouble() == false ||
                in[0]->isString() == false && in[1]->isString() == false )
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "error", 2);
            return Function::Error;
        }

        if (in[0]->isDouble())
        {
            iPosString = 2;
            pDbl = in[0]->getAs<types::Double>();
            pStr = in[1]->getAs<types::String>();
        }
        else
        {
            iPosDouble = 2;
            pDbl = in[1]->getAs<types::Double>();
            pStr = in[0]->getAs<types::String>();
        }

        if (pDbl->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "error", iPosDouble);
            return Function::Error;
        }

        if (pStr->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "error", iPosString);
            return Function::Error;
        }

        if (pDbl->get(0) <= 0 || pDbl->isComplex())
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Value greater than 0 expected.\n"), "error", iPosDouble);
            return Function::Error;
        }

        char* pst = wide_string_to_UTF8(pStr->get(0));
        Scierror((int)pDbl->get(0), "%s", pst);
        FREE(pst);
    }

    return Function::Error;
}
