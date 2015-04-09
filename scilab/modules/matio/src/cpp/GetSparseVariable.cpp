/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.hxx"

extern "C"
{
#include "api_scilab.h"
#include "scisparse.h"
}

matvar_t *GetSparseVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{

    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isSparse() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: Sparse matrix expected.\n"), "GetSparseVariable");
        return NULL;
    }

    types::Sparse* pSparse = in[iVar - 1]->getAs<types::Sparse>();

    return GetMat_VarSparse(pSparse, name);
}

matvar_t* GetMat_VarSparse(types::Sparse* pSparse, const char *name)
{
    int Dims = pSparse->getDims();
    int* pDims = pSparse->getDimsArray();
    int isize = pSparse->getSize();
    size_t* psize_t = NULL;
    matvar_t * pMatVarOut = NULL;

    if (pSparse->getDims() > 2)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetMat_VarSparse");
        return NULL;
    }

    mat_sparse_t *sparseData = NULL;
    sparseData = (mat_sparse_t*)MALLOC(sizeof(mat_sparse_t));
    if (sparseData == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetMat_VarSparse");
        return NULL;
    }

    int nonZeros = pSparse->nonZeros();
    int* colPos = new int[nonZeros];
    int* itemsRow = new int[pSparse->getRows()];
    pSparse->getNbItemByRow(itemsRow);

    int* colIndexes = (int*)MALLOC(sizeof(int) *  (pSparse->getCols() + 1));
    if (colIndexes == NULL)
    {
        FREE(sparseData);
        Scierror(999, _("%s: No more memory.\n"), "GetMat_VarSparse");
        return NULL;
    }

    colIndexes[0] = 0;

    for (int K = 0; K < pSparse->getCols(); ++K)
    {
        colIndexes[K + 1] = colIndexes[K] + itemsRow[K];
    }

    int* rowIndexes = (int*)MALLOC(sizeof(int) *  nonZeros);
    if (rowIndexes == NULL)
    {
        FREE(sparseData);
        FREE(colIndexes);
        Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
        return NULL;
    }

    pSparse->getColPos(colPos);
    for (int K = 0; K < nonZeros; ++K)
    {
        rowIndexes[K] = colPos[K] - 1;
    }


    /* Create Matlab Sparse matrix data */
    sparseData->nzmax = nonZeros;
    sparseData->nir = nonZeros;
    sparseData->ir = rowIndexes;
    /* sparseData->njc   = scilabSparse.nel + 1; */
    sparseData->njc = pSparse->getCols() + 1;
    sparseData->jc = colIndexes;
    sparseData->ndata = nonZeros;
    /* get position data*/
    int* iPositVal = new int[nonZeros];
    int j = 0;
    for (int i = 0; i < nonZeros; j++)
    {
        for (int k = 0; k < itemsRow[j]; k++)
        {

            iPositVal[i] = rowIndexes[i] * pSparse->getRows() + j;
            i++;
        }
    }

    psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (rowIndexes == NULL)
    {
        FREE(sparseData);
        FREE(rowIndexes);
        FREE(colIndexes);
        Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
        return NULL;
    }

    psize_t[0] = (int)pDims[0];
    psize_t[1] = (int)pDims[1];

    double* data = NULL;

    if (pSparse->isComplex())
    {
        if ((data = (double*)MALLOC(2 * sizeof(double) * nonZeros)) == NULL)
        {
            FREE(psize_t);
            FREE(sparseData);
            FREE(colIndexes);
            FREE(rowIndexes);
            Scierror(999, _("%s: No more memory.\n"), "GetMat_VarSparse");
            return NULL;
        }

        std::complex<double> complexData;
        for (int K = 0; K < nonZeros; ++K)
        {
            complexData = pSparse->getImg(iPositVal[K]);
            data[K] = complexData.real();
            data[nonZeros + K] = complexData.imag();
        }

        sparseData->data = (void*)data;

        pMatVarOut = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, Dims, psize_t, sparseData, MAT_F_COMPLEX | MAT_F_DONT_COPY_DATA);
    }
    else
    {
        if ((data = (double*)MALLOC(sizeof(double) * nonZeros)) == NULL)
        {
            FREE(psize_t);
            FREE(sparseData);
            FREE(colIndexes);
            FREE(rowIndexes);
            Scierror(999, _("%s: No more memory.\n"), "GetMat_VarSparse");
            return NULL;
        }

        for (int K = 0; K < nonZeros; ++K)
        {
            data[K] = pSparse->getReal(iPositVal[K]);
        }


        sparseData->data = (void*)data;

        pMatVarOut = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, Dims, psize_t, sparseData, 0 | MAT_F_DONT_COPY_DATA);
    }

    FREE(sparseData);
    FREE(psize_t);
    return pMatVarOut;
}