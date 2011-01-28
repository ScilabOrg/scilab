/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "parallel_gw.hxx"
#include "function.hxx"
#define MODULE_NAME L"parallel"

bool ParallelModule::Load()
{
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"parallel_run"
                                                                                , &sci_parallel_run, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"parallel_concurrency"
                                                                                , &sci_parallel_concurrency, MODULE_NAME));
	return true;
}

