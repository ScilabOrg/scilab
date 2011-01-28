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
#ifndef THREADSRUNNERER_HXX
#define THREADSRUNNERER_HXX

#include "ParallelRunner.hxx"

namespace parallel
{

/*
   ThreadsRunner implements paralle execution of Scilab code (macros, functions or foreign compiled functions) in OpenMP threads
   To avoid costly synchronization of variables access, we forbid side effects so each
*/
    struct ThreadsRunner : ParallelRunner
    {
        template<typename In>
        ThreadsRunner(types::typed_list& in, std::size_t nbRes, types::typed_list& out
                      , std::size_t n_, std::size_t nbArgs_, std::size_t chunkSize_, std::size_t nbWorkers_
                      , bool dynamic_, CallablePtr function_, CallablePtr prologue_, CallablePtr epilogue_, In sizesBegin, In sizesEnd)
            : ParallelRunner(in, nbRes, out, n_, nbArgs_, chunkSize_, nbWorkers_, dynamic_, function_, prologue_, epilogue_, sizesBegin, sizesEnd) { }

        void run();
    private:
        void doCall(types::typed_list& passIn, types::typed_list& getOut, std::size_t i, ast::ExecVisitor& exec);
    };
}
#endif
