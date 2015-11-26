/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"

const char fname[] = "double_test";

int sci_double_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt* opt, int nout, scilabVar* out)
{
    int i = 0;
    int inr1 = 0;
    int inc1 = 0;
    int size1 = 0;
    double* in1 = NULL;

    double in2 = 0;

    double* out1 = NULL;

    int* out2dims = NULL;
    double* out2 = NULL;

    double out3 = 0;

    if (nin != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    if (nout != 3)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 3);
        return STATUS_ERROR;
    }

    //in[0] : matrix 2d of double
    if (scilab_isDouble(env, in[0]) == 0 || scilab_isMatrix(env, in[0]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A double matrix expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    size1 = scilab_getDim2d(env, in[0], &inr1, &inc1);
    scilab_getDoubleArray(env, in[0], &in1);

    //in[1] : double
    if (scilab_isDouble(env, in[1]) == 0 || scilab_isScalar(env, in[1]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    scilab_getDouble(env, in[1], &in2);

    //out1 : matrix 2d of double with same size of in[0]
    out[0] = scilab_createDoubleMatrix2d(env, inr1, inc1, 0);
    scilab_getDoubleArray(env, out[0], &out1);

    for (i = 0; i < size1; ++i)
    {
        out1[i] = in1[i] * 10;
    }

    //out2 : 3d matrix of double
    out2dims = (int*)MALLOC(3 * sizeof(int));
    out2dims[0] = inr1;
    out2dims[1] = inc1;
    out2dims[2] = 2;

    out[1] = scilab_createDoubleMatrix(env, 3, out2dims, 0);
    scilab_getDoubleArray(env, out[1], &out2);

    for (i = 0; i < size1; ++i)
    {
        out2[i] = in1[i] * 10;
        out2[i + size1] = in1[i] * 100;
    }

    //out3 : double
    out[2] = scilab_createDouble(env, in2 * 1000);
    return STATUS_OK;
}
