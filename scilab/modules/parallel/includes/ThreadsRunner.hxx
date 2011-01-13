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
    struct ThreadsRunner : ParallelRunner
    {
        ThreadsRunner(types::typed_list& in, std::size_t nbRes, types::typed_list& out)
            : ParallelRunner(in, nbRes, out) { }

        void run();
    private:
        void doCall(types::typed_list& passIn, types::typed_list& getOut, std::size_t i, ast::ExecVisitor& exec);
    };
}
#endif
