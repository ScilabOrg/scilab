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

#include "double.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "ConcurrencyState.hxx"
/* function r=f(i); r=parallel_concurrency(); endfunction;
   parallel_run(1,f, tlist(["params","shared_memory"],1))
   parallel_run(1,f, tlist(["params","shared_memory"],0))
 */
types::Function::ReturnValue sci_parallel_concurrency(types::typed_list &in, int nbRes, types::typed_list &out)
{
    types::Double* d(new types::Double(1, 1));
    d->val_set(0, 0, static_cast<double>(parallel::concurrencyState.get()));
    out.push_back(d);
    return types::Function::OK;
}
