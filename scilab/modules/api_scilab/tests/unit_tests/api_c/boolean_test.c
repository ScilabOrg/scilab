/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"

const char fname[] = "boolean_test";

int sci_boolean_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    int i = 0;
    int inr1 = 0;
    int inc1 = 0;
    int size1 = 0;
    int* in1 = NULL;

    int in2 = 0;

    int* out1 = NULL;

    int* out2dims = NULL;
    int* out2 = NULL;

    int out3 = 0;

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

    //in[0] : matrix 2d of boolean
    if (scilab_isBoolean(env, in[0]) == 0 || scilab_isMatrix2d(env, in[0]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    size1 = scilab_getDim2d(env, in[0], &inr1, &inc1);
    scilab_getBooleanArray(env, in[0], &in1);

    //in[1] : bool
    if (scilab_isBoolean(env, in[1]) == 0 || scilab_isScalar(env, in[1]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    scilab_getBoolean(env, in[1], &in2);

    //out1 : matrix 2d of boolean with same size of in[0]
    out[0] = scilab_createBooleanMatrix2d(env, inr1, inc1);
    scilab_getBooleanArray(env, out[0], &out1);

    for (i = 0; i < size1; ++i)
    {
        out1[i] = in1[i] == 1 ? 0 : 1;
    }

    //out2 : 3d matrix of boolean
    out2dims = (int*)MALLOC(3 * sizeof(int));
    out2dims[0] = inr1;
    out2dims[1] = inc1;
    out2dims[2] = 2;

    out[1] = scilab_createBooleanMatrix(env, 3, out2dims);
    scilab_getBooleanArray(env, out[1], &out2);

    for (i = 0; i < size1; ++i)
    {
        out2[i] = in1[i];
        out2[i + size1] = in1[i] ? 0 : 1;
    }

    //out3 : bool
    out[2] = scilab_createBoolean(env, in2 ? 0 : 1);
    return STATUS_OK;
}
