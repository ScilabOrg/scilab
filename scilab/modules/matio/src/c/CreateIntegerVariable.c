/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

int CreateIntegerVariable(void *pvApiCtx, int iVar, int integerType, matvar_t *matVariable, int * parent, int item_position)
{
    int nbRow, nbCol, i;
    SciErr sciErr;
    char * tmp_int8 = NULL;
    short * tmp_int16 = NULL;
    int * tmp_int32 = NULL;
    int *piDims = NULL;
    unsigned char * tmp_uint8 = NULL;
    unsigned short * tmp_uint16 = NULL;
    unsigned int * tmp_uint32 = NULL;
#ifdef __SCILAB_INT64__
    long long * tmp_int64 = NULL;
    unsigned long long * tmp_uint64 = NULL;
#endif

    // Matrix dimensions
    nbRow = (int)matVariable->dims[0];
    nbCol = (int)matVariable->dims[1];

    if (matVariable->rank == 2) /* 2-D array */
    {
        switch (integerType)
        {
            case SCI_INT8:
                if (nbRow * nbCol)
                {
                    tmp_int8 = (char *)MALLOC(nbRow * nbCol * sizeof(char));
                    if (tmp_int8 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_int8[i] = ((char *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfInteger8(pvApiCtx, iVar, nbRow, nbCol, tmp_int8);
                }
                else
                {
                    sciErr = createMatrixOfInteger8InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int8);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_int8);
                break;
            case SCI_INT16:
                if (nbRow * nbCol)
                {
                    tmp_int16 = (short *)MALLOC(nbRow * nbCol * sizeof(short));
                    if (tmp_int16 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_int16[i] = ((short *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfInteger16(pvApiCtx, iVar, nbRow, nbCol, tmp_int16);
                }
                else
                {
                    sciErr = createMatrixOfInteger16InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int16);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_int16);
                break;
            case SCI_INT32:
                if (nbRow * nbCol)
                {
                    tmp_int32 = (int *)MALLOC(nbRow * nbCol * sizeof(int));
                    if (tmp_int32 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_int32[i] = ((int *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfInteger32(pvApiCtx, iVar, nbRow, nbCol, tmp_int32);
                }
                else
                {
                    sciErr = createMatrixOfInteger32InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int32);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_int32);
                break;
#ifdef __SCILAB_INT64__
            case SCI_INT64:
                if (nbRow * nbCol)
                {
                    tmp_int64 = (long long *)MALLOC(nbRow * nbCol * sizeof(long long));
                    if (tmp_int64 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_int64[i] = ((long long *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfInteger64(pvApiCtx, iVar, nbRow, nbCol, tmp_int64);
                }
                else
                {
                    sciErr = createMatrixOfInteger64InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_int64);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_int64);
                break;
#endif
            case SCI_UINT8 :
                if (nbRow * nbCol)
                {
                    tmp_uint8 = (unsigned char *)MALLOC(nbRow * nbCol * sizeof(unsigned char));
                    if (tmp_uint8 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_uint8[i] = ((unsigned char *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfUnsignedInteger8(pvApiCtx, iVar, nbRow, nbCol, tmp_uint8);
                }
                else
                {
                    sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint8);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_uint8);
                break;
            case SCI_UINT16:
                if (nbRow * nbCol)
                {
                    tmp_uint16 = (unsigned short *)MALLOC(nbRow * nbCol * sizeof(unsigned short));
                    if (tmp_uint16 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_uint16[i] = ((unsigned short *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfUnsignedInteger16(pvApiCtx, iVar, nbRow, nbCol, tmp_uint16);
                }
                else
                {
                    sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint16);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_uint16);
                break;
            case SCI_UINT32:
                if (nbRow * nbCol)
                {
                    tmp_uint32 = (unsigned int *)MALLOC(nbRow * nbCol * sizeof(unsigned int));
                    if (tmp_uint32 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_uint32[i] = ((unsigned int *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfUnsignedInteger32(pvApiCtx, iVar, nbRow, nbCol, tmp_uint32);
                }
                else
                {
                    sciErr = createMatrixOfUnsignedInteger32InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint32);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_uint32);
                break;
#ifdef __SCILAB_INT64__
            case SCI_UINT64:
                if (nbRow * nbCol)
                {
                    tmp_uint64 = (unsigned long long *)MALLOC(nbRow * nbCol * sizeof(unsigned long long));
                    if (tmp_uint64 == NULL)
                    {
                        Scierror(999, _("%s: No more memory.\n"), "CreateIntegerVariable");
                        return FALSE;
                    }
                    for (i = 0; i < nbRow * nbCol; i++)
                    {
                        tmp_uint64[i] = ((unsigned long long *)matVariable->data)[i];
                    }
                }

                if (parent == NULL)
                {
                    sciErr = createMatrixOfUnsignedInteger64(pvApiCtx, iVar, nbRow, nbCol, tmp_uint64);
                }
                else
                {
                    sciErr = createMatrixOfUnsignedInteger64InList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, tmp_uint64);
                }

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                FREE(tmp_uint64);
                break;
#endif
        }
    }
    else /* Multi-dimension array -> Scilab HyperMatrix */
    {
        piDims = (int*) MALLOC(matVariable->rank * sizeof(int));
        if (piDims == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "CreateBooleanVariable");
            return FALSE;
        }
        for (i = 0; i < matVariable->rank; i++)
        {
            piDims[i] = (int)matVariable->dims[i];
        }

        //CreateHyperMatrixVariable(pvApiCtx, iVar, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,  &integerType, &matVariable->rank,
        //                          piDims, (double*)matVariable->data, NULL, parent, item_position);

        FREE(piDims);
    }

    return TRUE;
}
