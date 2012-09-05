/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "prod.hxx"
extern "C"
{
#include "matrix_multiplication.h"
}

types::Double* prod(types::Double* pIn, int iOrientation)
{
    types::Double* pOut = NULL;
    double* pdblInReal  = pIn->getReal();
    double* pdblInImg   = pIn->getImg();

    if (iOrientation == 0) // all
    {
        double dblR = pdblInReal[0];
        if (pIn->isComplex())
        {
            double dblI = pdblInImg[0];
            double dblRTmp = 0;
            double dblITmp = 0;
            for (int i = 1 ; i < pIn->getSize() ; i++)
            {
                dblRTmp = dblR;
                dblITmp = dblI;
                iMultiComplexMatrixByComplexMatrix( &pdblInReal[i], &pdblInImg[i], 1, 1,
                                                    &dblRTmp, &dblITmp, 1, 1, &dblR, &dblI);
            }

            pOut = new types::Double(dblR, dblI);
        }
        else
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                dblR *= pdblInReal[i];
            }

            pOut = new types::Double(dblR);
        }
    }
    else // prod on one dimension
    {
        // create output dimensions
        int iDims   = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0 ; i < iDims ; i++)
        {
            piDims[i] = pIn->getDimsArray()[i];
        }

        piDims[iOrientation - 1] = 1;

        // create output variable
        pOut = new types::Double(iDims, piDims, pIn->isComplex());
        delete piDims;
        piDims = NULL;

        // init output
        double* pdblOut = pOut->get();
        double* pdblOutImg = pOut->get();;

        if (pIn->isComplex())
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                pdblOut[i] = 1;
                pdblOutImg[i] = 1;
            }
        }
        else
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                pdblOut[i] = 1;
            }
        }

        // perform operations
        int* piIndex = new int[iDims];
        if (pIn->isComplex())
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);

                double dblRTmp = pdblOut[iIndex];
                double dblITmp = pdblOutImg[iIndex];

                iMultiComplexMatrixByComplexMatrix( &pdblInReal[i], &pdblInImg[i], 1, 1,
                                                    &dblRTmp, &dblITmp, 1, 1,
                                                    &pdblOut[iIndex], &pdblOutImg[iIndex]);
            }
        }
        else
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);
                pdblOut[iIndex] *= pdblInReal[i];
            }
        }

        delete piIndex;
        piIndex = NULL;
    }

    return pOut;
}

