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
#include <algorithm>
#include <vector>

#include "types_tools.hxx"
#include "max.hxx"

/* ISNAN overloading for Mac OS X */
#ifdef __APPLE__
#undef ISNAN
#define ISNAN std::isnan
#endif

/*--------------------------- Double ---------------------------------------*/
void max(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut)
{
    int iDims       = vectIn[0]->getDims();
    int* iDimsArray = vectIn[0]->getDimsArray();
    int iSize       = vectIn[0]->getSize();
    int iInit       = 0;
    double dValue   = 0;

    if (vectIn.size() > 1) // Find the max value between all inputs matrix
    {
        //Find the first non scalar to init output matrix.
        for (int iter = 0; iter < vectIn.size(); iter++)
        {
            if (vectIn[iter]->isScalar() == false)
            {
                iInit = iter;
                break;
            }
        }

        iDims = vectIn[iInit]->getDims();
        iDimsArray = vectIn[iInit]->getDimsArray();
        iSize = vectIn[iInit]->getSize();

        // Init output matrix
        for (int i = 0; i < iSize; i++)
        {
            pOut->set(i, vectIn[iInit]->get(i));
        }

        if (pDblIndex)
        {
            for (int i = 0; i < pDblIndex->getSize(); i++)
            {
                pDblIndex->set(i, 1);
            }

            for (int i = 0; i < iSize; i++)
            {
                for (int iter = 0; iter < vectIn.size(); iter++)
                {
                    int iPos = i;
                    if (vectIn[iter]->isScalar())
                    {
                        iPos = 0;
                    }

                    dValue = vectIn[iter]->get(iPos);
                    if (pOut->get(i) < dValue || ISNAN(pOut->get(i)))
                    {
                        pOut->set(i, dValue);
                        pDblIndex->set(i, iter + 1);
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                for (int iter = 0; iter < vectIn.size(); iter++)
                {
                    int iPos = i;
                    if (vectIn[iter]->isScalar())
                    {
                        iPos = 0;
                    }

                    dValue = vectIn[iter]->get(iPos);
                    if (pOut->get(i) < dValue || ISNAN(pOut->get(i)))
                    {
                        pOut->set(i, dValue);
                    }
                }
            }
        }
    }
    else
    {
        if (iOrientation == 0) // Find the max value between all matrix elements
        {
            double dMax = vectIn[0]->get(0);
            int iIndex = 0;
            for (int i = 1; i < iSize; i++)
            {
                dValue = vectIn[0]->get(i);
                if (dMax < dValue || ISNAN(dMax))
                {
                    dMax = dValue;
                    iIndex = i;
                }
            }

            pOut->set(0, dMax);
            if (pDblIndex)
            {
                if (pDblIndex->isScalar())
                {
                    pDblIndex->set(0, iIndex + 1);
                }
                else
                {
                    int* piIndexes = new int[iDims];
                    types::getIndexesWithDims(iIndex, piIndexes, iDimsArray, iDims);
                    for (int i = 0; i < pDblIndex->getSize(); i++)
                    {
                        pDblIndex->set(i, piIndexes[i] + 1);
                    }
                    delete piIndexes;
                }
            }
        }
        else // Find the max value between all elements of the dimension N (iOrientation)
        {
            int iSizeOfDimN = iDimsArray[iOrientation - 1];
            int iIncrement  = 1;
            int iIncrOut    = 0;

            for (int i = 0; i < iOrientation - 1; i++)
            {
                iIncrement *= iDimsArray[i];
            }

            for (int j = 0; j < iSize; j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++)
                {
                    // init pOut with the first values of the dim N
                    pOut->set(iIncrOut, vectIn[0]->get(i));

                    if (pDblIndex)
                    {
                        pDblIndex->set(iIncrOut, 1);
                    }

                    for (int k = 1; k < iSizeOfDimN; k++)
                    {
                        dValue = vectIn[0]->get(k * iIncrement + i);
                        if (pOut->get(iIncrOut) < dValue || ISNAN(pOut->get(iIncrOut)))
                        {
                            pOut->set(iIncrOut, dValue);
                            if (pDblIndex)
                            {
                                pDblIndex->set(iIncrOut, k + 1);
                            }
                        }
                    }
                    iIncrOut++;
                }
            }
        }
    }
}
