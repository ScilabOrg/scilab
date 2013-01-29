/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "boolean_gw.hxx"
#include "context.hxx"
#include "function.hxx"

#define MODULE_NAME L"boolean"
extern "C"
{
#include "gw_boolean.h"
}

using namespace types;

int BooleanModule::Load()
{
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"or", &sci_or, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"and", &sci_and, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"bool2s", &sci_bool2s, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"find", &sci_find, MODULE_NAME));
    return 1;
}

