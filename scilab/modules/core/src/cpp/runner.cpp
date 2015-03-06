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

#include "runner.hxx"

__threadLock Runner::m_lock;
__threadSignal Runner::m_awakeScilab;
__threadSignalLock Runner::m_awakeScilabLock;

//__threadLock Runner::m_AstLock;
//__threadSignal Runner::m_AstLoopEnd;
//__threadSignalLock Runner::m_AstLoopEndLock;


using namespace ast;

void Runner::init()
{
    __InitSignal(&m_awakeScilab);
    __InitSignalLock(&m_awakeScilabLock);

    //    __InitSignal(&m_AstLoopEnd);
    //    __InitSignalLock(&m_AstLoopEndLock);
}

void *Runner::launch(void *args)
{
    bool bdoUnlock = false;
    //try to lock locker ( waiting parent thread register me )
    __Lock(&m_lock);
    //just release locker
    __UnLock(&m_lock);

    //exec !
    Runner *me = (Runner *)args;

    if (me->isInterruptible() == false)
    {
        // wait for interruptible exp execution ended
        //        __Lock(&m_AstLock);
    }

    try
    {
        me->getProgram()->accept(*(me->getVisitor()));
        //ConfigVariable::clearLastError();
    }
    catch (const ast::ScilabException& se)
    {
        //        __UnLock(&m_AstLock);
        // remove the last call from where in case pause/abort
        ConfigVariable::where_end();
        scilabErrorW(se.GetErrorMessage().c_str());
    }

    // reset error state when new prompt occurs
    ConfigVariable::resetError();

    __threadKey currentThreadKey = __GetCurrentThreadKey();

    //change thread status
    ThreadId* pThread = ConfigVariable::getThread(currentThreadKey);
    if (pThread->getStatus() != ThreadId::Aborted)
    {
        pThread->setStatus(ThreadId::Done);
        bdoUnlock = true;
    }

    //unregister thread
    ConfigVariable::deleteThread(currentThreadKey);

    if (me->isInterruptible() == false)
    {
        // wait for
        //        __Lock(&m_AstLock);
    }

    delete me;

    if (bdoUnlock)
    {
        UnlockPrompt();
    }


    return NULL;
}

void Runner::LockPrompt()
{
    __LockSignal(&m_awakeScilabLock);
    //free locker to release thread
    __UnLock(&m_lock);
    __Wait(&m_awakeScilab, &m_awakeScilabLock);
    __UnLockSignal(&m_awakeScilabLock);
}

void Runner::UnlockPrompt()
{
    __LockSignal(&m_awakeScilabLock);
    __Signal(&m_awakeScilab);
    __UnLockSignal(&m_awakeScilabLock);
}


void Runner::execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor, bool _isInterruptible)
{
    try
    {
        Runner *runMe = new Runner(_theProgram, _visitor, _isInterruptible);
        __threadKey threadKey;
        __threadId threadId;

        //init locker
        __InitLock(&m_lock);
        //        __InitLock(&m_AstLock);
        //lock locker
        __Lock(&m_lock);
        //launch thread but is can't really start since locker is locked
        __CreateThreadWithParams(&threadId, &threadKey, &Runner::launch, runMe);
        runMe->setThreadId(threadId);
        runMe->setThreadKey(threadKey);

        //register thread
        ConfigVariable::addThread(new ThreadId(threadId, threadKey));
        //free locker to release thread && wait and of thread execution
        LockPrompt();

        types::ThreadId* pExecThread = ConfigVariable::getThread(threadKey);
        if (pExecThread == NULL)
        {
            //call pthread_join to clean stack allocation
            __WaitThreadDie(threadId);
        }
    }
    catch (const ast::ScilabException& se)
    {
        throw se;
    }
}

void Runner::exec(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
{
    m_theProgram = _theProgram;
    m_visitor = _visitor;
    __CreateThreadWithParams(&m_threadId, &m_threadKey, &Runner::launch, this);
}
