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
    ArrayOf<T>* createEmptyDouble()
    {
        return Double::Empty();
    }
*/
    //n-uplet in french
    int computeTuples(int* _piCountDim, int _iDims, int _iCurrentDim, int* _piIndex)
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
            iRet = computeTuples(_piCountDim, _iDims, _iCurrentDim - 1, _piIndex);
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
    int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim)
    {
        int iDims       = static_cast<int>(_pArgsIn->size());
        int iSeqCount   = 1;
        bool bUndefine  = false;

        for(int i = 0 ; i < iDims ; i++)
        {
            InternalType* pIT = (*_pArgsIn)[i];
            if(pIT->isColon() || pIT->isImplicitList())
            {//: or a:b:c
                if(_pRef == NULL)
                {
                    _pArgsOut->push_back(NULL);
                    bUndefine = true;
                    continue;
                }

                ImplicitList* pIL = pIT->getAs<ImplicitList>();
                if(pIL->isComputable() == false)
                {//: or $
                    //evalute polynom with "MaxDim"
                    int iMaxDim = _pRef->getAsGenericType()->getVarMaxDim(i, iDims);
                    Double dbl(iMaxDim);
                    if(pIL->getStart()->isPoly())
                    {
                        MatrixPoly *poPoly	= pIL->getStart()->getAsPoly();
                        pIL->setStart(poPoly->evaluate(&dbl));
                    }
                    if(pIL->getStep()->isPoly())
                    {
                        MatrixPoly *poPoly	= pIL->getStep()->getAsPoly();
                        pIL->setStep(poPoly->evaluate(&dbl));
                    }
                    if(pIL->getEnd()->isPoly())
                    {
                        MatrixPoly *poPoly	= pIL->getEnd()->getAsPoly();
                        pIL->setEnd(poPoly->evaluate(&dbl));
                    }
                }

                _pArgsOut->push_back(pIL->extractFullMatrix());
            }
            else if(pIT->getAs<types::String>())
            {//see later for extract from struct or Tlist
            }
            else if(pIT->getAs<MatrixPoly>())
            {//$
                MatrixPoly* pMP = pIT->getAs<MatrixPoly>();
                int iMaxDim     = _pRef->getAsGenericType()->getVarMaxDim(i, iDims);
                Double dbl(iMaxDim); // $
                _pArgsOut->push_back(pMP->evaluate(&dbl));
            }
            else if(pIT->getAs<Bool>())
            {//[T F F T F]
                Bool *pB    = pIT->getAs<Bool>();
                int *piB    = pB->bool_get();

                //find true item count
                int iItemCount = 0;
                for(int j = 0 ; j < pB->getSize() ; j++)
                {
                    if(piB[j])
                    {
                        iItemCount++;
                    }
                }

                //allow new Double variable
                Double* pDbl    = new Double(iItemCount, 1);
                double* pdbl    = pDbl->getReal();

                int j = 0;
                for(int l = 0 ; l < pB->getSize() ; l++)
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

            _piCountDim[i] = (*_pArgsOut)[i]->getAs<GenericType>()->getSize();
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

    InternalType* createEmptyDouble()
    {
        return Double::Empty();
    }

    int getIntValueFromDouble(InternalType* _pIT, int _iPos)
    {
        return static_cast<int>(_pIT->getAs<Double>()->get(_iPos));
    }

    double* getDoubleArrayFromDouble(InternalType* _pIT)
    {
        return _pIT->getAs<Double>()->get();
    }

    InternalType* createDoubleVector(int _iSize)
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
