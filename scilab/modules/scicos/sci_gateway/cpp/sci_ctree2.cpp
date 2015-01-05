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

const std::string funname = "ctree2";

static double toDouble(const int a)
{
    return static_cast<double>(a);
}

types::Function::ReturnValue sci_ctree2(types::typed_list &in, int _iRetCount, types::typed_list &out)
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

    if (in[0]->isDouble() == false)
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

    // Copy and convert the input parameters into int arrays
    int n = pIn1->getSize();
    int* pIn1Copy = new int[n];
    for (int i = 0; i < n; ++i)
    {
        pIn1Copy[i] = static_cast<int>(pIn1->get(i));
    }
    int* pIn2Copy = new int[pIn2->getSize()];
    for (int i = 0; i < pIn2->getSize(); ++i)
    {
        pIn2Copy[i] = static_cast<int>(pIn2->get(i));
    }
    int* pIn3Copy = new int[pIn3->getSize()];
    for (int i = 0; i < pIn3->getSize(); ++i)
    {
        pIn3Copy[i] = static_cast<int>(pIn3->get(i));
    }
    int* pIn4Copy = new int[pIn4->getSize()];
    for (int i = 0; i < pIn4->getSize(); ++i)
    {
        pIn4Copy[i] = static_cast<int>(pIn4->get(i));
    }
    int* pIn5Copy = new int[pIn5->getSize()];
    for (int i = 0; i < pIn5->getSize(); ++i)
    {
        pIn5Copy[i] = static_cast<int>(pIn5->get(i));
    }

    int* ipord = new int[n];
    int nord;
    int ipok;
    ctree2(pIn1Copy, n, pIn4Copy, pIn5Copy, pIn2Copy, pIn3Copy, ipord, &nord, &ipok);

    types::Double* pOut1;
    if (nord == 1)
    {
        pOut1 = new types::Double(static_cast<double>(ipord[0]));
    }
    else
    {
        // More than 1 element to return in 'pOut1': copy (int*) 'ipord' into a double array
        double* ipordDouble;
        pOut1 = new types::Double(nord, 1, &ipordDouble);
        std::transform(ipord, ipord + nord, ipordDouble, toDouble);
    }
    types::Double* pOut2 = new types::Double(static_cast<double>(ipok));

    out.push_back(pOut1);
    out.push_back(pOut2);

    delete[] pIn1Copy;
    delete[] pIn2Copy;
    delete[] pIn3Copy;
    delete[] pIn4Copy;
    delete[] pIn5Copy;
    delete[] ipord;

    return types::Function::OK;
}
