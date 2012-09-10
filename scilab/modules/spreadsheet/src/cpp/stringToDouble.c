/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#if defined(__linux__)
#define _GNU_SOURCE             /* avoid dependency on GLIBC_2.7 */
#endif
// =============================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "core_math.h" /* M_PI */
#include "stringToDouble.h"
#include "MALLOC.h"
#include "csv_default.h"
#ifdef  _MSC_VER
#include "strdup_windows.h"
#endif
#include "nan.h"
#include "inf.h"
// =============================================================================
#ifndef _MSC_VER
#ifndef stricmp
#define stricmp strcasecmp
#endif
#else
#define stricmp _stricmp
#endif
// =============================================================================
double stringToDouble(const char *pSTR,
                      BOOL bConvertByNAN,
                      stringToDoubleError *ierr)
{
    double dValue = 0.0;
    *ierr = STRINGTODOUBLE_ERROR;
    if (pSTR)
    {
        if ((stricmp(pSTR, NanString) == 0) || (stricmp(pSTR, NegNanString) == 0) ||
                (stricmp(pSTR, PosNanString) == 0) || (stricmp(pSTR, ScilabPosNanString) == 0) ||
                (stricmp(pSTR, ScilabNanString) == 0) || (stricmp(pSTR, ScilabNegNanString) == 0))
        {
            dValue = returnNAN();
        }
        else if ((stricmp(pSTR, InfString) == 0) || (stricmp(pSTR, PosInfString) == 0) ||
                 (stricmp(pSTR, ScilabInfString) == 0) || (stricmp(pSTR, ScilabPosInfString) == 0))
        {
            dValue = returnINF(TRUE);
        }
        else if ((stricmp(pSTR, NegInfString) == 0) || (stricmp(pSTR, ScilabNegInfString) == 0))
        {
            dValue = returnINF(FALSE);
        }
        else if ((stricmp(pSTR, ScilabPiString) == 0) || (stricmp(pSTR, ScilabPosPiString) == 0))
        {
            dValue = M_PI;
        }
        else if (stricmp(pSTR, ScilabNegPiString) == 0)
        {
            dValue = -M_PI;
        }
        else
        {
            char *pEnd = NULL;
            double v = strtod(pSTR, &pEnd);
            if ((v == 0) && (pEnd == pSTR))
            {
                if (bConvertByNAN)
                {
                    dValue = returnNAN();
                }
                else
                {
                    *ierr = STRINGTODOUBLE_NOT_A_NUMBER;
                    return (dValue = 0.0);
                }
            }
            else
            {
                if (strcmp(pEnd, "") == 0)
                {
                    dValue = v;
                }
                else
                {
                    if (bConvertByNAN)
                    {
                        dValue = returnNAN();
                    }
                    else
                    {
                        *ierr = STRINGTODOUBLE_NOT_A_NUMBER;
                        return (dValue = 0.0);
                    }
                }
            }
        }
        *ierr = STRINGTODOUBLE_NO_ERROR;
    }
    else
    {
        *ierr = STRINGTODOUBLE_MEMORY_ALLOCATION;
    }
    return dValue;
}
// =============================================================================
