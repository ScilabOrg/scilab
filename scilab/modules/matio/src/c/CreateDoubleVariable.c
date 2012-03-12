/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "MALLOC.h"

int CreateDoubleVariable(int iVar, matvar_t *matVariable, int * parent, int item_position)
{
    int nbRow = 0, nbCol = 0;
    mat_complex_split_t *mat5ComplexData = NULL;
    SciErr sciErr;
    int *piDims = NULL;
    int i =0;

    if (matVariable->rank == 2) /* 2-D array */
    {
        nbRow = matVariable->dims[0];
        nbCol = matVariable->dims[1];
        if (matVariable->isComplex == 0)
        {
            if (parent == NULL)
            {
                sciErr = createMatrixOfDouble(pvApiCtx, iVar, nbRow, nbCol, matVariable->data);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
            else
            {
                sciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, matVariable->data);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
        }
        else
        {
            /* Since MATIO 1.3.2 data is a ComplexSplit for MAT4 and MAT5 formats */
            mat5ComplexData = matVariable->data;
            if (parent == NULL)
            {
                sciErr = createComplexMatrixOfDouble(pvApiCtx, iVar, nbRow, nbCol, mat5ComplexData->Re, mat5ComplexData->Im);
            }
            else
            {
                sciErr = createComplexMatrixOfDoubleInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol,
                          mat5ComplexData->Re, mat5ComplexData->Im);
            }
        }
    }
    else /* Multi-dimension array -> Scilab HyperMatrix */
    {
        piDims = (int *) MALLOC(matVariable->rank * sizeof(int));
        for (i = 0 ; i < matVariable->rank ; ++i)
        {
            piDims[i] = matVariable->dims[i];
        }

        if (matVariable->isComplex == 0)
        {
            CreateHyperMatrixVariable(iVar, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &matVariable->rank,
                                      piDims, matVariable->data, NULL, parent, item_position);
        }
        else
        {
            mat5ComplexData = matVariable->data;
            CreateHyperMatrixVariable(iVar, MATRIX_OF_DOUBLE_DATATYPE, &matVariable->isComplex, &matVariable->rank,
                                      piDims, mat5ComplexData->Re, mat5ComplexData->Im, parent, item_position);
        }

        FREE(piDims);
    }
    return TRUE;
}
