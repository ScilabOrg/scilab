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
#ifndef CONCURRENCY_STATE_HXX
#define CONCURRENCY_STATE_HXX

#include "Thread_Wrapper.h" /* locks for concurrency access */

namespace parallel
{
    /*
       This class manages the concurrency information about paralle execution (threads and / or processes).
       In order to ensure correct information, a ScopedUpdater object is used to update the status,
       its destructor reverting the update.
       A single instance of named concurrencyState is used as follows :

       - When launching paralle executions :
       parallel::ConcurrencyState::ScopedUpdater u(parallel::concurrencyState.scopedUpdater(withThreads));
       withThreads being true when launching parallel executions with threads, and false when using processes.

       - When querying for parallel state :
       parallel::concurrencyState.get() return a mask with bit 0 for parallel threads and bit 1 for parallel processes.
     */
    struct ConcurrencyState
    {
        ConcurrencyState();

        struct ScopedUpdater
        {
            ScopedUpdater(ConcurrencyState& c, bool threads);
            ~ScopedUpdater();
        private:
            __threadSignalLock* lock;
            int* countPtr;
        };
        ScopedUpdater scopedUpdater(bool threads);

        int get() const;

        ~ConcurrencyState();/* called by exit() for static variables */
        private:
        __threadSignalLock* lock;
        int* processConcurrencyLevelPtr; /* must be in shared mem */
        int threadConcurrencyLevel;
    };

    extern ConcurrencyState concurrencyState;

}

#endif
