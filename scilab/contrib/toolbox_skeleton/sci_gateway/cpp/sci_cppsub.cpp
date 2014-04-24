/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "function.hxx"
#include "double.hxx"
#include "Scierror.h"
#include "localization.h"

extern "C"
{
#include "csub.h"
}

/* ==================================================================== */
types::Function::ReturnValue sci_cppsub(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /* --> result = csub(3,8)

    /* check that we have only 2 input arguments */
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cpp_sub", 2);
        return types::Function::Error;
    }

    /* check that we have only 1 output argument */
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cpp_sub", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "cpp_sub", 1);
        return types::Function::Error;
    }

    types::Double* pIn1 = in[0]->getAs<types::Double>();

    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "cpp_sub", 2);
        return types::Function::Error;
    }

    types::Double* pIn2 = in[1]->getAs<types::Double>();

    /* check size */
    if (pIn1->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "cpp_sub", 1);
        return types::Function::Error;
    }

    if (pIn2->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "cpp_sub", 2);
        return types::Function::Error;
    }

    types::Double* pOut = new types::Double(1, 1);
    /* call c function csub */
    csub(pIn1->get(), pIn2->get(), pOut->get());

    /* return result */
    out.push_back(pOut);

    return types::Function::OK;
}
/* ==================================================================== */

