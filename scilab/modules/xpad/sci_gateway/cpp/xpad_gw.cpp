/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */
#include "xpad_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME "xpad"
extern "C"
{
   #include "gw_xpad.h"
}

using namespace types;

bool XpadModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction("xpad", &sci_xpad, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("editor", &sci_xpad, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("closeEditor", &sci_closeXpadFromScilab, MODULE_NAME));
    return true;
}
