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

#include "alltypes.hxx"
#include "sparse.hxx"
#include "funcmanager.hxx"

using namespace types;
#define CONST
namespace
{
    std::size_t nonZeros(types::Double CONST& d)
    {
        std::size_t res, n(d.size_get());

        if(d.isComplex())
        {
            for(std::size_t i(0); i != n; ++i)
            {
                if(d.real_get()[i] = 0. && d.img_get()[i] == 0.)
                {
                    ++res;
                }
            }
        }
        else
        {
            res= std::count(d.real_get(), d.real_get()+n, 0.);
        }
        return d.size_get() - res;
    }
}

Function::ReturnValue sci_nnz(typed_list &in, int nbRes, typed_list &out)
{

    if ( in.size() != 1 || (in[0]->getType() != InternalType::RealSparse && in[0]->getType() != InternalType::RealDouble) || nbRes > 1)
    {
        return Function::Error;
    }
    double* data;
    Double* res(new Double(1, 1, &data));
    *data= static_cast<double>( in[0]->getType()== InternalType::RealSparse
                                ? in[0]->getAsSparse()->nonZeros()
                                : nonZeros(*in[0]->getAsDouble()));
    out.push_back(res);

    return Function::OK;
}
