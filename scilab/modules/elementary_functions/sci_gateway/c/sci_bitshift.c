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
#include <string.h>
#include "gw_elementary_functions.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
// Shortcuts used to deal with the bitshift(mat, mat), bitshift(scal, mat) and bitshift(mat, scal) cases in just one case
// For instance, bitshift(x, y): if x is a vector and y a scalar, we iterate only on x because Min(i, mn1-1) will select i
//whereas Min(i, mn2-1) will select mn2-1 (=0).
#define MatOrScal1 Min(i, mn1-1)
#define MatOrScal2 Min(i, mn2-1)
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
    int *arg2 = NULL;
    int m2 = 0, n2 = 0, iType2 = 0, iPrec2 = 0;
    int *piAddressVarThree = NULL;
    int m3 = 0, n3 = 0, l3 = 0;
    char *str1 = NULL;

    // Variables used when arg2 is bigger than arg1
    double *dres = NULL;
    char *cres = NULL;
    short *sres = NULL;
    int *ires = NULL;
    unsigned char *ucres = NULL;
    unsigned short *usres = NULL;
    unsigned int *uires = NULL;
    int M = 0, N = 0; // Size of the result

    int i = 0, j = 0;
    int temp = 0;
    unsigned int utemp = 0;
    int drop = 0; // Flag controlling the overflow. If argument #3 is absent, 'drop' defaults to 0, wrapping the integers
    int mn1 = 0, mn2 = 0, maxmn = 0;

    CheckInputArgument(pvApiCtx, 2, 3);
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
        if (isVarComplex(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
            return 0;
        }
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
                if ((dbl1[i + m1 * j] != (int) dbl1[i + m1 * j]) || (abs(dbl1[i + m1 * j]) > 2147483647))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value between %s and %s expected.\n"), fname, 1, "-2147483648", "2147483647");
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
                break;
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
        if (isVarComplex(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 2);
            return 0;
        }
        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &dbl2);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
        for (i = 0; i < m2; ++i)
        {
            for (j = 0; j < n2; ++j)
            {
                if (dbl2[i + m2 * j] != (int) dbl2[i + m2 * j])
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                    free(arg2);
                    return 0;
                }
                else
                {
                    arg2[i + m2 * j] = (int) dbl2[i + m2 * j];
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

        switch (iPrec2)
        {
            case SCI_INT8 :
                sciErr = getMatrixOfInteger8(pvApiCtx, piAddressVarTwo, &m2, &n2, &c2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
                for (i = 0; i < m2 * n2; ++i)
                {
                    arg2[i] = (int) c2[i];
                }
                break;
            case SCI_INT16 :
                sciErr = getMatrixOfInteger16(pvApiCtx, piAddressVarTwo, &m2, &n2, &s2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
                for (i = 0; i < m2 * n2; ++i)
                {
                    arg2[i] = (int) s2[i];
                }
                break;
            case SCI_INT32 :
                sciErr = getMatrixOfInteger32(pvApiCtx, piAddressVarTwo, &m2, &n2, &i2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
                for (i = 0; i < m2 * n2; ++i)
                {
                    arg2[i] = (int) i2[i];
                }
                break;
            case SCI_UINT8 :
                sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddressVarTwo, &m2, &n2, &uc2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
                for (i = 0; i < m2 * n2; ++i)
                {
                    arg2[i] = (int) uc2[i];
                }
                break;
            case SCI_UINT16 :
                sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, piAddressVarTwo, &m2, &n2, &us2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
                for (i = 0; i < m2 * n2; ++i)
                {
                    arg2[i] = (int) us2[i];
                }
                break;
            case SCI_UINT32 :
                sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddressVarTwo, &m2, &n2, &ui2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                arg2 = (int *) MALLOC(sizeof(int) * m2 * n2);
                for (i = 0; i < m2 * n2; ++i)
                {
                    arg2[i] = (int) ui2[i];
                }
                break;
            default:
                break;
        }
    }

    //----------------------------------------------------------------
    // Retrieving argument #3

    if (Rhs == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            free(arg2);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarThree))
        {
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &l3, NULL);
            if (m3 != 1 || n3 != 1)
            {
                Scierror(231, _("%s: Wrong size for input argument #%d: Single string expected.\n"), fname, 3);
                free(arg2);
                return 0;
            }
            str1 = (char*) malloc(sizeof(char*) * m3 * n3);
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &l3, &str1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                free(arg2);
                return sciErr.iErr;
            }
            if (strcmp(str1, "drop") == 0)
            {
                drop = 1; // Drop mode, we will drop the overflowing bits
            }
            else
            {
                if (strcmp(str1, "wrap") != 0) // Otherwise, leave the default mode to "wrap"
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: %s or %s expected.\n"), fname, 3, "drop", "wrap");
                    free(arg2);
                    free(str1);
                    return 0;
                }
            }
        }
        else
        {
            Scierror(231, _("%s: Wrong type for input argument #%d: Single string expected.\n"), fname, 3);
            free(arg2);
            return 0;
        }

    }

    //----------------------------------------------------------------
    // Proceeding to the shifting
    //
    // In every case, use the 'temp' variable to store the unwrapped and undropped result.
    // Then, if we are in 'drop' mode, set it to the maximum (resp minimum) value of the concerned integer type if overflow (resp underflow),
    // or wrap it (default mode) by simply casting it to the concerned integer type.

    mn1 = m1 * n1;
    mn2 = m2 * n2;
    M = Max(m1, m2);
    N = Max(n1, n2);
    maxmn = Max(mn1, mn2);

    if ((m1 == m2 && n1 == n2) || (m1 == 1 && n1 == 1) || (m2 == 1 && n2 == 1)) // bitshift(mat, mat), bitshift(scal, mat) or bitshift(mat, scal)
    {
        if (iType1 == 1)
        {
            dres = (double *) MALLOC(sizeof(double) * maxmn);
            for (i = 0; i < maxmn; ++i)
            {
                if (arg2[MatOrScal2] >= 0)
                {
                    temp = ((int) dbl1[MatOrScal1]) << arg2[MatOrScal2]; // Multiply dbl1[i] by 2^dbl2[i]
                }
                else
                {
                    temp = ((int) dbl1[MatOrScal1]) >> -arg2[MatOrScal2]; // Divide dbl1[i] by 2^-dbl2[i]
                    if (((int) dbl1[MatOrScal1]) % 2 == -1)
                    {
                        temp++;    // If first argument is odd and negative, then round it to the superior value
                    }
                }
                if (drop == 1) // 'Drop' mode
                {
                    if (temp > 2147483647) // Maximum value of a 32 bits integer
                    {
                        temp = 2147483647;
                    }
                    else
                    {
                        if (temp < -2147483647) // Minimum value
                        {
                            temp = -2147483648;
                        }
                    }
                };
                dres[i] = (double) temp;
            }
        }
        else
            switch (iPrec1)
            {

                case SCI_INT8 :

                    cres = (char *) MALLOC(sizeof(char) * maxmn); // Allocate the result variable
                    for (i = 0; i < maxmn; ++i)
                    {
                        if (arg2[MatOrScal2] >= 0)
                        {
                            temp = c1[MatOrScal1] << arg2[MatOrScal2]; // Multiply c1[i] by 2^c2[i]
                        }
                        else
                        {
                            temp = c1[MatOrScal1] >> -arg2[MatOrScal2]; // Divide c1[i] by 2^-c2[i]
                            if (c1[MatOrScal1] % 2 == -1)
                            {
                                temp++;    // If first argument is odd and negative, then round it to the superior value
                            }
                        }
                        if (drop == 1) // 'Drop' mode
                        {
                            if (temp > 127) // Maximum value of an 8 bits integer
                            {
                                temp = 127;
                            }
                            else
                            {
                                if (temp < -128) // Minimum value
                                {
                                    temp = -128;
                                }
                            }
                        };
                        cres[i] = (char) temp;
                    }
                    break;

                case SCI_INT16 :

                    sres = (short *) MALLOC(sizeof(short) * maxmn); // Allocate the result variable
                    for (i = 0; i < maxmn; ++i)
                    {
                        if (arg2[MatOrScal2] >= 0)
                        {
                            temp = s1[MatOrScal1] << arg2[MatOrScal2]; // Multiply s1[i] by 2^s2[i]
                        }
                        else
                        {
                            temp = s1[MatOrScal1] >> -arg2[MatOrScal2]; // Divide s1[i] by 2^-s2[i]
                            if (s1[MatOrScal1] % 2 == -1)
                            {
                                temp++;    // If first argument is odd and negative, then round it to the superior value
                            }
                        }
                        if (drop == 1) // 'Drop' mode
                        {
                            if (temp > 32767) // Maximum value of a 16 bits integer
                            {
                                temp = 32767;
                            }
                            else
                            {
                                if (temp < -32768) // Minimum value
                                {
                                    temp = -32768;
                                }
                            }
                        };
                        sres[i] = (short) temp;
                    }
                    break;

                case SCI_INT32 :

                    ires = (int *) MALLOC(sizeof(int) * maxmn); // Allocate the result variable
                    for (i = 0; i < maxmn; ++i)
                    {
                        if (arg2[MatOrScal2] >= 0)
                        {
                            temp = i1[MatOrScal1] << arg2[MatOrScal2]; // Multiply i1[i] by 2^i2[i]
                        }
                        else
                        {
                            temp = i1[MatOrScal1] >> -arg2[MatOrScal2]; // Divide i1[i] by 2^-i2[i]
                            if (i1[MatOrScal1] % 2 == -1)
                            {
                                temp++;    // If first argument is odd and negative, then round it to the superior value
                            }
                        }
                        if (drop == 1) // 'Drop' mode
                        {
                            if (temp > 2147483647) // Maximum value of a 32 bits integer
                            {
                                temp = 2147483647;
                            }
                            else
                            {
                                if (temp < -2147483647) // Minimum value
                                {
                                    temp = -2147483648;
                                }
                            }
                        };
                        ires[i] = temp;
                    }
                    break;

                case SCI_UINT8 :

                    ucres = (unsigned char *) MALLOC(sizeof(unsigned char) * maxmn); // Allocate the result variable
                    for (i = 0; i < maxmn; ++i)
                    {
                        utemp = uc1[MatOrScal1] << arg2[MatOrScal2]; // Multiply uc1[i] by 2^uc2[i]
                        if (drop == 1 && utemp > 255) // 'Drop' mode and maximum value of an unsigned 8 bits integer
                        {
                            utemp = 255;
                        }
                        ucres[i] = (unsigned char) utemp;
                    }
                    break;

                case SCI_UINT16 :

                    usres = (unsigned short *) MALLOC(sizeof(unsigned short) * maxmn); // Allocate the result variable
                    for (i = 0; i < maxmn; ++i)
                    {
                        utemp = us1[MatOrScal1] << arg2[MatOrScal2]; // Multiply us1[i] by 2^us2[i]
                        if (drop == 1 && utemp > 65535) // 'Drop' mode and maximum value of an unsigned 16 bits integer
                        {
                            utemp = 65535;
                        }
                        usres[i] = (unsigned short) utemp;
                    }
                    break;

                case SCI_UINT32 :

                    uires = (unsigned int *) MALLOC(sizeof(unsigned int) * maxmn); // Allocate the result variable
                    for (i = 0; i < maxmn; ++i)
                    {
                        utemp = ui1[MatOrScal1] << arg2[MatOrScal2]; // Multiply ui1[i] by 2^ui2[i]
                        if (drop == 1 && utemp > 4294967295) // 'Drop' mode and maximum value of an unsigned 32 bits integer
                        {
                            utemp = 4294967295;
                        }
                        uires[i] = utemp;
                    }
                    break;

                default:
                    break;
            }
    }
    else
    {
        Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), fname);
        free(arg2);
        if (Rhs == 3)
        {
            free(str1);
        }
        return 0;
    }

    //----------------------------------------------------------------
    // Returning the result
    //

    if (iType1 == 1)
    {
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, M, N, dres);
        free(dres);
    }
    else
        switch (iPrec1)
        {
            case SCI_INT8 :
                sciErr = createMatrixOfInteger8(pvApiCtx, Rhs + 1, M, N, cres);
                free(cres);
                break;
            case SCI_INT16 :
                sciErr = createMatrixOfInteger16(pvApiCtx, Rhs + 1, M, N, sres);
                free(sres);
                break;
            case SCI_INT32 :
                sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 1, M, N, ires);
                free(ires);
                break;
            case SCI_UINT8 :
                sciErr = createMatrixOfUnsignedInteger8(pvApiCtx, Rhs + 1, M, N, ucres);
                free(ucres);
                break;
            case SCI_UINT16 :
                sciErr = createMatrixOfUnsignedInteger16(pvApiCtx, Rhs + 1, M, N, usres);
                free(usres);
                break;
            case SCI_UINT32 :
                sciErr = createMatrixOfUnsignedInteger32(pvApiCtx, Rhs + 1, M, N, uires);
                free(uires);
                break;
            default:
                break;
        }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        free(arg2);
        if (Rhs == 3)
        {
            free(str1);
        }
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    free(arg2);
    if (Rhs == 3)
    {
        free(str1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
