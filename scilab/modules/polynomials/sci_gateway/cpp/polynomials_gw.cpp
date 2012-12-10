/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "polynomials_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"polynomials"
extern "C"
{
#include "gw_polynomials.h"
}

int PolynomialsModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"poly", &sci_poly, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"varn", &sci_varn, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"roots", &sci_roots, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"coeff", &sci_coeff, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"degree", &sci_degree, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"pppdiv", &sci_pppdiv, MODULE_NAME));
    return 1;
}

