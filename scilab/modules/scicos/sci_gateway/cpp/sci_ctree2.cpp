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

static const std::string funname = "ctree2";

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


    int nvec = pIn1->getRows();
    int mvec = pIn1->getCols();
    int n = nvec * mvec;

    double* ipord = new double[n];
    int nord;
    int ipok;
    ctree2((int*) pIn1->getReal(), n, (int*) pIn4->getReal(), (int*) pIn5->getReal(), (int*) pIn2->getReal(), (int*) pIn3->getReal(), (int*) ipord, &nord, &ipok);

    types::Double* pOut1 = new types::Double(nord, 1, &ipord);
    types::Double* pOut2 = new types::Double(static_cast<double>(ipok));

    out.push_back(pOut1);
    out.push_back(pOut2);

    //int one = 1, ipvec = 0, nvec = 0, mvec = 0, noin = 0, moin = 0, ipoin = 0, noinr = 0, moinr = 0, ipoinr = 0;
    //int ndep = 0, mdep = 0, ipdep = 0, ndepuptr = 0, mdepuptr = 0, ipdepuptr = 0, ipord = 0, ipok = 0, n = 0, nord = 0;

    //CheckRhs(5, 5);
    //CheckLhs(2, 2);

    //GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &nvec, &mvec, &ipvec);
    //GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &noin, &moin, &ipoin);
    //GetRhsVar(3, MATRIX_OF_INTEGER_DATATYPE, &noinr, &moinr, &ipoinr);
    //GetRhsVar(4, MATRIX_OF_INTEGER_DATATYPE, &ndep, &mdep, &ipdep);
    //GetRhsVar(5, MATRIX_OF_INTEGER_DATATYPE, &ndepuptr, &mdepuptr, &ipdepuptr);
    //n = nvec * mvec;
    //CreateVar(6, MATRIX_OF_INTEGER_DATATYPE, &n, &one, &ipord);
    //CreateVar(7, MATRIX_OF_INTEGER_DATATYPE, &one, &one, &ipok);

    //ctree2(istk(ipvec), n, istk(ipdep), istk(ipdepuptr), istk(ipoin), istk(ipoinr), istk(ipord), &nord, istk(ipok));

    //*istk(iadr(C2F(intersci).iwhere[5]) + 1) = nord;

    return types::Function::OK;
}
