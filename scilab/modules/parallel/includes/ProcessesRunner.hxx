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
#ifndef PROCESSESRUNNER_HXX
#define PROCESSESRUNNER_HXX

#include <vector>

#include "SharedData.hxx"
#include "ParallelRunner.hxx"
#include "parallel_utility.hxx"
/*
  Our variables should use (standard) containers with allocators, and we would use
  a Boost.Interprocess shared memory allocator to perform safe/efficient/easy sharing of the args / results.
  The processes and threads workers code would then be very similar (and could be shared with templates).

  For now, we migrate variables across memory spaces using SharedData.


  Note: For writing to slices from args slices, we can always (shared memory or not) use the simple shared memory model, because
  fork() implements a copy on write, giving each forked process a copy of the arguments.

  Windows specific: However, an hypothetical windows separate meomry model implementation that would spawn a new pristine Scilab
  process would have to use shared memory for both arguments and results. Note that it would not have any environnement available,
  but the master process could store the environement (with hdf5) and the slave processes would load it with a proper prologue
  (as we pay the performance price of dynamic scoping, we might as well use it).

*/
#include "SharedData.hxx"

namespace parallel
{
    struct ProcessesRunner : ParallelRunner
    {
        /* used to represent a chunk of work (interval of argument slices) to handle by worker processes. */
        typedef std::pair<std::size_t, std::size_t> workshare_t;

        /* ProcessRunner constructor, allocates the shared data for inter process communication of results. */

        template<typename In>
        ProcessesRunner(types::typed_list& in, std::size_t nbRes, types::typed_list& out
                        , std::size_t n_, std::size_t nbArgs_, std::size_t chunkSize_, std::size_t nbWorkers_
                        , bool dynamic_, CallablePtr function_, CallablePtr prologue_, CallablePtr epilogue_, In sizesBegin, In sizesEnd)
            : ParallelRunner(in, nbRes, out, n_, nbArgs_, chunkSize_, nbWorkers_, dynamic_, function_, prologue_, epilogue_, sizesBegin, sizesEnd)
            , todo(allocSharedMem<std::size_t>()), out_of_work(0)
            , todo_protect(1), sharedBuffer(nbRes)
        {
            std::transform(out.begin(), out.end(), sharedBuffer.begin(), &SharedData::create);
            *todo= 0;
        }

        void run();
        /* destroy/ free semaphores/shared memory, and restores original w.res[] values. */
        ~ProcessesRunner();

    private:
        /* copy slices to shared memory buffer
         * @param b iterator to the beginning  of a sequence of (n) boolean saying if the slice should
         * be copied (because it was handeled by the current process).
         * Each process calls the member function after working to share its results.
         */
        template<typename In>
        void copyToSharedMemory(In b)
        {
            for (std::size_t var(0); var != nbRes; ++var)
            {
                sharedBuffer[var]->readSlices(b, b+n, *out[var]);
            }
        }
        /* copy slices from shared memory buffer
         * called by parent process after checking that every process has finished and copied its results
         * into the shared memory.
         */
        void copyFromSharedMemory();

    private:
        /* compute initial workshares. no need to synch because we did not fork() yet
         * @param p process id from 0(parent) -> nb_process-1
         */
        workshare_t initialWork( std::size_t p) const ;
        /* computes next workshare with protected access to shared *todo value  */
        workshare_t getWork();

        std::size_t* todo; /* shared value of the next slice to process, used to compute the next chunk of slices to give to a process */
        PortableSemaphore out_of_work, todo_protect; /* interprocess synch to signal that all work is done, that a new chunk of slices is being given */
        std::vector<SharedData*> sharedBuffer; /* ptrs to shared memory buffers for Scilab variables data */
    };
}
#endif
