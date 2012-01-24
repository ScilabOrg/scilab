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
#include "scilabexception.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"

#include "Thread_Wrapper.h"
}

types::Function::ReturnValue sci_pause(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() != 0)
    {
        ScierrorW(77,_W("%ls: Wrong number of input argument(s): %d expected.\n"), L"pause", 0);
        return types::Function::Error;
    }

    ConfigVariable::IncreasePauseLevel();
    Runner::UnlockPrompt();
    ThreadId* pThread = ConfigVariable::getLastRunningThread();
    pThread->suspend();

    // Running from here means we have been awaken by some resume / abort
    ConfigVariable::DecreasePauseLevel();
    if (pThread->getStatus() == types::ThreadId::Aborted)
    {
        throw InternalAbort();
    }

    return types::Function::OK;

}
