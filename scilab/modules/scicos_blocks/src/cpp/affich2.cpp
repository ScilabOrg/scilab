/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Helpers.hxx"

extern "C"
{
#include "machine.h"
#include "dynlib_scicos_blocks.h"
#include "MALLOC.h"
#include "scicos_block4.h"
#include "scicos.h"
#include "core_math.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

    double C2F(sciround) (double *x);
    SCICOS_BLOCKS_IMPEXP void affich2(scicos_block * block, int flag);
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos_block;

/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void affich2(scicos_block * block, int flag)
{
    int i;
    int j;
    int iRowsIn = 0;
    int iColsIn = 0;
    double *pdblReal = NULL;
    char ***pstValue = NULL;
    char pstConv[128];

    iRowsIn = GetInPortRows(block, 1);
    iColsIn = GetInPortCols(block, 1);

    pdblReal = (double *)GetInPortPtrs(block, 1);

    //functions
    switch (flag)
    {
        case StateUpdate:          //state evolution
        case ReInitialization:
            // Getting the allocated area
            pstValue = (char ***)block->work[0];

            for (i = 0; i < iRowsIn; i++)
            {
                for (j = 0; j < iColsIn; j++)
                {
                    int iDigit = GetIparPtrs(block)[3];
                    int iPrec = GetIparPtrs(block)[4];

                    double dblScale = pow((double)10, iPrec);
                    double dblTemp = pdblReal[i + (j * iRowsIn)] * dblScale;
                    double dblValue = C2F(sciround) (&dblTemp) / dblScale;
                    char pstFormat[10];

#if _MSC_VER
                    //"%0.2f"
                    sprintf_s(pstFormat, 10, "%%%d.%df", iDigit, iPrec);
                    sprintf_s(pstConv, 128, pstFormat, dblValue);
#else
                    sprintf(pstFormat, "%%%d.%df", iDigit, iPrec);
                    sprintf(pstConv, pstFormat, dblValue);
#endif
                    pstValue[i][j] = strdup(pstConv);
                }
            }

            AfficheBlock_setValue(block->uid, pstValue, iRowsIn, iColsIn);

            break;

        case Initialization:       //init
            pstValue = (char ***)MALLOC(sizeof(char **) * iRowsIn);

            for (i = 0; i < iRowsIn; i++)
            {
                pstValue[i] = (char **)MALLOC(sizeof(char *) * iColsIn);

                for (j = 0; j < iColsIn; j++)
                {
#if _MSC_VER
                    sprintf_s(pstConv, 128, "%0.2f", 0.0);
#else
                    sprintf(pstConv, "%0.2f", 0.0);
#endif
                    pstValue[i][j] = strdup(pstConv);
                }
            }

            if (AfficheBlock_setValue(block->uid, pstValue, iRowsIn, iColsIn) == 0)
            {
                // storing the allocated area on the block work field.
                block->work[0] = pstValue;
            }
            else
            {
                for (i = 0; i < iRowsIn; i++)
                {
                    FREE(pstValue[i]);
                }
                FREE(pstValue);
            }
            break;

        case Ending:
            // Getting the allocated area
            pstValue = (char ***)block->work[0];

            for (i = 0; i < iRowsIn; i++)
            {
                FREE(pstValue[i]);
            }
            FREE(pstValue);
            break;

        default:
            break;
    }
}

//
