/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "cacsd_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"cacsd"
extern "C"
{
#include "gw_cacsd.h"
}

int CacsdModule::Load()
{
    // C gateways
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"contr",   &sci_contr,     MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dhinf",   &sci_dhinf,     MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"hinf",    &sci_hinf,      MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"linmeq",  &sci_linmeq,    MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mucomp",  &sci_mucomp,    MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rankqr",  &sci_rankqr,    MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ricc",    &sci_ricc,      MODULE_NAME));

    // CPP gateways
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"arl2_ius",&sci_arl2_ius,  MODULE_NAME));

    return 1;
}
