/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Cedric DELAMARRE
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

//#define __DEBUG_LOCK
//#define __DEBUG_SIGNAL
# if defined(__DEBUG_SIGNAL) || defined(__DEBUG_LOCK)
#include <iostream>
# endif


#include "threadmanagement.hxx"

__threadLock ThreadManagement::m_StartLock;
__threadLock ThreadManagement::m_RunnerLock;
__threadLock ThreadManagement::m_ParseLock;
__threadLock ThreadManagement::m_StoreCommandLock;

__threadSignal ThreadManagement::m_ConsoleExecDone;
__threadSignalLock ThreadManagement::m_ConsoleExecDoneLock;

__threadSignal ThreadManagement::m_TCLExecDone;
__threadSignalLock ThreadManagement::m_TCLExecDoneLock;

__threadSignal ThreadManagement::m_AwakeRunner;
__threadSignalLock ThreadManagement::m_AwakeRunnerLock;

__threadSignal ThreadManagement::m_AstPending;
__threadSignalLock ThreadManagement::m_AstPendingLock;

__threadSignal ThreadManagement::m_StartPending;
__threadSignalLock ThreadManagement::m_StartPendingLock;

__threadSignal ThreadManagement::m_CommandStored;
__threadSignalLock ThreadManagement::m_CommandStoredLock;

__threadSignal ThreadManagement::m_EmptyQueue;
__threadSignalLock ThreadManagement::m_EmptyQueueLock;


void ThreadManagement::initialize()
{
    __InitLock(&m_RunnerLock);
    __InitLock(&m_StartLock);
    __InitLock(&m_ParseLock);
    __InitLock(&m_StoreCommandLock);

    __InitSignal(&m_AwakeRunner);
    __InitSignalLock(&m_AwakeRunnerLock);

    __InitSignal(&m_ConsoleExecDone);
    __InitSignalLock(&m_ConsoleExecDoneLock);

    __InitSignal(&m_TCLExecDone);
    __InitSignalLock(&m_TCLExecDoneLock);

    __InitSignal(&m_AstPending);
    __InitSignalLock(&m_AstPendingLock);

    __InitSignal(&m_StartPending);
    __InitSignalLock(&m_StartPendingLock);

    __InitSignal(&m_CommandStored);
    __InitSignalLock(&m_CommandStoredLock);

    __InitSignal(&m_EmptyQueue);
    __InitSignalLock(&m_EmptyQueueLock);

}

void ThreadManagement::LockStart(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "LockStart" << std::endl;
# endif // __DEBUG_LOCK

    __Lock(&m_StartLock);
}

void ThreadManagement::UnlockStart(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "UnlockStart" << std::endl;
# endif // __DEBUG_LOCK

    __UnLock(&m_StartLock);
}

void ThreadManagement::LockParser(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "LockParser" << std::endl;
# endif // __DEBUG_LOCK

    __Lock(&m_ParseLock);
}

void ThreadManagement::UnlockParser(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "UnlockParser" << std::endl;
# endif // __DEBUG_LOCK

    __UnLock(&m_ParseLock);
}

void ThreadManagement::LockStoreCommand(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "LockStoreCommand" << std::endl;
# endif // __DEBUG_LOCK
    __Lock(&m_StoreCommandLock);
}

void ThreadManagement::UnlockStoreCommand(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "UnlockStoreCommand" << std::endl;
# endif // __DEBUG_LOCK
    __UnLock(&m_StoreCommandLock);
}

void ThreadManagement::LockRunner(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "LockRunner" << std::endl;
# endif // __DEBUG_LOCK
    __Lock(&m_RunnerLock);
}

void ThreadManagement::UnlockRunner(void)
{
# ifdef __DEBUG_LOCK
    std::cout << "UnlockRunner" << std::endl;
# endif // __DEBUG_LOCK
    __UnLock(&m_RunnerLock);
}

void ThreadManagement::SendAstPendingSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendAstPendingSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_AstPendingLock);
    __Signal(&m_AstPending);
    __UnLockSignal(&m_AstPendingLock);
}

void ThreadManagement::WaitForAstPendingSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForAstPendingSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_AstPendingLock);
    __Wait(&m_AstPending, &m_AstPendingLock);
    __UnLockSignal(&m_AstPendingLock);
}

void ThreadManagement::SendConsoleExecDoneSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendConsoleExecDoneSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_ConsoleExecDoneLock);
    __Signal(&m_ConsoleExecDone);
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::WaitForConsoleExecDoneSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForConsoleExecDoneSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_ConsoleExecDoneLock);
    __Wait(&m_ConsoleExecDone, &m_ConsoleExecDoneLock);
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::SendTCLExecDoneSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendTCLExecDoneSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_TCLExecDoneLock);
    __Signal(&m_TCLExecDone);
    __UnLockSignal(&m_TCLExecDoneLock);
}

void ThreadManagement::WaitForTCLExecDoneSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForTCLExecDoneSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_TCLExecDoneLock);
    __Wait(&m_TCLExecDone, &m_TCLExecDoneLock);
    __UnLockSignal(&m_TCLExecDoneLock);
}

void ThreadManagement::SendAwakeRunnerSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendAwakeRunnerSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_AwakeRunnerLock);
    __Signal(&m_AwakeRunner);
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::WaitForAwakeRunnerSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForAwakeRunnerSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_AwakeRunnerLock);
    ThreadManagement::UnlockRunner();
    __Wait(&m_AwakeRunner, &m_AwakeRunnerLock);
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::SendStartPendingSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendStartPendingSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_StartPendingLock);
    __Signal(&m_StartPending);
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::WaitForStartPendingSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForStartPendingSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_StartPendingLock);
    ThreadManagement::UnlockStart();
    __Wait(&m_StartPending, &m_StartPendingLock);
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::SendCommandStoredSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendCommandStoredSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_CommandStoredLock);
    __Signal(&m_CommandStored);
    __UnLockSignal(&m_CommandStoredLock);
}

void ThreadManagement::WaitForCommandStoredSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForCommandStoredSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_CommandStoredLock);
    __Wait(&m_CommandStored, &m_CommandStoredLock);
    __UnLockSignal(&m_CommandStoredLock);
}

void ThreadManagement::SendEmptyQueueSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendEmptyQueueSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_EmptyQueueLock);
    __Signal(&m_EmptyQueue);
    __UnLockSignal(&m_EmptyQueueLock);
}

void ThreadManagement::WaitForEmptyQueueSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForEmptyQueueSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_EmptyQueueLock);
    __Wait(&m_EmptyQueue, &m_EmptyQueueLock);
    __UnLockSignal(&m_EmptyQueueLock);
}
