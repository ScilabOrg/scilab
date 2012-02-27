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
#include "min.hxx"

/*--------------------------- Double ---------------------------------------*/
void min(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut)
{
    int iDims       = vectIn[0]->getDims();
    int* iDimsArray = vectIn[0]->getDimsArray();
    int iSize       = vectIn[0]->getSize();
    int iInit       = 0;
    double dValue   = 0;

    if(vectIn.size() > 1) // Find the min value between all inputs matrix
    {
        //Find the first non scalar to init output matrix.
        for(int iter = 0; iter < vectIn.size(); iter++)
        {
            if(vectIn[iter]->isScalar() == false)
            {
                iInit = iter;
                break;
            }
        }

        iDims = vectIn[iInit]->getDims();
        iDimsArray = vectIn[iInit]->getDimsArray();
        iSize = vectIn[iInit]->getSize();

        // Init output matrix
        for(int i = 0; i < iSize; i++)
        {
            pOut->set(i, vectIn[iInit]->get(i));
        }

        if(pDblIndex)
        {
            for(int i = 0; i < pDblIndex->getSize(); i++)
            {
                pDblIndex->set(i, 1);
            }

            for(int i = 0; i < iSize; i++)
            {
                for(int iter = 0; iter < vectIn.size(); iter++)
                {
                    int iPos = i;
                    if(vectIn[iter]->isScalar())
                    {
                        iPos = 0;
                    }

                    dValue = vectIn[iter]->get(iPos);
                    if(pOut->get(i) > dValue || ISNAN(pOut->get(i)))
                    {
                        pOut->set(i, dValue);
                        pDblIndex->set(i, iter + 1);
                    }
                }
            }
        }
        else
        {
            for(int i = 0; i < iSize; i++)
            {
                for(int iter = 0; iter < vectIn.size(); iter++)
                {
                    int iPos = i;
                    if(vectIn[iter]->isScalar())
                    {
                        iPos = 0;
                    }

                    dValue = vectIn[iter]->get(iPos);
                    if(pOut->get(i) > dValue || ISNAN(pOut->get(i)))
                    {
                        pOut->set(i, dValue);
                    }
                }
            }
        }
    }
    else
    {
        if(iOrientation == 0) // Find the min value between all matrix elements
        {
            double dMin = vectIn[0]->get(0);
            int iIndex = 0;
            for(int i = 1; i < iSize; i++)
            {
                dValue = vectIn[0]->get(i);
                if(dMin > dValue || ISNAN(dMin))
                {
                    dMin = dValue;
                    iIndex = i;
                }
            }

            pOut->set(0, dMin);
            if(pDblIndex)
            {
                if(pDblIndex->isScalar())
                {
                    pDblIndex->set(0, iIndex + 1);
                }
                else
                {
                    int* piIndexes = new int[iDims];
                    types::getIndexesWithDims(iIndex, piIndexes, iDimsArray, iDims);
                    for(int i = 0; i < pDblIndex->getSize(); i++)
                    {
                        pDblIndex->set(i, piIndexes[i] + 1);
                    }
                    delete piIndexes;
                }
            }
        }
        else // Find the min value between all elements of the dimension N (iOrientation)
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
                    // init pOut with the first values of the dim N
                    pOut->set(iIncrOut, vectIn[0]->get(i));

                    if(pDblIndex)
                    {
                        pDblIndex->set(iIncrOut, 1);
                    }

                    for(int k = 1; k < iSizeOfDimN; k++)
                    {
                        dValue = vectIn[0]->get(k * iIncrement + i);
                        if(pOut->get(iIncrOut) > dValue || ISNAN(pOut->get(iIncrOut)))
                        {
                            pOut->set(iIncrOut, dValue);
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
void min(std::vector<types::Sparse*> vectIn, int iOrientation, types::Sparse* pSparseIndex, types::Sparse* pOut)
{
    int iDims       = vectIn[0]->getDims();
    int* iDimsArray = vectIn[0]->getDimsArray();
    int iSize       = vectIn[0]->getSize();
    int iIndex      = 0;
    double dMin     = 0;

    size_t iNonZeros    = 0;
    double* pRowsPos    = NULL;
    double* pColsPos    = NULL;
    double* pNonZeroR   = NULL;
    double* pNonZeroI   = NULL;

    //get values and position from sparse matrix
    iNonZeros = vectIn[0]->nonZeros();
    pRowsPos = new double[iNonZeros * 2];
    vectIn[0]->outputRowCol(pRowsPos);
    pColsPos = pRowsPos + iNonZeros;
    pNonZeroR = new double[iNonZeros];
    pNonZeroI = new double[iNonZeros];
    vectIn[0]->outputValues(pNonZeroR, pNonZeroI);

    if(vectIn.size() > 1) // Find the min value between all inputs matrix
    {
        for(int i = 0; i < iNonZeros; i++) // init the result with the first sparse
        {
            int iNumRow = (int)(pRowsPos[i] - 1);
            int iNumCol = (int)(pColsPos[i] - 1);

            pOut->set(iNumRow, iNumCol, pNonZeroR[i]);
        }

        if(pSparseIndex)
        {
            for(int i = 0; i < iNonZeros; i++) // init the result with the first sparse
            {
                int iNumRow = (int)(pRowsPos[i] - 1);
                int iNumCol = (int)(pColsPos[i] - 1);

                pSparseIndex->set(iNumRow, iNumCol, 1);
            }
        }

        for(int iter = 1; iter < vectIn.size(); iter++) // find the min values
        {
            iNonZeros = vectIn[iter]->nonZeros();
            pRowsPos = new double[iNonZeros * 2];
            vectIn[iter]->outputRowCol(pRowsPos);
            pColsPos = pRowsPos + iNonZeros;
            pNonZeroR = new double[iNonZeros];
            pNonZeroI = new double[iNonZeros];
            vectIn[iter]->outputValues(pNonZeroR, pNonZeroI);

            if(pSparseIndex)
            {
                for(int i = 0; i < iNonZeros; i++)
                {
                    int iNumRow = (int)(pRowsPos[i] - 1);
                    int iNumCol = (int)(pColsPos[i] - 1);

                    if(pOut->get(iNumRow, iNumCol) > pNonZeroR[i] || ISNAN(pOut->get(iNumRow, iNumCol)))
                    {
                        pOut->set(iNumRow, iNumCol, pNonZeroR[i]);
                        pSparseIndex->set(iNumRow, iNumCol, iter + 1);
                    }
                }
            }
            else
            {
                for(int i = 0; i < iNonZeros; i++)
                {
                    int iNumRow = (int)(pRowsPos[i] - 1);
                    int iNumCol = (int)(pColsPos[i] - 1);

                    if(pOut->get(iNumRow, iNumCol) > pNonZeroR[i] || ISNAN(pOut->get(iNumRow, iNumCol)))
                    {
                        pOut->set(iNumRow, iNumCol, pNonZeroR[i]);
                    }
                }
            }
        }

        // check that the positive values are really the min values.
        iNonZeros = pOut->nonZeros();
        pRowsPos = new double[iNonZeros * 2];
        pOut->outputRowCol(pRowsPos);
        pColsPos = pRowsPos + iNonZeros;
        pNonZeroR = new double[iNonZeros];
        pNonZeroI = new double[iNonZeros];
        pOut->outputValues(pNonZeroR, pNonZeroI);

        for(int i = 0; i < iNonZeros; i++)
        {
            if(pNonZeroR[i] > 0 || ISNAN(pNonZeroR[i]))
            {
                int iNumRow = (int)(pRowsPos[i] - 1);
                int iNumCol = (int)(pColsPos[i] - 1);

                for(int j = 0; j < vectIn.size(); j++)
                {
                    if(vectIn[j]->get(iNumRow, iNumCol) == 0)
                    {
                        pOut->set(iNumRow, iNumCol, 0);
                        if(pSparseIndex)
                        {
                            pSparseIndex->set(iNumRow, iNumCol, j + 1);
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if(iOrientation == 0) // Find the min value between all matrix elements
        {
            iIndex  = 0;
            dMin = pNonZeroR[0];

            for(int i = 1; i < iNonZeros; i++)
            {
                if(dMin > pNonZeroR[i] || ISNAN(dMin))
                {
                    dMin = pNonZeroR[i];
                    iIndex = i;
                }
            }

            iIndex = (pColsPos[iIndex] - 1) * iDimsArray[0] + (pRowsPos[iIndex] - 1);

            if((dMin > 0 || ISNAN(dMin)) && iNonZeros < iSize) // get the first 0
            {
                dMin = 0;
                if(pSparseIndex)
                {
                    iIndex = 0;
                    for(int i = 0; i < iSize; i++)
                    {
                        if(i != (int)((pColsPos[i] - 1) * iDimsArray[0] + (pRowsPos[i] - 1)))
                        {
                            iIndex = i;
                            break;
                        }
                    }
                }
            }

            pOut->set(0, 0, dMin);
            if(pSparseIndex)
            {
                if(pSparseIndex->isScalar())
                {
                    pSparseIndex->set(0, 0, iIndex);
                }
                else
                {
                    int* piIndexes = new int[iDims];
                    types::getIndexesWithDims(iIndex, piIndexes, iDimsArray, iDims);
                    for(int i = 0; i < pSparseIndex->getSize(); i++)
                    {
                        pSparseIndex->set(0, i, piIndexes[i] + 1);
                    }
                    delete piIndexes;
                }
            }
        }
        else
        {
            if(iOrientation == 1) // cols
            {
                if(pSparseIndex)
                {
                    for(int i = 0; i < iNonZeros; i++)
                    {
                        int iNumRow = (int)(pRowsPos[i] - 1);
                        int iNumCol = (int)(pColsPos[i] - 1);
                        if(iNumRow == 0)
                        {
                            pOut->set(0, iNumCol, pNonZeroR[i]);
                            pSparseIndex->set(0, iNumCol, pRowsPos[i]);
                        }
                        else if(pOut->get(0, iNumCol) > pNonZeroR[i] || ISNAN(pOut->get(0, iNumCol)))
                        {
                            pOut->set(0, iNumCol, pNonZeroR[i]);
                            pSparseIndex->set(0, iNumCol, pRowsPos[i]);
                        }
                    }
                }
                else
                {
                    for(int i = 0; i < iNonZeros; i++)
                    {
                        int iNumRow = (int)(pRowsPos[i] - 1);
                        int iNumCol = (int)(pColsPos[i] - 1);
                        if(iNumRow == 0)
                        {
                            pOut->set(0, iNumCol, pNonZeroR[i]);
                        }
                        else if(pOut->get(0, iNumCol) > pNonZeroR[i] || ISNAN(pOut->get(0, iNumCol)))
                        {
                            pOut->set(0, iNumCol, pNonZeroR[i]);
                        }
                    }
                }

                // check that the positive values are really the min values.
                iNonZeros = pOut->nonZeros();
                pRowsPos = new double[iNonZeros * 2];
                pOut->outputRowCol(pRowsPos);
                pColsPos = pRowsPos + iNonZeros;
                pNonZeroR = new double[iNonZeros];
                pNonZeroI = new double[iNonZeros];
                pOut->outputValues(pNonZeroR, pNonZeroI);

                for(int i = 0; i < iNonZeros; i++)
                {
                    if(pNonZeroR[i] > 0 || ISNAN(pNonZeroR[i]))
                    {
                        int iNumCol = (int)(pColsPos[i] - 1);
                        for(int j = 0; j < iDimsArray[0]; j++)
                        {
                            if(vectIn[0]->get(j, iNumCol) == 0)
                            {
                                pOut->set(0, iNumCol, 0);
                                if(pSparseIndex)
                                {
                                    pSparseIndex->set(0, iNumCol, j + 1);
                                }
                                break;
                            }
                        }
                    }
                }
            }
            else if(iOrientation == 2) // rows
            {
                if(pSparseIndex)
                {
                    for(int i = 0; i < iNonZeros; i++)
                    {
                        int iNumRow = (int)(pRowsPos[i] - 1);
                        int iNumCol = (int)(pColsPos[i] - 1);
                        if(iNumCol == 0)
                        {
                            pOut->set(iNumRow, 0, pNonZeroR[i]);
                            pSparseIndex->set(iNumRow, 0, pColsPos[i]);
                        }
                        else if(pOut->get(iNumRow, 0) > pNonZeroR[i] || ISNAN(pOut->get(iNumRow, 0)))
                        {
                            pOut->set(iNumRow, 0, pNonZeroR[i]);
                            pSparseIndex->set(iNumRow, 0, pColsPos[i]);
                        }
                    }
                }
                else
                {
                    for(int i = 0; i < iNonZeros; i++)
                    {
                        int iNumRow = (int)(pRowsPos[i] - 1);
                        int iNumCol = (int)(pColsPos[i] - 1);
                        if(iNumCol == 0)
                        {
                            pOut->set(iNumRow, 0, pNonZeroR[i]);
                        }
                        else if(pOut->get(iNumRow, 0) > pNonZeroR[i] || ISNAN(pOut->get(iNumRow, 0)))
                        {
                            pOut->set(iNumRow, 0, pNonZeroR[i]);
                        }
                    }
                }

                // check that the positive values are really the min values.
                iNonZeros = pOut->nonZeros();
                pRowsPos = new double[iNonZeros * 2];
                pOut->outputRowCol(pRowsPos);
                pColsPos = pRowsPos + iNonZeros;
                pNonZeroR = new double[iNonZeros];
                pNonZeroI = new double[iNonZeros];
                pOut->outputValues(pNonZeroR, pNonZeroI);

                for(int i = 0; i < iNonZeros; i++)
                {
                    int iNumRow = (int)(pRowsPos[i] - 1);
                    if(pOut->get(iNumRow, 0) > 0 || ISNAN(pNonZeroR[i]))
                    {
                        for(int j = 0; j < iDimsArray[1]; j++)
                        {
                            if(vectIn[0]->get(iNumRow, j) == 0)
                            {
                                pOut->set(iNumRow, 0, 0);
                                if(pSparseIndex)
                                {
                                    pSparseIndex->set(iNumRow, 0, j + 1);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void min(types::Sparse* SparseIn, types::Double* pDblIndex, types::Double* pOut)
{
    int iDims       = SparseIn->getDims();
    int* iDimsArray = SparseIn->getDimsArray();
    int iSize       = SparseIn->getSize();
    int iIndex      = 0;
    double dMin     = 0;

    size_t iNonZeros    = 0;
    double* pRowsPos    = NULL;
    double* pColsPos    = NULL;
    double* pNonZeroR   = NULL;
    double* pNonZeroI   = NULL;

    //get values and position from sparse matrix
    iNonZeros = SparseIn->nonZeros();
    pRowsPos = new double[iNonZeros * 2];
    SparseIn->outputRowCol(pRowsPos);
    pColsPos = pRowsPos + iNonZeros;
    pNonZeroR = new double[iNonZeros];
    pNonZeroI = new double[iNonZeros];
    SparseIn->outputValues(pNonZeroR, pNonZeroI);

    iIndex = 0;
    dMin = pNonZeroR[0];

    for(int i = 1; i < iNonZeros; i++)
    {
        if(dMin > pNonZeroR[i] || ISNAN(dMin))
        {
            dMin = pNonZeroR[i];
            iIndex = i;
        }
    }

    iIndex = (pColsPos[iIndex] - 1) * iDimsArray[0] + (pRowsPos[iIndex] - 1);

    if((dMin > 0 || ISNAN(dMin)) && iNonZeros < iSize) // get the first 0
    {
        dMin = 0;
        if(pDblIndex)
        {
            iIndex = 0;
            for(int i = 0; i < iSize; i++)
            {
                if(i != (int)((pColsPos[i] - 1) * iDimsArray[0] + (pRowsPos[i] - 1)))
                {
                    iIndex = i;
                    break;
                }
            }
        }
    }

    pOut->set(0, dMin);
    if(pDblIndex)
    {
        if(pDblIndex->isScalar())
        {
            pDblIndex->set(0, iIndex + 1);
        }
        else
        {
            int* piIndexes = new int[iDims];
            types::getIndexesWithDims(iIndex, piIndexes, iDimsArray, iDims);
            for(int i = 0; i < pDblIndex->getSize(); i++)
            {
                pDblIndex->set(i, piIndexes[i] + 1);
            }
            delete piIndexes;
        }
    }
}
