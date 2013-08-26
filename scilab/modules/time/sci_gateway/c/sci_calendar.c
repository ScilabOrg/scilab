
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_time.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "IsAScalar.h"
#include "transposeMatrix.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static int isBissextile (unsigned year);
static unsigned months_to_days (unsigned month);
static long years_to_days (unsigned year);
static long ymd_to_scalar (unsigned year, unsigned month, unsigned day);
/*--------------------------------------------------------------------------*/
#define NBRDAY 7
#define NBRWEEK 6
/*--------------------------------------------------------------------------*/
int days[12]    = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*--------------------------------------------------------------------------*/
int sci_calendar(char *fname, unsigned long fname_len)
{
    static int l1, n1, m1;

    int month = 0;
    int year = 0;
    int day, day_1, numdays, i;
    int a = 0;

    int *CALMONTH = NULL;
    int *tmpMatrix = NULL;

    Rhs = Max(0, Rhs);
    CheckRhs(2, 2);
    CheckLhs(1, 1);

    if ( IsAScalar(Rhs - 1) && IsAScalar(Rhs) )
    {
        GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
        year = *istk(l1);

        GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
        month = *istk(l1);

        if ( (year < 1800) || (year > 3000) )
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"), fname, 2, 1800, 3000);
            return 0;
        }

        if ( (month < 1) || (month > 12) )
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"), fname, 1, 1, 12);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input arguments: Scalar values expected.\n"), fname);
        return 0;
    }
    CALMONTH = (int *)MALLOC( (NBRDAY * NBRWEEK) * sizeof(int) );
    for (i = 0; i < NBRDAY * NBRWEEK; i++)
    {
        CALMONTH[i] = 0;
    }

    /* check if the month of feb is 28 or 29 days */
    numdays = days[month - 1];
    if (2 == month && isBissextile(year))
    {
        ++numdays;
    }

    /* Starts the calendar on monday */
    day_1 = (int)((ymd_to_scalar(year, month, 1) - (long)1) % 7L);

    for (day = 0; day < day_1; ++day)
    {
        a++;
    }

    /* Browse all the days */
    for (day = 1; day <= numdays; ++day, ++day_1, day_1 %= 7)
    {
        CALMONTH[a] = day;
        a++;
    }
    m1 = NBRWEEK;
    n1 = NBRDAY;
    tmpMatrix = CALMONTH;

    CALMONTH = transposeMatrixInt(NBRDAY, NBRWEEK, CALMONTH);
    if (tmpMatrix)
    {
        FREE(tmpMatrix);
        tmpMatrix = NULL;
    }

    CreateVarFromPtr(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1 , &CALMONTH);
    LhsVar(1) = Rhs + 1;

    if (CALMONTH)
    {
        FREE(CALMONTH);
        CALMONTH = NULL;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int isBissextile (unsigned year)
{
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}
/*--------------------------------------------------------------------------*/
static unsigned months_to_days (unsigned month)
{
    return (month * 3057 - 3007) / 100;
}
/*--------------------------------------------------------------------------*/
static long years_to_days (unsigned year)
{
    return year * 365L + year / 4 - year / 100 + year / 400;
}
/*--------------------------------------------------------------------------*/
static long ymd_to_scalar (unsigned year, unsigned month, unsigned day)
{
    long scalaire;
    scalaire = day + months_to_days(month);
    if ( month > 2 )
    {
        scalaire -= isBissextile(year) ? 1 : 2;
    }
    year--;
    scalaire += years_to_days(year);
    return scalaire;
}
/*--------------------------------------------------------------------------*/

