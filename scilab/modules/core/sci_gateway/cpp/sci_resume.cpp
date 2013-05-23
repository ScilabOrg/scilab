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

#include "core_gw.hxx"
#include "function.hxx"
#include "runner.hxx"
#include "threadId.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"

#include "Thread_Wrapper.h"
}

types::Function::ReturnValue sci_resume(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("!!!!!!!!!!!!  NEVER OCCUR  !!!!!!!!!!!!\n");
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "resume", 0);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "resume", 0, 1);
        return types::Function::Error;
    }

    ThreadId* pThreadId = ConfigVariable::getLastPausedThread();
    if (pThreadId == NULL)
    {
        //no paused thread, so just go leave
        return types::Function::OK;
    }

    __threadId id = pThreadId->getId();
    pThreadId->resume();
    __WaitThreadDie(id);

    return types::Function::OK;
}
