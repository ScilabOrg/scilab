/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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
/* Copyright INRIA */
/*--------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void relay(flag, nevprt, t, xd, x, nx, z, nz, tvec, ntvec, rpar, nrpar, ipar, nipar, inptr, insz, nin, outptr, outsz, nout)
     int *flag, *nevprt, *nx, *nz, *ntvec, *nrpar, ipar[], *nipar, insz[], *nin, outsz[], *nout;
     double x[], xd[], z[], tvec[], rpar[];
     double *inptr[], *outptr[], *t;

{
    int k = 0;
    double *y = NULL;
    double *u = NULL;
    int nev = 0;
    int ic = (int)z[0];

    if ((*flag) < 3)
    {
        if ((*nevprt) > 0)
        {
            ic = -1;
            nev = *nevprt;
            while (nev >= 1)
            {
                ic = ic + 1;
                nev = nev / 2;
            }
        }
        if ((*flag) == 2)
        {
            z[0] = ic;
            return;
        }
        if (*nin > 1)
        {
            y = (double *)outptr[0];
            u = (double *)inptr[ic];
            for (k = 0; k < outsz[0]; k++)
                *(y++) = *(u++);
        }
        else
        {
            y = (double *)outptr[ic];
            u = (double *)inptr[0];
            for (k = 0; k < outsz[0]; k++)
                *(y++) = *(u++);
        }
    }
}

/*--------------------------------------------------------------------------*/
