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
//[ij,v,mn]=spget(
Function::ReturnValue sci_spget(typed_list &in, int nbRes, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if ( in.size() != 1 || in[0]->getType() != InternalType::RealSparse || nbRes > 3)
    {
        return Function::Error;
    }
    types::Sparse CONST& sp(*(in[0]->getAsSparse()));
    std::size_t const nonZeros(sp.nonZeros());
    double* data;
    using types::Double;
    Double* res;
    if(nbRes >=1 )
    {
        res= new Double(nonZeros, 2, &data);
        sp.outputRowCol(data);
        out.push_back(res);
    }
    if(nbRes >= 2)
    {
        double* dataImag;
        res= sp.isComplex()
            ? new Double(nonZeros, 1, &data, &dataImag)
            : new Double(nonZeros, 1, &data );
        sp.outputValues(data, dataImag);
        out.push_back(res);
    }
    if(nbRes == 3)
    {
        res= new Double(1, 2, &data);
        data[0]= sp.rows_get();
        data[1]= sp.cols_get();
        out.push_back(res);
    }
    return Function::OK;
}
