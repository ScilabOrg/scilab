/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "tclsci_gw.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "function.hxx"

extern "C"
{
#include "gw_tclsci.h"
}

#define MODULE_NAME L"tclsci"

int TclsciModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_DoOneEvent",   &sci_TCL_DoOneEvent,   MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_EvalStr",      &sci_TCL_EvalStr,      MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_GetVar",       &sci_TCL_GetVar,       MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_SetVar",       &sci_TCL_SetVar,       MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sci_opentk",       &sci_opentk,           MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_GetVersion",   &sci_TCL_GetVersion,   MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_UnsetVar",     &sci_TCL_UnsetVar,     MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_ExistVar",     &sci_TCL_ExistVar,     MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_UpVar",        &sci_TCL_UpVar,        MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_DeleteInterp", &sci_TCL_DeleteInterp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_ExistInterp",  &sci_TCL_ExistInterp,  MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_ExistArray",   &sci_TCL_ExistArray,   MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"TCL_EvalFile",     &sci_TCL_EvalFile,     MODULE_NAME));
    return 1;
}
