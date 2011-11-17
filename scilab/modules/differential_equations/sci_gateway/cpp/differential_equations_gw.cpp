/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "context.hxx"
#include "function.hxx"
#include "differential_equations_gw.hxx"

#define MODULE_NAME L"differential_equations"

int DifferentialEquationsModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"ode", &sci_ode, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"odedc", &sci_odedc, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"intg", &sci_intg, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"int2d", &sci_int2d, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"int3d", &sci_int3d, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"bvode", &sci_bvode, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"feval", &sci_feval, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"impl", &sci_impl, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"dassl", &sci_dassl, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"dasrt", &sci_dasrt, MODULE_NAME));

    return true;
}
