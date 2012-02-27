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

#include "cumprod.hxx"

int cumprod(types::Double* pIn, int iOrientation, types::Double* pOut)
{
    if (iOrientation == 0) // all
    {
        pOut->set(0, pIn->get(0));
        for (int i = 1; i < pIn->getSize(); i++)
        {
            pOut->set(i, pIn->get(i) * pOut->get(i - 1));
        }

        if (pIn->isComplex())
        {
            pOut->setImg(0, pIn->getImg(0));
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pOut->setImg(i, pIn->getImg(i) * pOut->getImg(i - 1));
            }
        }
    }
    else // cumprod on one dimension
    {
        int iSizeOfDimN = pIn->getDimsArray()[iOrientation - 1];
        int iIncrement = 1;

        for (int i = 0; i < iOrientation - 1; i++)
        {
            iIncrement *= pIn->getDimsArray()[i];
        }

        for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
        {
            for (int i = j; i < iIncrement + j; i++) // set the first values in out
            {
                pOut->set(i, pIn->get(i));
            }

            for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
            {
                for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                {
                    pOut->set(i, pIn->get(i) * pOut->get(i - iIncrement));
                }
            }
        }
        if (pIn->isComplex())
        {
            for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pOut->setImg(i, pIn->getImg(i));
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        pOut->setImg(i, pIn->getImg(i) * pOut->getImg(i - iIncrement));
                    }
                }
            }
        }
    }

    return 0;
}

// sparse
//int cumprod(types::Sparse* pIn, int iOrientation, types::Sparse* pOut)
//{
//    int const nonZeros  = static_cast<int>(pIn->nonZeros());
//    double* pRows       = new double[nonZeros * 2];
//    pIn->outputRowCol(pRows);
//    double* pCols       = pRows + nonZeros;
//
//    double dLastReal    = 1;
//    double dLastImag    = 1;
//    int iErr            = 0;
//    int iRows           = 0;
//    int iCols           = 0;
//    int iBegin          = 0;
//    int iEnd            = 0;
//
//    if(iOrientation == 0) // all
//    {
//        int iFirstZero = 0;
//        bool bBreak = false;
//
//        for(int i = 0; (i < pIn->getCols() && bBreak == false); i++)
//        {
//            for(int j = 0; j < pIn->getRows(); j++)
//            {
//                if( (iFirstZero >= nonZeros)    ||
//                    ((int)(pRows[iFirstZero]-1) != j)  ||
//                    ((int)(pCols[iFirstZero]-1) != i)  )
//                {
//                    bBreak = true;
//                    break;
//                }
//                iFirstZero++;
//            }
//        }
//
//        for(int i = 0 ; i < iFirstZero ; i++)
//        {
//            iRows = pRows[i]-1;
//            iCols = pCols[i]-1;
//            if(pIn->isComplex())
//            {
//                std::complex<double> cplxIn = pIn->getImg(iRows, iCols);
//                dLastReal *= cplxIn.real();
//                dLastImag *= cplxIn.imag();
//
//                std::complex<double> cplxOut(dLastReal, dLastImag);
//                pOut->set(iRows, iCols, cplxOut);
//            }
//            else
//            {
//                dLastReal *= pIn->get(iRows, iCols);
//                pOut->set(iRows, iCols, dLastReal);
//            }
//        }
//    }
//    else // cumprod on row or colomn
//    {
//        std::multimap<int,int> iPosition;
//        std::multimap<int,int>::iterator it;
//
//        int iCount = 0;
//
//        if(iOrientation == 1)
//        {
//            for(int i = 0; i < nonZeros; i++)
//            {
//                iPosition.insert(std::pair<int,int>(pCols[i]-1, pRows[i]-1));
//            }
//
//            for(int j = 0; j < pIn->getCols(); j++)
//            {
//                dLastReal = 1;
//                dLastImag = 1;
//                iRows = 0;
//                iCols = 0;
//                iCount = 0;
//
//                for(it = iPosition.equal_range(j).first; it != iPosition.equal_range(j).second; it++)
//                {
//                    iCols = it->first;
//                    iRows = it->second;
//
//                    if(iRows != iCount)
//                    {
//                        break;
//                    }
//
//                    iCount++;
//
//                    if(pIn->isComplex())
//                    {
//                        std::complex<double> elem = pIn->getImg(iRows, iCols);
//                        dLastReal *= elem.real();
//                        dLastImag *= elem.imag();
//
//                        std::complex<double> cplxOut(dLastReal, dLastImag);
//                        pOut->set(iRows, iCols, cplxOut);
//                    }
//                    else
//                    {
//                        dLastReal *= pIn->get(iRows, iCols);
//                        pOut->set(iRows, iCols, dLastReal);
//                    }
//                }
//            }
//        }
//        else if(iOrientation == 2)
//        {
//            for(int i = 0; i < nonZeros; i++)
//            {
//                iPosition.insert(std::pair<int,int>(pRows[i]-1, pCols[i]-1));
//            }
//
//            for(int j = 0; j < pIn->getRows(); j++)
//            {
//                dLastReal = 1;
//                dLastImag = 1;
//                iRows = 0;
//                iCols = 0;
//                iCount = 0;
//
//                for(it = iPosition.equal_range(j).first; it != iPosition.equal_range(j).second; it++)
//                {
//                    iRows = it->first;
//                    iCols = it->second;
//
//                    if(iCols != iCount)
//                    {
//                        break;
//                    }
//
//                    iCount++;
//
//                    if(pIn->isComplex())
//                    {
//                        std::complex<double> elem = pIn->getImg(iRows, iCols);
//                        dLastReal *= elem.real();
//                        dLastImag *= elem.imag();
//
//                        std::complex<double> cplxOut(dLastReal, dLastImag);
//                        pOut->set(iRows, iCols, cplxOut);
//                    }
//                    else
//                    {
//                        dLastReal *= pIn->get(iRows, iCols);
//                        pOut->set(iRows, iCols, dLastReal);
//                    }
//                }
//            }
//        }
//        else
//        {
//            iErr = 1;
//        }
//    }
//
//    delete[] pRows;
//    return iErr;
//}

