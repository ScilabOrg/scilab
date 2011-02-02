/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "arrayof.hxx"

namespace types
{
/*    template <typename T>
    ArrayOf<T>* create_empty_double()
    {
        return Double::Empty();
    }
*/

    int GetVarMaxDim(types::InternalType *_pIT, int _iCurrentDim, int _iMaxDim)
    {
        if(_pIT == NULL)
        {
            return 0;
        }

        int iDim    = 1;
        int iDims   = _pIT->getAs<ArrayOf<double> >()->get_dims();
        int* piDims = _pIT->getAs<ArrayOf<double> >()->get_dims_array();

        if(_iMaxDim != iDims)
        {//view as "vector", not all dimensions are used
            if(_iCurrentDim == (_iMaxDim - 1))
            {
                for(int i = _iCurrentDim ; i < iDims ; i++)
                {
                    iDim *= piDims[i];
                }
            }
            else
            {
                iDim = piDims[_iCurrentDim];
            }
        }
        else
        {//normal view, all dimensions are used
            iDim = piDims[_iCurrentDim];
        }

        return iDim;
    }

    //n-uplet in french
    int compute_tuples(int* _piCountDim, int _iDims, int _iCurrentDim, int* _piIndex)
    {
        //if bRet == 1, previous dims has reach max value.
        int iRet = 0;

        if(_iCurrentDim == 0)
        {//last dims
            if(_piIndex[_iCurrentDim] >= _piCountDim[_iCurrentDim])
            {
               _piIndex[_iCurrentDim] = 0;
               return 1;
            }
        }
        else
        {
            iRet = compute_tuples(_piCountDim, _iDims, _iCurrentDim - 1, _piIndex);
            if(iRet)
            {
                _piIndex[_iCurrentDim]++;
                if(_piIndex[_iCurrentDim] >=_piCountDim[_iCurrentDim])
                {
                   _piIndex[_iCurrentDim] = 0;
                   return 1;
                }
            }
        }
        return 0;
    }

    //check argument types and compute, dimensions, count of combinations, max indexes
    int check_indexes_arguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim)
    {
        int iDims       = static_cast<int>(_pArgsIn->size());
        int iSeqCount   = 1;
        bool bUndefine  = false;

        for(int i = 0 ; i < iDims ; i++)
        {
            InternalType* pIT = (*_pArgsIn)[i];
            if(pIT->getAs<Colon>() || pIT->getAs<ImplicitList>())
            {//: or a:b:c
                if(_pRef == NULL)
                {
                    _pArgsOut->push_back(NULL);
                    bUndefine = true;
                    continue;
                }

                ImplicitList* pIL = pIT->getAs<ImplicitList>();
                if(pIL->computable() == false)
                {//: or $
                    //evalute polynom with "MaxDim"
                    int iMaxDim = GetVarMaxDim(_pRef, i, iDims);
                    Double dbl(iMaxDim);
                    if(pIL->start_type_get() == InternalType::RealPoly)
                    {
                        MatrixPoly *poPoly	= pIL->start_get()->getAsPoly();
                        pIL->start_set(poPoly->evaluate(&dbl));
                    }
                    if(pIL->step_type_get() == InternalType::RealPoly)
                    {
                        MatrixPoly *poPoly	= pIL->step_get()->getAsPoly();
                        pIL->step_set(poPoly->evaluate(&dbl));
                    }
                    if(pIL->end_type_get() == InternalType::RealPoly)
                    {
                        MatrixPoly *poPoly	= pIL->end_get()->getAsPoly();
                        pIL->end_set(poPoly->evaluate(&dbl));
                    }
                }

                _pArgsOut->push_back(pIL->extract_matrix());
            }
            else if(pIT->getAs<String>())
            {//see later for extract from struct or Tlist
            }
            else if(pIT->getAs<MatrixPoly>())
            {//$
                MatrixPoly* pMP = pIT->getAs<MatrixPoly>();
                int iMaxDim     = GetVarMaxDim(_pRef, i, iDims);
                Double dbl(iMaxDim); // $
                _pArgsOut->push_back(pMP->evaluate(&dbl));
            }
            else if(pIT->getAs<Bool>())
            {//[T F F T F]
                Bool *pB    = pIT->getAs<Bool>();
                int *piB    = pB->bool_get();

                //find true item count
                int iItemCount = 0;
                for(int j = 0 ; j < pB->size_get() ; j++)
                {
                    if(piB[j])
                    {
                        iItemCount++;
                    }
                }

                //allow new Double variable
                Double* pDbl    = new Double(iItemCount, 1);
                double* pdbl    = pDbl->get_real();

                int j = 0;
                for(int l = 0 ; l < pB->size_get() ; l++)
                {
                    if(piB[l])
                    {
                        pdbl[j++] = l + 1;
                    }
                }
                _pArgsOut->push_back(pDbl);
            }
            else
            {//Double
                if(pIT->getAs<Double>()->isEmpty())
                {
                    return 0;
                }

                _pArgsOut->push_back(pIT);
            }

            _piCountDim[i] = (*_pArgsOut)[i]->getAs<GenericType>()->size_get();
            _piMaxDim[i] = 0;
            for(int j = 0 ; j < _piCountDim[i] ; j++)
            {
                _piMaxDim[i] = Max(_piMaxDim[i], static_cast<int>((*_pArgsOut)[i]->getAs<Double>()->get(j)));
            }

            iSeqCount *= _piCountDim[i];
        }

        //returns a negative value if at least one parameter is undefined
        //case with : or $ for creation by insertion
        return (!bUndefine ? iSeqCount : - iSeqCount);
    }

    InternalType* create_empty_double()
    {
        return Double::Empty();
    }

    InternalType* create_double_vector(int _iSize)
    {
        int piDims[] = {1, _iSize};
        Double* pOut = new Double(2, piDims);
        for(int i = 0 ; i < _iSize ; i++)
        {
            pOut->set(i, i + 1);
        }
        return pOut;
    }
}
