/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;
using namespace std;

namespace
{
    size_t nonZeros(types::Double CONST& d)
    {
        size_t res = 0;
        size_t n = d.getSize();

        if(d.isComplex())
        {
            for(size_t i = 0 ; i != n ; i++)
            {
                if(d.getReal()[i] == 0 && d.getImg()[i] == 0)
                {
                    ++res;
                }
            }
        }
        else
        {
            res = count(d.getReal(), d.getReal() + n, 0);
        }
        return d.getSize() - res;
    }
}

Function::ReturnValue sci_nnz(typed_list &in, int nbRes, typed_list &out)
{
    if(in.size() != 1)
    {
         Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "nnz", 1);
        return Function::Error;
    }

    if(in[0]->isSparse() == false && in[0]->isSparseBool() == false && in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type argument %d: Sparse or matrix expected.\n"), "nnz", 1);
        return Function::Error;
    }

    if(nbRes > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "nnz", 1);
        return Function::Error;
    }

    double dblVal = 0;



    if(in[0]->isSparse())
    {
        dblVal = static_cast<double>(in[0]->getAs<Sparse>()->nonZeros());
    }
    else if(in[0]->isSparseBool())
    {
        dblVal = static_cast<double>(in[0]->getAs<SparseBool>()->nbTrue());
    }
    else
    {
        dblVal = static_cast<double>(nonZeros(*in[0]->getAs<Double>()));
    }

    out.push_back(new Double(dblVal));
    return Function::OK;
}