// polynom
int cumprod(types::Polynom* pIn, int iOrientation, types::Polynom* pOut)
{
    int iErr        = 0;
    int iRank       = 0;
    int iLastRank   = 0;
    double* pdRData = NULL;
    double* pdIData = NULL;
    bool bComplex   = pIn->isComplex();

    types::SinglePoly* pSP = NULL;

    if (iOrientation == 0) // all
    {
        // set first element
        iRank = pIn->get(0)->getRank();

        if (bComplex)
        {
            pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
        }
        else
        {
            pSP = new types::SinglePoly(&pdRData, iRank);
        }

        for (int j = 0; j < iRank; j++)
        {
            pdRData[j] = pIn->get(0)->getCoefReal()[j];
            if (bComplex)
            {
                pdIData[j] = pIn->get(0)->getCoefImg()[j];
            }
        }

        pOut->set(0, pSP);
        iLastRank = iRank;

        // compute next elements
        for (int i = 1; i < pIn->getSize(); i++)
        {
            if (iRank > pIn->get(i)->getRank())
            {
                iRank = pIn->get(i)->getRank();
            }

            if (bComplex)
            {
                pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
            }
            else
            {
                pSP = new types::SinglePoly(&pdRData, iRank);
            }

            for (int j = 0; j < iRank; j++)
            {
                pdRData[j] = pIn->get(i)->getCoefReal()[j] * pOut->get(i - 1)->getCoefReal()[j];
                if (bComplex)
                {
                    pdIData[j] = pIn->get(i)->getCoefImg()[j] * pOut->get(i - 1)->getCoefImg()[j];
                }
            }

            pOut->set(i, pSP);
            iLastRank = iRank;
        }
    }
    else // cumprod on one dimension
    {
        int iSizeOfDimN = pIn->getDimsArray()[iOrientation - 1];
        int iIncrement = 1;

        for (int i = 0; i < iOrientation - 1; i++)
        {
            iIncrement *= pIn->getDimsArray()[i];
        }

        for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
        {
            for (int i = j; i < iIncrement + j; i++) // set the first values in out
            {
                iRank = pIn->get(i)->getRank();

                if (bComplex)
                {
                    pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
                }
                else
                {
                    pSP = new types::SinglePoly(&pdRData, iRank);
                }

                for (int j = 0; j < iRank; j++)
                {
                    pdRData[j] = pIn->get(i)->getCoefReal()[j];
                    if (bComplex)
                    {
                        pdIData[j] = pIn->get(i)->getCoefImg()[j];
                    }
                }
                pOut->set(i, pSP);
            }

            for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
            {
                for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                {
                    iRank = pOut->get(i - iIncrement)->getRank();
                    if (iRank > pIn->get(i)->getRank())
                    {
                        iRank = pIn->get(i)->getRank();
                    }

                    if (bComplex)
                    {
                        pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
                    }
                    else
                    {
                        pSP = new types::SinglePoly(&pdRData, iRank);
                    }

                    for (int j = 0; j < iRank; j++)
                    {
                        pdRData[j] = pIn->get(i)->getCoefReal()[j] * pOut->get(i - iIncrement)->getCoefReal()[j];
                        if (bComplex)
                        {
                            pdIData[j] = pIn->get(i)->getCoefImg()[j] * pOut->get(i - iIncrement)->getCoefImg()[j];
                        }
                    }
                    pOut->set(i, pSP);
                }
            }
        }
    }

    return iErr;
}

