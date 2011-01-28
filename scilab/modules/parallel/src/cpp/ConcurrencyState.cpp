/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcurrencyState.hxx"
#include "Thread_Wrapper.h" /* locks for concurrency access */

#ifdef _MSC_VER
#include "mmapWindows.h"
#else
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif
#endif

namespace parallel
{
    ConcurrencyState concurrencyState;

    ConcurrencyState::ConcurrencyState()
    {
        lock = static_cast<__threadSignalLock*>(mmap(0, sizeof( __threadSignalLock), PROT_READ | PROT_WRITE,MAP_SHARED |  MAP_ANONYMOUS, -1, 0));
        processConcurrencyLevelPtr= static_cast<int*>(mmap(0, sizeof(int), PROT_READ | PROT_WRITE,MAP_SHARED |  MAP_ANONYMOUS, -1, 0));
        __InitSignalLock(lock);
    }

    ConcurrencyState::ScopedUpdater::ScopedUpdater(ConcurrencyState& c, bool threads)
        : lock(c.lock), countPtr( threads
                                  ? &c.threadConcurrencyLevel
                                  : c.processConcurrencyLevelPtr)
    {
        __LockSignal(lock);
        ++(*countPtr);
        __UnLockSignal(lock);
    }

    ConcurrencyState::ScopedUpdater::~ScopedUpdater()
    {
        __LockSignal(lock);
        --(*countPtr);
        __UnLockSignal(lock);
    }
    ConcurrencyState::ScopedUpdater ConcurrencyState::scopedUpdater(bool threads)
    {
        return ConcurrencyState::ScopedUpdater(*this, threads);
    }

    int ConcurrencyState::get() const
    {
        __LockSignal(lock);
        int res((threadConcurrencyLevel ? 1 : 0) | (*processConcurrencyLevelPtr ? 2 : 0));
        __UnLockSignal(lock);
        return res;
    }

    ConcurrencyState::~ConcurrencyState()/* called by exit() for static variables */
    {
        __UnLockSignal(lock);
    }
}
