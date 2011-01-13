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

#include "internal.hxx" // types::typed_list
#include "function.hxx" // return values
#include "ParallelRunner.hxx"

/*
  parallel_run maps parallel computations on slices of argument matrices and aggregates the results in result matrices.
*/


Function::ReturnValue sci_parallel_run(types::typed_list &in, int nbRes, types::typed_list &out)
{
    // args validations and parameters extraction done by the constructor
    parallel::ParallelRunner* runner(parallel::ParallelRunner::create(in, nbRes, out));
    if (!runner) // would be cleaner if the constructor threw an exception with error message
    {
        std::cerr<<"illegal Args!\n";
        return Function::Error;
    }
    runner->run();
    return Function::OK;
}
