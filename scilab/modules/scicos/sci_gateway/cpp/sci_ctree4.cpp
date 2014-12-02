/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>
#include <algorithm>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"

#include "tree.h"
}

static const std::string funname = "ctree4";

static double toDouble(const int a)
{
    return static_cast<double>(a);
}

types::Function::ReturnValue sci_ctree4(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 5);
        return types::Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::Double* pIn1 = in[0]->getAs<types::Double>();
    if (pIn1->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    pIn1->convertToInteger();

    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }
    types::Double* pIn2 = in[1]->getAs<types::Double>();
    if (pIn2->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }
    pIn2->convertToInteger();

    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 3);
        return types::Function::Error;
    }
    types::Double* pIn3 = in[2]->getAs<types::Double>();
    if (pIn3->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 3);
        return types::Function::Error;
    }
    pIn3->convertToInteger();

    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 4);
        return types::Function::Error;
    }
    types::Double* pIn4 = in[3]->getAs<types::Double>();
    if (pIn4->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 4);
        return types::Function::Error;
    }
    pIn4->convertToInteger();

    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 5);
        return types::Function::Error;
    }
    types::Double* pIn5 = in[4]->getAs<types::Double>();
    if (pIn5->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 5);
        return types::Function::Error;
    }
    pIn5->convertToInteger();

    types::Double* pIn1Clone = pIn1->clone()->getAs<types::Double>();
    pIn1Clone->convertToInteger();
    types::Double* pIn2Clone = pIn2->clone()->getAs<types::Double>();
    pIn2Clone->convertToInteger();
    types::Double* pIn3Clone = pIn3->clone()->getAs<types::Double>();
    pIn3Clone->convertToInteger();
    types::Double* pIn4Clone = pIn4->clone()->getAs<types::Double>();
    pIn4Clone->convertToInteger();
    types::Double* pIn5Clone = pIn5->clone()->getAs<types::Double>();
    pIn5Clone->convertToInteger();

    int n   = pIn1Clone->getSize();
    int mnd = pIn4Clone->getCols();
    int nn  = pIn4Clone->getSize();

    int* ipr1 = new int[nn];
    int* ipr2 = new int[nn];
    int nr;
    ctree4((int*) pIn1Clone->getReal(), n, (int*) pIn4Clone->getReal(), mnd, (int*) pIn5Clone->getReal(), (int*) pIn2Clone->getReal(), (int*) pIn3Clone->getReal(), ipr1, ipr2, &nr);

    types::Double* pOut1;
    types::Double* pOut2;
    if (nr == 1)
    {
        pOut1 = new types::Double(static_cast<double>(ipr1[0]));
        pOut2 = new types::Double(static_cast<double>(ipr2[0]));
    }
    else
    {
        // More than 1 element to return in each 'pOut': copy int arrays into double arrays
        double* ipr1Double;
        pOut1 = new types::Double(1, nr, &ipr1Double);
        std::transform(ipr1, ipr1 + nr, ipr1Double, toDouble);
        double* ipr2Double;
        pOut2 = new types::Double(1, nr, &ipr2Double);
        std::transform(ipr2, ipr2 + nr, ipr2Double, toDouble);
    }

    out.push_back(pOut1);
    out.push_back(pOut2);

    pIn1Clone->DecreaseRef();
    pIn1Clone->killMe();
    pIn2Clone->DecreaseRef();
    pIn2Clone->killMe();
    pIn3Clone->DecreaseRef();
    pIn3Clone->killMe();
    pIn4Clone->DecreaseRef();
    pIn4Clone->killMe();
    pIn5Clone->DecreaseRef();
    pIn5Clone->killMe();

    return types::Function::OK;
}
