/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

Function::ReturnValue sci_sparse(typed_list &in, int _piRetCount, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    // per the scilab manual sparse will take upto 3 arguments but no less than one
    if ( (in.size() < 1) || (in.size() > 3) )
    {
        return Function::Error;
    }

    for(int i = 0 ; i < in.size() ; i++)
    {
        if(in[i]->getType() != InternalType::RealDouble && in[i]->getType() != InternalType::RealSparse)
        {
            return Function::Error;
        }
    }

    // if one argument is given, it will be a matrix of constant or sparse type, which will be converted into a sparse matrix
    if(in.size() == 1)
    {
        pRetVal = in[0]->getAsSparse()
            ?  new types::Sparse(*in[0]->getAsSparse())
            : (in[0]->getAsDouble()
               ? new types::Sparse(*in[0]->getAsDouble())
               :0);

        // if two arguments are given the first is a 'n x 2' matrix of the non zero indices and the second is a 'n x 1' vector of the values
    }else if((in.size() == 2) || (in.size() == 3))
    {
        for(int i = 0 ; i < in.size() ; i++)
        {
            if(in[i]->getType() != InternalType::RealDouble)
            {
                return Function::Error;
            }
        }

        Double& _indices = *in[0]->getAsDouble();
        Double& _vals    = *in[1]->getAsDouble();



        // check was overly strict: values can be in any shape
        if ( (_indices.rows_get() != _vals.size_get()) || (_indices.cols_get() != 2))
        {
            return Function::Error;
        }
        // indices must be >=1
        if(*std::min_element(_indices.real_get(), _indices.real_get() + _indices.size_get()) < 1)
        {
            return Function::Error;
        }

        if( in.size()==2)
        {
            pRetVal = new types::Sparse(_vals, _indices);
        }
        // if three arguments are given the first two are the same for the case of two arugments and the third is a '1 x 2' vector for the size of the matrix
        else
        {
            Double& _dims    = *(in[2]->getAsDouble());
            if ((_dims.rows_get() != 1) || (_dims.cols_get() != 2) )
            {
                return Function::Error;
            }
            double *  endOfRow(_indices.real_get() + _indices.rows_get());
            if(*std::max_element(_indices.real_get(), endOfRow) > _dims.real_get(0,0))
            {
                return Function::Error;
            }
            if(*std::max_element(endOfRow, endOfRow+ _indices.rows_get()) > _dims.real_get(0,1))
            {
                return Function::Error;
            }

            pRetVal = new types::Sparse(_vals, _indices, _dims);
        }
    }
    if(pRetVal == NULL)
    {
        return Function::Error;
    }

    out.push_back(pRetVal);

    return Function::OK;
}
