/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <stdio.h>
#include <string.h>
#include "matrix_addition.h"
#include "core_math.h"
#include "operations_tools.h"
#include "elem_common.h"

/* SinglePoly + SinglePoly*/
int iAddScilabPolynomToScilabPolynom(double* _pCoef1R, int _iRank1, double* _pCoef2R, int _iRank2, double* _pCoefOutR, int _iRanOut)
{
    int iRankMin			= Min(_iRank1, _iRank2);
    int iRankMax			= Max(_iRank1, _iRank2);
    int iRank				= 0;
    double dblSum 			= 0.0;
    double* pCoefMaxR	= (_iRank1 > _iRank2) ? _pCoef1R : _pCoef2R;

    for (iRank = 0; iRank < iRankMin ; iRank++)
    {
        dblSum = _pCoef1R[iRank] + _pCoef2R[iRank];
        if (fabs(dblSum) > Max(fabs(_pCoef1R[iRank]), fabs(_pCoef2R[iRank])) * 2 * nc_eps())
        {
            _pCoefOutR[iRank] = dblSum;
        }
        else
        {
            _pCoefOutR[iRank] = 0.0;
        }
    }

    for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
    {
        _pCoefOutR[iRank] = pCoefMaxR[iRank];
    }
    return 0;
}

int iAddScilabPolynomToComplexPoly(double* _pCoef1R, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut)
{
    int iRankMin			= Min(_iRank1, _iRank2);
    int iRankMax			= Max(_iRank1, _iRank2);
    int iRank					= 0;
    double* pCoefMaxR	= (_iRank1 > _iRank2) ? _pCoef1R : _pCoef2R;
    double* pCoefMaxI	= NULL;

    for (iRank = 0; iRank < iRankMin ; iRank++)
    {
        _pCoefOutR[iRank] = _pCoef1R[iRank] + _pCoef2R[iRank];
        _pCoefOutI[iRank] = _pCoef2I[iRank];
    }

    if (_iRank1 > _iRank2)
    {
        for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
        {
            _pCoefOutR[iRank] = pCoefMaxR[iRank];
            _pCoefOutI[iRank] = 0;
        }
    }
    else
    {
        for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
        {
            _pCoefOutR[iRank] = pCoefMaxR[iRank];
            _pCoefOutI[iRank] = _pCoef2I[iRank];
        }
    }
    return 0;
}

int iAddComplexPolyToComplexPoly(double* _pCoef1R, double* _pCoef1I, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut)
{
    int iRankMin			= Min(_iRank1, _iRank2);
    int iRankMax			= Max(_iRank1, _iRank2);
    int iRank					= 0;
    double* pCoefMaxR	= (_iRank1 > _iRank2) ? _pCoef1R : _pCoef2R;
    double* pCoefMaxI	= (_iRank1 > _iRank2) ? _pCoef1I : _pCoef2I;

    for (iRank = 0; iRank < iRankMin ; iRank++)
    {
        _pCoefOutR[iRank] = _pCoef1R[iRank] + _pCoef2R[iRank];
        _pCoefOutI[iRank] = _pCoef1I[iRank] + _pCoef2I[iRank];
    }

    for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
    {
        _pCoefOutR[iRank] = pCoefMaxR[iRank];
        _pCoefOutI[iRank] = pCoefMaxI[iRank];
    }
    return 0;
}

