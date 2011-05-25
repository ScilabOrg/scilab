/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "alltypes.hxx"
#include "types_tools.hxx"

namespace types
{
    //check argument types and compute, dimensions, count of combinations, max indexes
    int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim)
    {
        int iDims       = static_cast<int>(_pArgsIn->size());
        int iSeqCount   = 1;
        bool bUndefine  = false;
        for(int i = 0 ; i < iDims ; i++)
        {
            InternalType* pIT = (*_pArgsIn)[i];
            Double *pCurrentArg = NULL;
            
            if(pIT->isDouble())
            {
                pCurrentArg = pIT->getAs<Double>();
                if(pCurrentArg->isEmpty())
                {
                    return 0;
                }

            }
            else if(pIT->isColon() || pIT->isImplicitList())
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
                        Polynom *poPoly	= pIL->getStart()->getAs<types::Polynom>();
                        pIL->setStart(poPoly->evaluate(&dbl));
                    }
                    if(pIL->getStep()->isPoly())
                    {
                        Polynom *poPoly	= pIL->getStep()->getAs<types::Polynom>();
                        pIL->setStep(poPoly->evaluate(&dbl));
                    }
                    if(pIL->getEnd()->isPoly())
                    {
                        Polynom *poPoly	= pIL->getEnd()->getAs<types::Polynom>();
                        pIL->setEnd(poPoly->evaluate(&dbl));
                    }
                }

                pCurrentArg = pIL->extractFullMatrix()->getAs<Double>();
            }
            else if(pIT->getAs<types::String>())
            {//see later for extract from struct or Tlist
            }
            else if(pIT->getAs<types::Polynom>())
            {//$
                Polynom* pMP = pIT->getAs<types::Polynom>();
                int iMaxDim     = 0;
                //if pRef == NULL, use 0 insteadof, to allow a($+1) on new variable
                if(_pRef)
                {
                    iMaxDim     = _pRef->getAsGenericType()->getVarMaxDim(i, iDims);
                }
                Double dbl(iMaxDim); // $
                pCurrentArg = pMP->evaluate(&dbl);
            }
            else if(pIT->getAs<types::Bool>())
            {//[T F F T F]
                Bool *pB    = pIT->getAs<types::Bool>();
                int *piB    = pB->get();

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
                pCurrentArg = pDbl;
            }

            if(pCurrentArg)
            {
                _piCountDim[i] = pCurrentArg->getSize();
                _piMaxDim[i] = 0;
                for(int j = 0 ; j < _piCountDim[i] ; j++)
                {
                    _piMaxDim[i] = Max(_piMaxDim[i], static_cast<int>(pCurrentArg->get(j)));
                }

                iSeqCount *= _piCountDim[i];
            }

            _pArgsOut->push_back(pCurrentArg);

        }

        //returns a negative value if at least one parameter is undefined
        //case with : or $ for creation by insertion
        return (!bUndefine ? iSeqCount : - iSeqCount);
    }
}
