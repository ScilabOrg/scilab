/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
* Copyright (C) 2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include "dynlib_scicos_blocks.h"
#include "assert.h"

/**
 * Check the size of the output and inputs
 */
#ifndef NDEBUG
static void check_size(int insz[], int nin, int outsz[]);
#endif

SCICOS_BLOCKS_IMPEXP void sum(int *flag, int *nevprt, double *t, double xd[],
                              double x[], int *nx, double z[], int *nz,
                              double tvec[], int *ntvec, double rpar[],
                              int *nrpar, int ipar[], int *nipar,
                              double *inptr[], int insz[], int *nin,
                              double *outptr[], int outsz[], int *nout)
{
    /*
     * Pre conditions
     */
    if (nin == 0 || nout == 0 || *nout != 1)
    {
        // not a data case
        return;
    }
#ifndef NDEBUG
    check_size(insz, *nin, outsz);
#endif

    double *y = (double *)outptr[0];

    int n = outsz[0];           /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

    int i;

    for (i = 0; i < n; i++)
    {
        y[i] = 0.0;

        int k;

        for (k = 0; k < *nin; k++)
        {
            double *u = (double *)inptr[k];

            // if rpar exist at the index then use it, use 1 otherwise
            double gain;

            if (k < *nrpar)
                gain = rpar[k];
            else
                gain = 1.0;

            y[i] = y[i] + u[i] * gain;
        }
    }
}

#ifndef NDEBUG
static void check_size(int insz[], int nin, int outsz[])
{
    int j;

    for (j = 0; j < nin; j += 2)
    {
        assert(outsz[0] == insz[j]);
        assert(outsz[1] == insz[nin + j]);
    }
}
#endif

/*--------------------------------------------------------------------------*/
