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

#include "ThreadsRunner.hxx"
#include "ConcurrencyState.hxx"

#include <omp.h>


#ifdef TRACE
#include <iostream>
#endif

#include "context.hxx"

namespace parallel
{
    void ThreadsRunner::run(){
        omp_set_num_threads(nbWorkers);
        parallel::ConcurrencyState::ScopedUpdater u(parallel::concurrencyState.scopedUpdater(true));

#pragma omp parallel
        {
            int const id(omp_get_thread_num());
            symbol::Context::initThreadedContext();
            types::typed_list passIn, getOut;
            createSlices(in.begin(), in.begin()+nbArgs, std::back_inserter(passIn));
            ast::ExecVisitor exec;
            call(prologue, id, exec);
#pragma omp barrier // wait for each thread to have the correct init context

            std::size_t i(0);
            if(dynamic)
            {
#pragma omp for private(i, exec) schedule(dynamic, chunkSize)
                for (i =0; i < n; ++i)
                {
                    doCall(passIn, getOut, i, exec);
                }
            }
            else
            {
#pragma omp for private(i, exec) schedule(static, chunkSize)
                for (i =0; i < n; ++i)
                {
                    doCall(passIn, getOut, i, exec);
                }
            }
            deleteSlices(passIn);
            call(epilogue, id, exec);
        }
    }
    void ThreadsRunner::doCall(types::typed_list& passIn, types::typed_list& getOut, std::size_t i, ast::ExecVisitor& exec)
    {
#ifdef TRACE
#pragma omp critical
        std::cerr<<"in thread "<<omp_get_thread_num()<<" slice "<<i<<" of "<<n<<std::endl;
#endif
        writeSlices(i, passIn);
        getOut.clear();
        function->call(passIn, nbRes, getOut, &exec);
        readSlices(i, getOut);
    }

}
