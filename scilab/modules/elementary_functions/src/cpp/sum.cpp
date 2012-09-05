/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "sum.hxx"

types::Double* sum(types::Double* pIn, int iOrientation)
{
    types::Double* pOut = NULL;
    if (iOrientation == 0)
    {
        double dblR = 0;
        double dblI = 0;

        for (int i = 0 ; i < pIn->getSize() ; i++)
        {
            dblR += pIn->get(i);
        }

        pOut = new types::Double(dblR);
        if (pIn->isComplex())
        {
            pOut->setComplex(true);
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                dblI += pIn->getImg(i);
            }
            pOut->setImg(0, dblI);
        }
    }
    else
    {
        int iDims = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0 ; i < iDims ; i++)
        {
            if (iOrientation == i + 1)
            {
                piDims[i] = 1;
            }
            else
            {
                piDims[i] = pIn->getDimsArray()[i];
            }
        }

        pOut = new types::Double(iDims, piDims, pIn->isComplex());
        pOut->setZeros();

        double* pdblIn  = pIn->get();
        double* pdblOut = pOut->get();

        int* piIndex = new int[iDims];

        if (pIn->isComplex())
        {
            double* pInImg  = pIn->getImg();
            double* pOutImg = pOut->getImg();
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);
                pOutImg[iIndex] += pInImg[i];
                pdblIn[iIndex] += pdblIn[i];
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
                pdblOut[iIndex] += pdblIn[i];
            }
        }
    }

    return pOut;
}

// polynom
types::Polynom* sum(types::Polynom* pIn, int iOrientation)
{
    types::Polynom* pOut = NULL;
    if (iOrientation == 0) // sum of all element
    {
        int* piRanks = new int[pIn->getSize()];
        pIn->getRank(piRanks);
        int iRankMax = pIn->getMaxRank();
        pOut = new types::Polynom(pIn->getVariableName(), 1, 1, &iRankMax);
        pOut->setComplex(pIn->isComplex());

        double* dblRealOut = pOut->get(0)->getCoef()->getReal();
        dblRealOut[0] = 0;
        if (pIn->isComplex())
        {
            double* dblImgOut = pOut->get(0)->getCoef()->getImg();
            dblImgOut[0] = 0;
            for (int i = 0; i < pIn->getSize(); i++)
            {
                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
                double* dblImgIn = pIn->get(i)->getCoef()->getImg();
                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                    dblImgOut[iRankN] += dblImgIn[iRankN];
                }
            }
        }
        else
        {
            for (int i = 0; i < pIn->getSize(); i++)
            {
                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                }
            }
        }
    }
    else // sum following a dimension
    {
        int iDims = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0 ; i < iDims ; i++)
        {
            if (iOrientation == i + 1)
            {
                piDims[i] = 1;
            }
            else
            {
                piDims[i] = pIn->getDimsArray()[i];
            }
        }

        // get ranks of all polynom
        int* piRanks = new int[pIn->getSize()];
        pIn->getRank(piRanks);

        // compute the maximum rank of the dim to compute
        types::Double* pDblRanks = new types::Double(pIn->getDims(), pIn->getDimsArray());
        for (int i = 0; i < pDblRanks->getSize(); i++)
        {
            pDblRanks->set(i, static_cast<double>(piRanks[i]));
        }

        types::Double* pDblRanksOut = new types::Double(iDims, piDims);

        int*    piIndex = new int[iDims];
        double* pdblIn  = pDblRanks->get();
        double* pdblOut = pDblRanksOut->get();

        for (int i = 0 ; i < pDblRanks->getSize() ; i++)
        {
            //get array of dim
            pDblRanks->getIndexes(i, piIndex);

            //convert indexes for result
            piIndex[iOrientation - 1] = 0;
            int iIndex = pDblRanksOut->getIndex(piIndex);
            pdblOut[iIndex] = std::max(pdblOut[iIndex], pdblIn[i]);
        }

        int* piRankMax = new int[pDblRanksOut->getSize()];
        for (int i = 0; i < pDblRanksOut->getSize(); i++)
        {
            piRankMax[i] = static_cast<int>(pdblOut[i]);
        }

        // create the outpout polynom
        pOut = new types::Polynom(pIn->getVariableName(), iDims, piDims, piRankMax);
        pOut->setComplex(pIn->isComplex());

        if (pIn->isComplex())
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);

                // make sum on each ranks
                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
                double* dblRealOut = pOut->get(iIndex)->getCoef()->getReal();
                double* dblImgIn = pIn->get(i)->getCoef()->getImg();
                double* dblImgOut = pOut->get(iIndex)->getCoef()->getImg();
                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                    dblImgOut[iRankN] += dblImgIn[iRankN];
                }
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

                // make sum on each ranks
                double* dblRealIn = pIn->get(i)->getCoef()->getReal();
                double* dblRealOut = pOut->get(iIndex)->getCoef()->getReal();
                for (int iRankN = 0; iRankN < piRanks[i]; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                }
            }
        }
    }

    return pOut;
}
