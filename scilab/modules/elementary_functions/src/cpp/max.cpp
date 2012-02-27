/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric Delamarre
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

#include "max.hxx"

/*--------------------------- Double ---------------------------------------*/
void max(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut)
{
    int iDims       = vectIn[0]->getDims();
    int* iDimsArray = vectIn[0]->getDimsArray();
    int iSize       = vectIn[0]->getSize();

    if(vectIn.size() > 1)
    {
        for(int i = 0; i < iSize; i++)
        {
            pOut->set(i, vectIn[0]->get(i));
        }

        if(pDblIndex)
        {
            for(int i = 0; i < pDblIndex->getSize(); i++)
            {
                pDblIndex->set(i, 1);
            }
        }

        for(int i = 0; i < iSize; i++)
        {
            for(int iter = 0; iter < vectIn.size(); iter++)
            {
                if(pOut->get(i) < vectIn[iter]->get(i))
                {
                    pOut->set(i, vectIn[iter]->get(i));
                    if(pDblIndex)
                    {
                        pDblIndex->set(i, iter + 1);
                    }
                }
            }
        }
    }
    else
    {
        if(iOrientation == 0)
        {
            double dMax = vectIn[0]->get(0);
            int iIndex = 0;
            for(int i = 1; i < iSize; i++)
            {
                if(dMax < vectIn[0]->get(i))
                {
                    dMax = vectIn[0]->get(i);
                    iIndex = i;
                }
            }

            pOut->set(0, dMax);
            if(pDblIndex)
            {
                pDblIndex->set(0, iIndex+1);
            }
        }
        else
        {
            int iSizeOfDimN = iDimsArray[iOrientation-1];
            int iIncrement  = 1;
            int iIncrOut    = 0;

            for(int i = 0; i < iOrientation-1; i++)
            {
                iIncrement *= iDimsArray[i];
            }

            for(int j = 0; j < iSize; j += (iIncrement * iSizeOfDimN))
            {
                for(int i = j; i < iIncrement + j; i++)
                {
                    pOut->set(iIncrOut, vectIn[0]->get(i));

                    if(pDblIndex)
                    {
                        pDblIndex->set(iIncrOut, 1);
                    }

                    for(int k = 1; k < iSizeOfDimN; k++)
                    {
                        if(pOut->get(iIncrOut) < vectIn[0]->get(k * iIncrement + i))
                        {
                            pOut->set(iIncrOut, vectIn[0]->get(k * iIncrement + i));
                            if(pDblIndex)
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

/*--------------------------- Sparse ---------------------------------------*/
void max(std::vector<types::Sparse*> vectIn, int iOrientation, types::Double* pDblIndex, types::Sparse* pOut)
{

}
