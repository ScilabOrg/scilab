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

#include <iostream>

using namespace types;
#define CONST
//[xadj,adjncy,anz]= sp2adj(
Function::ReturnValue sci_adj2sp(typed_list &in, int nbRes, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if ( ( in.size() > 4 || in.size()<3) || nbRes != 1 )
    {
        return Function::Error;
    }
    for(std::size_t i(0); i!= in.size(); ++i)
    {
        if(in[i]->getType() != InternalType::RealDouble || ( i < 3 && in[i]->getAsDouble()->cols_get() != 1))
        {
            return Function::Error;
        }
    }

    types::Double CONST& xadj(*(in[0]->getAsDouble()));
    types::Double CONST& adjncy(*(in[1]->getAsDouble()));
    types::Double CONST& anz(*(in[2]->getAsDouble()));

    std::size_t n(xadj.rows_get()-1),m (*std::max_element(adjncy.real_get(), adjncy.real_get()+adjncy.size_get()));

    if(in.size()==4)
    {
        double* data;
        Double& dims    = *(in[3]->getAsDouble());
        if ((dims.rows_get() != 1) || (dims.cols_get() != 2) )
        {
            std::cerr<<"dims dim not corect\n";
            return Function::Error;
        }
        if(n > dims.real_get(0,0))
        {
            return Function::Error;
        }
        if(m > dims.real_get(0,1))
        {
            return Function::Error;
        }
        n= dims.real_get(0,0);
        m= dims.real_get(0,1);
    }
    out.push_back(new types::Sparse(xadj, adjncy, anz, m, n));
    return Function::OK;
}
