/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "gsort.h"
#include "string.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "sciprint.h"
#include "warningmode.h"
#include "isanan.h"

/* Next define intruction put here instead of stack-c.h waiting for
 more genral usage stack-c.h macros CreateVarFromPtr and CreateVar
 contains a "return 0" which make impossible to free indices in case
 of error
*/
#define CreateVarFromPtrNoCheck(n,ct,mx,nx,lx) C2F(createvarfromptr)((c_local=n,&c_local),ct,mx,nx,(void *)lx,1L)
#define CreateVarNoCheck(n,ct,mx,nx,lx) C2F(createvar)((c_local=n,&c_local),ct,mx,nx,(void *)lx, 1L)
/*--------------------------------------------------------------------------*/
static int gsort_complex(char *fname, char *mode, char *order);

/*--------------------------------------------------------------------------*/
int sci_gsort(char *fname, unsigned long fname_len)
{
    char iord[2];
    char typex[10];
    SciIntMat Im;
    int Type = 0;
    char **S = NULL;
    int m1 = 0, n1 = 0, l1 = 0;
    int m2 = 0, n2 = 0, l2 = 0;
    int m3 = 0, n3 = 0, l3 = 0;
    int ind_m1 = 0, ind_n1 = 0;
    int *indices = NULL;
    int iflag = 0;
    int i;

    iord[0] = DECREASE_COMMAND;
    iord[1] = '\0';

    typex[0] = GLOBAL_SORT;
    typex[1] = '\0';

    Rhs = Max(0, Rhs);
    CheckRhs(1, 3);
    CheckLhs(1, 2);

    if (Rhs >= 1)
    {
        Type = VarType(1);
        switch (Type)
        {
            case sci_strings:
                GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &S);
                break;
            case sci_matrix:
            {
#define COMPLEX 1
                int *header = NULL;
                int Cmplx = 0;

                header = (int *)GetData(1);
                Cmplx = header[3];

                if (Cmplx == COMPLEX)
                {
                    return gsort_complex(fname, typex, iord);
                }
                else
                {
                    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
                    if ((m1 * n1) == 0) /* [] returns [] */
                    {
                        int m = 0, n = 0, l = 0;

                        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
                        LhsVar(1) = Rhs + 1;
                        if (Lhs == 2)
                        {
                            CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
                            LhsVar(2) = Rhs + 2;
                        }
                        PutLhsVar();
                        return 0;
                    }
                }
            }
            break;
            case sci_ints:
                GetRhsVar(1, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m1, &n1, &Im);
                break;
            case sci_sparse:
            default:
                OverLoad(1);
                return 0;
                break;
        }
    }

    if (Rhs == 3)
    {
        int* piAddr = NULL;
        char* pstData = NULL;

        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr, &pstData))
        {
            return 1;
        }

        if (strcmp(pstData, "i") && strcmp(pstData, "d"))
        {
            if (getWarningMode())
            {
                sciprint(_("WARNING: %s\n"), _("This usage of the third argument of gsort is obsolete."));
                sciprint(_("WARNING: %s\n"), _("It will no more be available in Scilab 6."));
                sciprint(_("WARNING: %s\n"), _("Please use 'd' or 'i' instead."));
            }
        }

        iord[0] = pstData[0];
        freeAllocatedSingleString(pstData);
    }

    if (Rhs >= 2)
    {
        int* piAddr = NULL;
        char* pstData = NULL;

        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr, &pstData))
        {
            if (pstData)
            {
                freeAllocatedSingleString(pstData);
            }

            return 1;
        }

        if ((pstData[0] != ROW_SORT) && (pstData[0] != COLUMN_SORT) && (pstData[0] != GLOBAL_SORT) && (pstData[0] != LIST_SORT))
        {
            FREE(pstData);
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s', '%s' or '%s' expected.\n"), fname, 2, "r", "c", "g", "lr", "lc");
            return 1;
        }

        if (strcmp(pstData, "c") && strcmp(pstData, "r") && strcmp(pstData, "g") && strcmp(pstData, "lc") && strcmp(pstData, "lr"))
        {
            if (getWarningMode())
            {
                sciprint(_("WARNING: %s\n"), _("This usage of the second argument of gsort is obsolete."));
                sciprint(_("WARNING: %s\n"), _("It will no more be available in Scilab 6."));
                sciprint(_("WARNING: %s\n"), _("Please use 'r', 'c', 'g', 'lr' or 'lc' instead."));
            }
        }

        strcpy(typex, pstData);
        freeAllocatedSingleString(pstData);
    }

    if (typex[0] == LIST_SORT)
    {
        if (typex[1] == ROW_SORT)
        {
            ind_m1 = m1;
            ind_n1 = 1;
            if (ind_m1 != 0)
            {
                indices = (int *)MALLOC(sizeof(int) * (ind_m1));    /* Only return in row */
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
        else if (typex[1] == COLUMN_SORT)
        {
            ind_m1 = 1;
            ind_n1 = n1;
            if (ind_n1 != 0)
            {
                indices = (int *)MALLOC(sizeof(int) * (ind_n1));    /*Only return in col */
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
            return 0;
        }
    }
    else
    {
        ind_m1 = m1;
        ind_n1 = n1;
        if (ind_m1 * ind_n1 != 0)
        {
            indices = (int *)MALLOC(sizeof(int) * (ind_m1 * ind_n1));    /* return a matrix */
        }
    }

    if (Lhs == 2)
    {
        iflag = 1;
    }
    else
    {
        iflag = 0;
    }

    switch (Type)
    {
        case sci_matrix:
        {
            if (m1 * n1 != 0)
            {
                int lr;
                double *matrix = stk(l1);
                double *tmp_matrix = NULL;

                /* next CreateVar and corresponding copy not needed if arg1 is not passed by reference */
                if (!CreateVarNoCheck(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &lr))
                {
                    if (indices)
                    {
                        FREE(indices);
                        indices = NULL;
                    }
                    return 0;
                }

                tmp_matrix = stk(lr);
                for (i = 0; i < m1 * n1; i++)
                {
                    tmp_matrix[i] = matrix[i];
                }

                C2F(gsortd) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                LhsVar(1) = Rhs + 1;

                if (Lhs == 2)
                {
                    if (!CreateVarFromPtrNoCheck(Rhs + 2, MATRIX_OF_INTEGER_DATATYPE, &ind_m1, &ind_n1, &indices))
                    {
                        if (indices)
                        {
                            FREE(indices);
                            indices = NULL;
                        }
                        return 0;
                    }
                    LhsVar(2) = Rhs + 2;
                }
                if (indices)
                {
                    FREE(indices);
                    indices = NULL;
                }
                PutLhsVar();
            }
        }
        break;

        case sci_ints:
        {
            int lr;

            lr = Im.it;
            /* next CreateVar and corresponding copy not needed if arg1 is not passed by reference */
            if (!CreateVarNoCheck(Rhs + 1, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m1, &n1, &lr))
            {
                if (indices)
                {
                    FREE(indices);
                    indices = NULL;
                }
                return 0;
            }

            switch (Im.it)      /* Type defined in stack-c.h */
            {
                case I_CHAR:
                {
                    char *matrix = Im.D;
                    char *tmp_matrix = (char *)istk(lr);

                    for (i = 0; i < m1 * n1; i++)
                    {
                        tmp_matrix[i] = matrix[i];
                    }
                    C2F(gsortchar) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                }
                break;

                case I_INT32:
                {
                    int *matrix = Im.D;
                    int *tmp_matrix = istk(lr);

                    for (i = 0; i < m1 * n1; i++)
                    {
                        tmp_matrix[i] = matrix[i];
                    }
                    C2F(gsortint) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                }
                break;
                case I_UCHAR:
                {
                    unsigned char *matrix = Im.D;
                    unsigned char *tmp_matrix = (unsigned char *)istk(lr);

                    for (i = 0; i < m1 * n1; i++)
                    {
                        tmp_matrix[i] = matrix[i];
                    }
                    C2F(gsortuchar) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                }
                break;
                case I_INT16:
                {
                    short *matrix = Im.D;
                    short *tmp_matrix = (short *)istk(lr);

                    for (i = 0; i < m1 * n1; i++)
                    {
                        tmp_matrix[i] = matrix[i];
                    }
                    C2F(gsortshort) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                }
                break;
                case I_UINT16:
                {
                    unsigned short *matrix = Im.D;
                    unsigned short *tmp_matrix = (short *)istk(lr);

                    for (i = 0; i < m1 * n1; i++)
                    {
                        tmp_matrix[i] = matrix[i];
                    }
                    C2F(gsortushort) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                }
                break;
                case I_UINT32:
                {
                    unsigned int *matrix = Im.D;
                    unsigned int *tmp_matrix = (unsigned int *)istk(lr);

                    for (i = 0; i < m1 * n1; i++)
                    {
                        tmp_matrix[i] = matrix[i];
                    }
                    C2F(gsortuint) (tmp_matrix, indices, &iflag, &m1, &n1, typex, iord);
                }
                break;
                default:
                    if (indices)
                    {
                        FREE(indices);
                        indices = NULL;
                    }
                    Scierror(999, _("%s: Wrong type for input argument #%d: Unknown type.\n"), fname, 1);
                    return 0;
            }

            LhsVar(1) = Rhs + 1;

            if (Lhs == 2)
            {
                if (!CreateVarFromPtrNoCheck(Rhs + 2, MATRIX_OF_INTEGER_DATATYPE, &ind_m1, &ind_n1, &indices))
                {
                    if (indices)
                    {
                        FREE(indices);
                        indices = NULL;
                    }
                    return 0;
                }
                LhsVar(2) = Rhs + 2;
            }
            if (indices)
            {
                FREE(indices);
                indices = NULL;
            }
            PutLhsVar();
        }
        break;

        case sci_strings:
        {
            C2F(gsorts) (S, indices, &iflag, &m1, &n1, typex, iord);
            if (!CreateVarFromPtrNoCheck(Rhs + 1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, S))
            {
                if (indices)
                {
                    FREE(indices);
                    indices = NULL;
                }
                return 0;
            }
            LhsVar(1) = Rhs + 1;

            if (Lhs == 2)
            {
                if (!CreateVarFromPtrNoCheck(Rhs + 2, MATRIX_OF_INTEGER_DATATYPE, &ind_m1, &ind_n1, &indices))
                {
                    if (indices)
                    {
                        FREE(indices);
                        indices = NULL;
                    }
                    return 0;
                }
                LhsVar(2) = Rhs + 2;
            }
            if (indices)
            {
                FREE(indices);
                indices = NULL;
            }
            freeArrayOfString(S, m1 * n1);
            PutLhsVar();
        }
        break;

        default:
            if (indices)
            {
                FREE(indices);
                indices = NULL;
            }
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
            return 0;
            break;
    }
    return 0;
}

/*-----------------------------------------------------------------------------------*/
static int gsort_complex(char *fname, char *mode, char *order)
{
    int lw = 0;

    C2F(overload) (&lw, "gsort", (unsigned long)strlen("gsort"));
    return 0;
}

/*-----------------------------------------------------------------------------------*/
