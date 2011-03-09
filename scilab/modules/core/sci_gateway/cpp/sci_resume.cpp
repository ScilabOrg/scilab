/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "runner.hxx"
#include "threadId.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"

#include "Thread_Wrapper.h"
}

types::Function::ReturnValue sci_resume(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(77,_W("%ls: Wrong number of input argument(s): %d expected.\n"), L"resume", 1);
        return types::Function::Error;
    }

    if(_iRetCount > 1)
    {
        ScierrorW(78,_W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"resume", 0, 1);
        return types::Function::Error;
    }

    ThreadId* pThreadId = in[0]->getAs<ThreadId>();

    if (pThreadId == NULL)
    {
        ScierrorW(9999,_W("%ls: Wrong type of input argument(s): %ls expected.\n"), L"resume", L"ThreadId");
        return types::Function::Error;
    }

    pThreadId->resume();

    __WaitThreadDie(pThreadId->getId());

    return types::Function::OK;
}
