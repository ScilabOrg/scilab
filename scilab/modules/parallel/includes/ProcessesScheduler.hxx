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
#ifndef PROCESSESSCHEDULER_HXX
#define PROCESSESSCHEDULER_HXX

#include <vector>

#include "SharedData.hxx"
#include "ParallelRunner.hxx"
#include "parallel_utility.hxx"
/*
  Our variables should use (standard) containers with allocators, and we would use
  a Boost.Interprocess shared memory allocator to perform safe/efficient/easy sharing of the args / results.
  The processes and threads workers code would then be very similar (and could be shared with templates).

  For now, we migrate variables across memory spaces using SharedData.
*/
#include "SharedData.hxx"

namespace parallel
{
    struct ProcessesScheduler
    {
        typedef std::pair<std::size_t, std::size_t> workshare_t;

        explicit ProcessesScheduler(ParallelRunner& p);

        void operator()();

        template<typename In>
        void copyToSharedMemory(In b)
        {
            for (std::size_t var(0); var != pr.nbRes; ++var)
            {
                sharedBuffer[var]->readSlices(b, b+pr.n, *pr.out[var]);
            }
        }

        void copyFromSharedMemory();

        /* destroy/ free semaphores/shared memory, and restores original w.res[] values. */
        ~ProcessesScheduler();
    private:
        /* compute initial workshares. no need to synch because we did not fork() yet
         * @param p process id from 0(parent) -> nb_process-1
         */
        workshare_t initialWork( std::size_t p) const ;
        /* computes next workshare with protected access to shared *todo value  */
        workshare_t getWork();

        ParallelRunner& pr;
        std::size_t* todo;
        PortableSemaphore out_of_work, todo_protect;
        std::vector<SharedData*> sharedBuffer;
    };
}
#endif
