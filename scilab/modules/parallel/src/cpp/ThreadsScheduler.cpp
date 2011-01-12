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

#include "ThreadsScheduler.hxx"

#include <omp.h>

#ifdef TRACE
#include <iostream>
#endif

#include "context.hxx"

namespace parallel
{
    ThreadsScheduler::ThreadsScheduler(ParallelRunner& p): pr(p) {}

    void ThreadsScheduler::operator()(){
        omp_set_num_threads(pr.nbWorkers);
#pragma omp parallel
        {
            int const id(omp_get_thread_num());
            symbol::Context::initThreadedContext();
            types::typed_list fIn, fOut;
            pr.createSlices(std::back_inserter(fIn));
            ast::ExecVisitor exec;
            pr.call(pr.prologue, id, exec);
#pragma omp barrier // wait for each thread to have the correct init context

            std::size_t i(0);
            if(pr.dynamic)
            {
#pragma omp for private(i, exec) schedule(dynamic, pr.chunkSize)
                for (i =0; i < pr.n; ++i)
                {
#ifdef TRACE
#pragma omp critical
                    std::cerr<<"in thread "<<id<<" slice "<<i<<" of "<<pr.n<<std::endl;
#endif
                    pr.writeSlices(i, fIn);
                    fOut.clear();
                    pr.fun->call(fIn, pr.nbRes, fOut, &exec);
                    pr.readSlices(i, fOut);
                }
            }
            else
            {
#pragma omp for private(i, exec) schedule(static, pr.chunkSize)
                for (i =0; i < pr.n; ++i)
                {
#ifdef TRACE
#pragma omp critical
                    std::cerr<<"in thread "<<id<<" slice "<<i<<" of "<<pr.n<<std::endl;
#endif
                    pr.writeSlices(i, fIn);
                    fOut.clear();
                    pr.fun->call(fIn, pr.nbRes, fOut, &exec);
                    pr.readSlices(i, fOut);
                }
            }
            pr.deleteSlices(fIn);
            pr.call(pr.epilogue, id, exec);
        }
    }
}