//// polynom
//types::Polynom* prod(types::Polynom* pIn, int iOrientation)
//{
//    types::Polynom* pOut = NULL;
//    if (iOrientation == 0) // sum of all element
//    {
//        // get rank of all input single poly
//        int* piRanks = new int[pIn->getSize()];
//        pIn->getRank(piRanks);
//
//        // create output matrix of poly
//        int iRankMax = 0;
//        for(int i = 0; i < pIn->getSize(); i++)
//        {
//            iRankMax += piRanks[i];
//        }
//
//        pOut = new types::Polynom(pIn->getVariableName(), 1, 1, &iRankMax);
//        pOut->setComplex(pIn->isComplex());
//
//
//        double* dblRealOut = pOut->get(0)->getCoef()->getReal();
//        if (pIn->isComplex())
//        {
//            double* dblImgOut = pOut->get(0)->getCoef()->getImg();
//
//            // init output
//            for (int iRankN = 0; iRankN < iRankMax; iRankN++)
//            {
//                dblRealOut[iRankN] = 1;
//                dblImgOut[iRankN]  = 1;
//            }
//
//            // perform operations
//            for (int i = 0; i < pIn->getSize(); i++)
//            {
//                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
//                double* dblImgIn = pIn->get(i)->getCoef()->getImg();
//                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
//                {
//// Stop here
//                    double* pdblTempReal = // loop on each values ...
//                    double* pdblTempImg  =
//                    iMultiComplexPolyByComplexPoly();
//
//                    dblRealOut[iRankN] += dblRealIn[iRankN];
//                    dblImgOut[iRankN] += dblImgIn[iRankN];
//                }
//            }
//        }
//        else
//        {
//            // init output
//            for (int iRankN = 0; iRankN < iRankMax; iRankN++)
//            {
//                dblRealOut[iRankN] = 1;
//            }
//
//            // perform operations
//            for (int i = 0; i < pIn->getSize(); i++)
//            {
//                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
//                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
//                {
//                    iMultiRealPolyByRealPoly
//                    dblRealOut[iRankN] *= dblRealIn[iRankN];
//                }
//            }
//        }
//    }
//    else // sum following a dimension
//    {
//        int iDims = pIn->getDims();
//        int* piDims = new int[iDims];
//
//        for (int i = 0 ; i < iDims ; i++)
//        {
//            if (iOrientation == i + 1)
//            {
//                piDims[i] = 1;
//            }
//            else
//            {
//                piDims[i] = pIn->getDimsArray()[i];
//            }
//        }
//
//        // get ranks of all polynom
//        int* piRanks = new int[pIn->getSize()];
//        pIn->getRank(piRanks);
//
//        // compute the maximum rank of the dim to compute
//        types::Double* pDblRanks = new types::Double(pIn->getDims(), pIn->getDimsArray());
//        for (int i = 0; i < pDblRanks->getSize(); i++)
//        {
//            pDblRanks->set(i, static_cast<double>(piRanks[i]));
//        }
//
//        types::Double* pDblRanksOut = new types::Double(iDims, piDims);
//
//        int*    piIndex = new int[iDims];
//        double* pdblIn  = pDblRanks->get();
//        double* pdblOut = pDblRanksOut->get();
//
//        for (int i = 0 ; i < pDblRanks->getSize() ; i++)
//        {
//            //get array of dim
//            pDblRanks->getIndexes(i, piIndex);
//
//            //convert indexes for result
//            piIndex[iOrientation - 1] = 0;
//            int iIndex = pDblRanksOut->getIndex(piIndex);
//            pdblOut[iIndex] = std::max(pdblOut[iIndex], pdblIn[i]);
//        }
//
//        int* piRankMax = new int[pDblRanksOut->getSize()];
//        for (int i = 0; i < pDblRanksOut->getSize(); i++)
//        {
//            piRankMax[i] = static_cast<int>(pdblOut[i]);
//        }
//
//        // create the outpout polynom
//        pOut = new types::Polynom(pIn->getVariableName(), iDims, piDims, piRankMax);
//        pOut->setComplex(pIn->isComplex());
//
//        if (pIn->isComplex())
//        {
//            for (int i = 0 ; i < pIn->getSize() ; i++)
//            {
//                //get array of dim
//                pIn->getIndexes(i, piIndex);
//
//                //convert indexes for result
//                piIndex[iOrientation - 1] = 0;
//                int iIndex = pOut->getIndex(piIndex);
//
//                // make sum on each ranks
//                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
//                double* dblRealOut = pOut->get(iIndex)->getCoef()->getReal();
//                double* dblImgIn = pIn->get(i)->getCoef()->getImg();
//                double* dblImgOut = pOut->get(iIndex)->getCoef()->getImg();
//                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
//                {
//                    dblRealOut[iRankN] += dblRealIn[iRankN];
//                    dblImgOut[iRankN] += dblImgIn[iRankN];
//                }
//            }
//        }
//        else
//        {
//            for (int i = 0 ; i < pIn->getSize() ; i++)
//            {
//                //get array of dim
//                pIn->getIndexes(i, piIndex);
//
//                //convert indexes for result
//                piIndex[iOrientation - 1] = 0;
//                int iIndex = pOut->getIndex(piIndex);
//
//                // make sum on each ranks
//                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
//                double* dblRealOut = pOut->get(iIndex)->getCoef()->getReal();
//                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
//                {
//                    dblRealOut[iRankN] += dblRealIn[iRankN];
//                }
//            }
//        }
//    }
//
//    return pOut;
//}
//
