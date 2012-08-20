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

#include "diag.hxx"

types::Double* diag(types::Double* pIn,  int iStartPos)
{
    types::Double* pDblOut = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = Max(0, Min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = Max(0, Min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pDblOut = new types::Double(iSize, 1, pIn->isComplex());
        }
        else
        {
            return types::Double::Empty();
        }

        for (int i = 0; i < iSize; i++)
        {
            iPos = (i + iStartCol) * iRows + (i + iStartRow);
            pDblOut->set(i, pIn->get(iPos));

            if (pIn->isComplex())
            {
                pDblOut->setImg(i, pIn->getImg(iPos));
            }
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = Max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pDblOut = new types::Double(iSize, iSize, pIn->isComplex());
        memset(pDblOut->get(), 0x00, iSize * iSize * sizeof(double));

        if (pIn->isComplex())
        {
            memset(pDblOut->getImg(), 0x00, iSize * iSize * sizeof(double));
        }

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iPos = (i + iStartCol) * iSize + (i + iStartRow);
            pDblOut->set(iPos, pIn->get(i));

            if (pIn->isComplex())
            {
                pDblOut->setImg(iPos, pIn->getImg(i));
            }
        }
    }

    return pDblOut;
}

//types::Sparse* diag(types::Sparse* pIn,  int iStartPos)
//{
//    types::Sparse* pSparseOut = NULL;
//
//    int iRows = pIn->getRows();
//    int iCols = pIn->getCols();
//
//    int iSize       = 0;
//    int iStartRow   = 0;
//    int iStartCol   = 0;
//    int iPos        = 0;
//
//    bool bFind = false;
//
//    int const nonZeros  = static_cast<int>(pIn->nonZeros());
//    double* pRows       = new double[nonZeros * 2];
//    pIn->outputRowCol(pRows);
//    double* pCols       = pRows + nonZeros;
//
//    std::multimap<int,int> iPosition;
//    std::multimap<int,int>::iterator it;
//
//    for(int i = 0; i < nonZeros; i++)
//    {
//        iPosition.insert(std::pair<int,int>(pRows[i]-1, pCols[i]-1));
//    }
//
//    if(iRows != 1 && iCols != 1) // pIn is a matrix
//    {
//        if(iStartPos < 0)
//        {
//            iSize = Max(0, Min(iRows + iStartPos, iCols));
//            iStartRow = -iStartPos;
//        }
//        else
//        {
//            iSize = Max(0, Min(iRows, iCols - iStartPos));
//            iStartCol = iStartPos;
//        }
//
//        if(iSize)
//        {
//            pSparseOut = new types::Sparse(iSize, 1, pIn->isComplex());
//        }
//        else
//        {
//            return NULL;
//        }
//
//        for(int i = 0; i < iSize; i++)
//        {
//            bFind = false;
//            for(it = iPosition.equal_range(i + iStartRow).first; it != iPosition.equal_range(i + iStartRow).second; it++)
//            {
//                iRows = it->first;
//                iCols = it->second;
//printf("iRows : %d  iCols :  %d\n",iRows, iCols);
//                if(iCols == (i + iStartCol))
//                {
//                    if(pIn->isComplex())
//                    {
//                        std::complex<double> elem = pIn->getImg(iRows, iCols);
//                        pSparseOut->set(i, 0, elem);
//                    }
//                    else
//                    {
//                        pSparseOut->set(i, 0, pIn->get(iRows, iCols));
//                    }
//
//                    break;
//                }
//            }
//        }
//    }
//    else // pIn is a vector
//    {
//        int iSizeOfVector = Max(iRows, iCols);
//        if(iStartPos < 0)
//        {
//            iSize = iSizeOfVector - iStartPos;
//            iStartRow = -iStartPos;
//        }
//        else
//        {
//            iSize = iSizeOfVector + iStartPos;
//            iStartCol = iStartPos;
//        }
//
//        pSparseOut = new types::Sparse(iSize, iSize, pIn->isComplex());
//
//        for(int i = 0; i < nonZeros; i++)
//        {
//            iRows = pRows[i] + iStartRow - 1;
//            iCols = pCols[i] + iStartCol - 1;
//
//            if(pIn->isComplex())
//            {
//                std::complex<double> elem = pIn->getImg(pRows[i]-1, pCols[i]-1);
//                pSparseOut->set(iRows, iCols, elem);
//            }
//            else
//            {
//                pSparseOut->set(iRows, iCols, pIn->get(pRows[i]-1, pCols[i]-1));
//            }
//        }
//    }
//
//    delete[] pRows;
//    return pSparseOut;
//}

types::Polynom* diag(types::Polynom* pIn,  int iStartPos)
{
    types::Polynom* pPolyOut    = NULL;
    types::SinglePoly* pSP      = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;
    int iRank       = 0;

    double* pdRData = 0;
    double* pdIData = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = Max(0, Min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = Max(0, Min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pPolyOut = new types::Polynom(pIn->getVariableName(), iSize, 1);
            pPolyOut->setComplex(pIn->isComplex());
        }
        else
        {
            return NULL;
        }

        for (int i = 0; i < iSize; i++)
        {
            iRank = pIn->get(i)->getRank();

            if (pIn->isComplex())
            {
                pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
            }
            else
            {
                pSP = new types::SinglePoly(&pdRData, iRank);
            }

            iPos = (i + iStartCol) * iRows + (i + iStartRow);

            for (int j = 0; j < iRank; j++)
            {
                pdRData[j] = pIn->get(iPos)->getCoefReal()[j];
                if (pIn->isComplex())
                {
                    pdIData[j] = pIn->get(iPos)->getCoefImg()[j];
                }
            }

            pPolyOut->set(i, pSP);
            delete pSP;
            pSP = NULL;
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = Max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pPolyOut = new types::Polynom(pIn->getVariableName(), iSize, iSize);
        pPolyOut->setComplex(pIn->isComplex());

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iRank = pIn->get(i)->getRank();

            if (pIn->isComplex())
            {
                pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
            }
            else
            {
                pSP = new types::SinglePoly(&pdRData, iRank);
            }

            iPos = (i + iStartCol) * iSize + (i + iStartRow);

            for (int j = 0; j < iRank; j++)
            {
                pdRData[j] = pIn->get(i)->getCoefReal()[j];
                if (pIn->isComplex())
                {
                    pdIData[j] = pIn->get(i)->getCoefImg()[j];
                }
            }

            pPolyOut->set(iPos, pSP);
            delete pSP;
            pSP = NULL;
        }
    }

    return pPolyOut;
}


types::String* diag(types::String* pIn,  int iStartPos)
{
    types::String* pStrOut = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = Max(0, Min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = Max(0, Min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pStrOut = new types::String(iSize, 1);
        }
        else
        {
            return NULL;
        }

        for (int i = 0; i < iSize; i++)
        {
            iPos = (i + iStartCol) * iRows + (i + iStartRow);
            pStrOut->set(i, pIn->get(iPos));
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = Max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pStrOut = new types::String(iSize, iSize);

        for (int i = 0; i < iSize * iSize; i++)
        {
            pStrOut->set(i, L"");
        }

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iPos = (i + iStartCol) * iSize + (i + iStartRow);
            pStrOut->set(iPos, pIn->get(i));
        }
    }

    return pStrOut;
}

