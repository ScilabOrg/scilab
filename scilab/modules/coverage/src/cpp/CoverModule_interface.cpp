/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "CoverModule.hxx"
#include "callable.hxx"

extern "C"
{
#include "CoverModule_interface.h"
}

void CoverModule_invoke(void* call)
{
    types::Callable* c = (types::Callable*)call;
    if (coverage::CoverModule::getInstance())
    {
        coverage::CoverModule::getInstance()->invoke(c);
    }
}

void CoverModule_invokeAndStartChrono(void* exp)
{
    ast::Exp* e = (ast::Exp*)exp;
    coverage::CoverModule::invokeAndStartChrono(*e);
}

void CoverModule_stopChrono(void* exp)
{
    ast::Exp* e = (ast::Exp*)exp;
    coverage::CoverModule::stopChrono(*e);
}

