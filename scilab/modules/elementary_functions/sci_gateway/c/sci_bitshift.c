/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <math.h>
#include "gw_elementary_functions.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_bitshift(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    double *dbl1 = NULL;
    char *c1 = NULL;
    short *s1 = NULL;
    int *i1 = NULL;
    unsigned char *uc1 = NULL;
    unsigned short *us1 = NULL;
    unsigned int *ui1 = NULL;
    int m1 = 0, n1 = 0, iType1 = 0, iPrec1 = 0;
    int *piAddressVarTwo = NULL;
    double *dbl2 = NULL;
    char *c2 = NULL;
    short *s2 = NULL;
    int *i2 = NULL;
    unsigned char *uc2 = NULL;
    unsigned short *us2 = NULL;
    unsigned int *ui2 = NULL;
    int m2 = 0, n2 = 0, iType2 = 0, iPrec2 = 0;

    int i = 0, j = 0;
    int res2 = 0;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //----------------------------------------------------------------
    // Retrieving argument #1

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType1 != 1 && iType1 != 8)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
        return 0;
    }

    if (iType1 == 1)
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &dbl1);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        for (i = 0; i < m1; ++i)
        {
            for (j = 0; j < n1; ++j)
            {
                if (dbl1[i + m1 * j] != (int) dbl1[i + m1 * j])
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                    return 0;
                }
            }
        }

    }
    else
    {
        sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddressVarOne, &iPrec1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        switch (iPrec1)
        {
            case SCI_INT8 :
                sciErr = getMatrixOfInteger8(pvApiCtx, piAddressVarOne, &m1, &n1, &c1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_INT16 :
                sciErr = getMatrixOfInteger16(pvApiCtx, piAddressVarOne, &m1, &n1, &s1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_INT32 :
                sciErr = getMatrixOfInteger32(pvApiCtx, piAddressVarOne, &m1, &n1, &i1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_UINT8 :
                sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddressVarOne, &m1, &n1, &uc1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_UINT16 :
                sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddressVarOne, &m1, &n1, &us1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_UINT32 :
                sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddressVarOne, &m1, &n1, &ui1);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
            default:
                break;
        }
    }

    //----------------------------------------------------------------
    // Retrieving argument #2

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (iType2 != 1 && iType2 != 8)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 2);
        return 0;
    }

    if (iType2 == 1)
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &dbl2);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        for (i = 0; i < m2; ++i)
        {
            for (j = 0; j < n2; ++j)
            {
                if (dbl2[i + m2 * j] != (int) dbl2[i + m2 * j])
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                    return 0;
                }
            }
        }

    }
    else
    {
        sciErr = getMatrixOfIntegerPrecision(pvApiCtx, piAddressVarTwo, &iPrec2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        // Checking that arguments share the same type
        if (iType2 != iType1 || iPrec1 != iPrec2)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Same type as argument #%d expected.\n"), fname, 2, 1);
            return 0;
        }

        switch (iPrec2)
        {
            case SCI_INT8 :
                sciErr = getMatrixOfInteger8(pvApiCtx, piAddressVarTwo, &m2, &n2, &c2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_INT16 :
                sciErr = getMatrixOfInteger16(pvApiCtx, piAddressVarTwo, &m2, &n2, &s2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_INT32 :
                sciErr = getMatrixOfInteger32(pvApiCtx, piAddressVarTwo, &m2, &n2, &i2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_UINT8 :
                sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddressVarTwo, &m2, &n2, &uc2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_UINT16 :
                sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddressVarTwo, &m2, &n2, &us2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                break;
            case SCI_UINT32 :
                sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddressVarTwo, &m2, &n2, &ui2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
            default:
                break;
        }
    }

    //----------------------------------------------------------------
    // Proceeding to the shifting
    //
    // Distinguish 3 cases : batshift(mat, mat), bitshift(scalar, mat) and bitshift(mat, scalar).
    // In every case, shift arg1 by arg2 elementwise.

    if (m1 == m2 && n1 == n2) // bitshift(mat, mat)
        for (i = 0; i < m1 * n1; ++i)
            if (iType1 == 1)
            {
                if (dbl2[i] >= 0)
                {
                    dbl1[i] = ((int) dbl1[i]) << ((int) dbl2[i]); // Multiply dbl1[i] by 2^dbl2[i]
                }
                else
                {
                    dbl1[i] = ((int) dbl1[i]) >> ((int) - dbl2[i]); // Divide dbl1[i] by 2^-dbl2[i]
                }
            }
            else
                switch (iPrec1)
                {
                    case SCI_INT8 :
                        if (c2[i] >= 0)
                        {
                            c1[i] = c1[i] << c2[i]; // Multiply c1[i] by 2^c2[i]
                        }
                        else
                        {
                            c1[i] = c1[i] >> -c2[i]; // Divide c1[i] by 2^-c2[i]
                        }
                        break;
                    case SCI_INT16 :
                        if (s2[i] >= 0)
                        {
                            s1[i] = s1[i] << s2[i]; // Multiply s1[i] by 2^s2[i]
                        }
                        else
                        {
                            s1[i] = s1[i] >> -s2[i]; // Divide s1[i] by 2^-s2[i]
                        }
                        break;
                    case SCI_INT32 :
                        if (i2[i] >= 0)
                        {
                            i1[i] = i1[i] << i2[i]; // Multiply i1[i] by 2^i2[i]
                        }
                        else
                        {
                            i1[i] = i1[i] >> -i2[i]; // Divide i1[i] by 2^-i2[i]
                        }
                        break;
                    case SCI_UINT8 :
                        uc1[i] = uc1[i] << uc2[i]; // Multiply uc1[i] by 2^uc2[i]
                        break;
                    case SCI_UINT16 :
                        us1[i] = us1[i] << us2[i]; // Multiply us1[i] by 2^us2[i]
                        break;
                    case SCI_UINT32 :
                        ui1[i] = ui1[i] << ui2[i]; // Multiply ui1[i] by 2^ui2[i]
                    default:
                        break;
                }
    else
    {
        if (m1 == 1 && n1 == 1) // bitshift(scalar, mat)
        {
            for (i = 0; i < m2 * n2; ++i)
                if (iType1 == 1)
                {
                    if (dbl2[i] >= 0)
                    {
                        dbl2[i] = ((int) * dbl1) << ((int) dbl2[i]); // Multiply dbl1 by 2^dbl2[i]
                    }
                    else
                    {
                        dbl2[i] = ((int) * dbl1) >> ((int) - dbl2[i]); // Divide dbl1 by 2^-dbl2[i]
                    }
                }
                else
                    switch (iPrec1)
                    {
                        case SCI_INT8 :
                            if (c2[i] >= 0)
                            {
                                c2[i] = *c1 << c2[i]; // Multiply c1 by 2^c2[i]
                            }
                            else
                            {
                                c2[i] = *c1 >> -c2[i]; // Divide c1 by 2^-c2[i]
                            }
                            break;
                        case SCI_INT16 :
                            if (s2[i] >= 0)
                            {
                                s2[i] = *s1 << s2[i]; // Multiply s1 by 2^s2[i]
                            }
                            else
                            {
                                s2[i] = *s1 >> -s2[i]; // Divide s1 by 2^-s2[i]
                            }
                            break;
                        case SCI_INT32 :
                            if (i2[i] >= 0)
                            {
                                i2[i] = *i1 << i2[i]; // Multiply i1 by 2^i2[i]
                            }
                            else
                            {
                                i2[i] = *i1 >> -i2[i]; // Divide i1 by 2^-i2[i]
                            }
                            break;
                        case SCI_UINT8 :
                            uc2[i] = *uc1 << uc2[i]; // Multiply uc1 by 2^uc2[i]
                            break;
                        case SCI_UINT16 :
                            us2[i] = *us1 << us2[i]; // Multiply us1 by 2^us2[i]
                            break;
                        case SCI_UINT32 :
                            ui2[i] = *ui1 << ui2[i]; // Multiply ui1 by 2^ui2[i]
                        default:
                            break;
                    }
            res2 = 1; // Boolean to indicate that the result is stored in the second argument.
        }
        else
        {
            if (m2 == 1 && n2 == 1) // bitshift(mat, scalar)
            {
                for (i = 0; i < m1 * n1; ++i)
                    if (iType1 == 1)
                    {
                        if (*dbl2 >= 0)
                        {
                            dbl1[i] = ((int) dbl1[i]) << ((int) * dbl2); // Multiply dbl1[i] by 2^dbl2
                        }
                        else
                        {
                            dbl1[i] = ((int) dbl1[i]) >> ((int) - *dbl2); // Divide dbl1[i] by 2^-dbl2
                        }
                    }
                    else
                        switch (iPrec1)
                        {
                            case SCI_INT8 :
                                if (*c2 >= 0)
                                {
                                    c1[i] = c1[i] << *c2; // Multiply c1[i] by 2^c2
                                }
                                else
                                {
                                    c1[i] = c1[i] >> -*c2; // Divide c1[i] by 2^-c2
                                }
                                break;
                            case SCI_INT16 :
                                if (*s2 >= 0)
                                {
                                    s1[i] = s1[i] << *s2; // Multiply s1[i] by 2^s2
                                }
                                else
                                {
                                    s1[i] = s1[i] >> -*s2; // Divide s1[i] by 2^-s2
                                }
                                break;
                            case SCI_INT32 :
                                if (*i2 >= 0)
                                {
                                    i1[i] = i1[i] << *i2; // Multiply i1[i] by 2^i2
                                }
                                else
                                {
                                    i1[i] = i1[i] >> -*i2; // Divide i1[i] by 2^-i2
                                }
                                break;
                            case SCI_UINT8 :
                                uc1[i] = uc1[i] << *uc2; // Multiply uc1[i] by 2^uc2
                                break;
                            case SCI_UINT16 :
                                us1[i] = us1[i] << *us2; // Multiply us1[i] by 2^us2
                                break;
                            case SCI_UINT32 :
                                ui1[i] = ui1[i] << *ui2; // Multiply ui1[i] by 2^ui2
                            default:
                                break;
                        }
            }
            else
            {
                Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), fname);
                return 0;
            }
        }
    }

    if (res2 == 0) // Result is stored in the first argument. Return it.
    {
        if (iType1 == 1)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, dbl1);
        }
        else
            switch (iPrec1)
            {
                case SCI_INT8 :
                    sciErr = createMatrixOfInteger8(pvApiCtx, Rhs + 1, m1, n1, c1);
                    break;
                case SCI_INT16 :
                    sciErr = createMatrixOfInteger16(pvApiCtx, Rhs + 1, m1, n1, s1);
                    break;
                case SCI_INT32 :
                    sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 1, m1, n1, i1);
                    break;
                case SCI_UINT8 :
                    sciErr = createMatrixOfUnsignedInteger8(pvApiCtx, Rhs + 1, m1, n1, uc1);
                    break;
                case SCI_UINT16 :
                    sciErr = createMatrixOfUnsignedInteger16(pvApiCtx, Rhs + 1, m1, n1, us1);
                    break;
                case SCI_UINT32 :
                    sciErr = createMatrixOfUnsignedInteger32(pvApiCtx, Rhs + 1, m1, n1, ui1);
                default:
                    break;
            }
    }
    else // Result is stored in the second argument. Return it.
    {
        if (iType1 == 1)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, m2, n2, dbl2);
        }
        else
            switch (iPrec1)
            {
                case SCI_INT8 :
                    sciErr = createMatrixOfInteger8(pvApiCtx, Rhs + 2, m2, n2, c2);
                    break;
                case SCI_INT16 :
                    sciErr = createMatrixOfInteger16(pvApiCtx, Rhs + 2, m2, n2, s2);
                    break;
                case SCI_INT32 :
                    sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 2, m2, n2, i2);
                    break;
                case SCI_UINT8 :
                    sciErr = createMatrixOfUnsignedInteger8(pvApiCtx, Rhs + 2, m2, n2, uc2);
                    break;
                case SCI_UINT16 :
                    sciErr = createMatrixOfUnsignedInteger16(pvApiCtx, Rhs + 2, m2, n2, us2);
                    break;
                case SCI_UINT32 :
                    sciErr = createMatrixOfUnsignedInteger32(pvApiCtx, Rhs + 2, m2, n2, ui2);
                default:
                    break;
            }
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1 + res2;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
