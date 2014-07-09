/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012-2014 - DIGITEO - cedric delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __DIAG_H__
#define __DIAG_H__

#include "double.hxx"
#include "polynom.hxx"
#include "string.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP types::InternalType* diag(types::Polynom* pIn,  int iStartPos);
ELEMENTARY_FUNCTIONS_IMPEXP types::InternalType* diag(types::String* pIn,  int iStartPos);

template<class bigT, class littleT>
types::InternalType* diag(bigT* pIn,  int iStartPos)
{
    bigT* pOut = NULL;

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
            pOut = new bigT(iSize, 1);
            pOut->setComplex(pIn->isComplex());
        }
        else
        {
            return types::Double::Empty();
        }

        littleT* pOutReal = pOut->get();
        littleT* pInReal  = pIn->get();

        if (pIn->isComplex())
        {
            littleT* pOutImag = pOut->getImg();
            littleT* pInImag  = pIn->getImg();
            for (int i = 0; i < iSize; i++)
            {
                iPos = (i + iStartCol) * iRows + (i + iStartRow);
                pOutReal[i] = pInReal[iPos];
                pOutImag[i] = pInImag[iPos];
            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                iPos = (i + iStartCol) * iRows + (i + iStartRow);
                pOutReal[i] = pInReal[iPos];
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

        pOut = new bigT(iSize, iSize);
        pOut->setComplex(pIn->isComplex());
        memset(pOut->get(), 0x00, iSize * iSize * sizeof(littleT));

        if (pIn->isComplex())
        {
            memset(pOut->getImg(), 0x00, iSize * iSize * sizeof(littleT));
        }

        littleT* pOutReal = pOut->get();
        littleT* pInReal  = pIn->get();

        if (pIn->isComplex())
        {
            littleT* pOutImag = pOut->getImg();
            littleT* pInImag  = pIn->getImg();

            for (int i = 0; i < iSizeOfVector; i++)
            {
                iPos = (i + iStartCol) * iSize + (i + iStartRow);
                pOutReal[i] = pInReal[iPos];
                pOutImag[i] = pInImag[iPos];
            }
        }
        else
        {
            for (int i = 0; i < iSizeOfVector; i++)
            {
                iPos = (i + iStartCol) * iSize + (i + iStartRow);
                pOutReal[i] = pInReal[iPos];
            }
        }
    }

    return pOut;
}

#endif /* __DIAG_H__ */
