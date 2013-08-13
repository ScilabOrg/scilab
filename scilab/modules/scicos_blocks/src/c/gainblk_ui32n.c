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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <math.h>
#include "scicos_block4.h"
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void gainblk_ui32n(scicos_block *block, int flag)
{
    if ((flag == 1) | (flag == 6))
    {
        int i = 0, j = 0, l = 0, ji = 0, jl = 0, il = 0;
        SCSUINT32_COP *u = NULL, *y = NULL;
        int mu = 0, ny = 0, my = 0, mo = 0, no = 0;
        SCSUINT32_COP *opar = NULL;
        double k = 0., D = 0., C = 0., t = 0.;

        mo = GetOparSize(block, 1, 1);
        no = GetOparSize(block, 1, 2);
        mu = GetInPortRows(block, 1);
        my = GetOutPortRows(block, 1);
        ny = GetOutPortCols(block, 1);
        u = Getuint32InPortPtrs(block, 1);
        y = Getuint32OutPortPtrs(block, 1);
        opar = Getuint32OparPtrs(block, 1);

        k = pow(2, 32);
        if (mo*no == 1)
        {
            for (i = 0; i < ny * mu; ++i)
            {
                D = (double)(opar[0]) * (double)(u[i]);
                t = D - (double)((int)(D / (k))) * ((k));
                if ((t >= k / 2) | (-(t) >= k / 2))
                {
                    if (t >= 0)
                    {
                        (t) = (-((k / 2)) + fabs(t - (double)((int)((t) / ((k / 2)))) * ((k / 2))));
                    }
                    else
                    {
                        (t) = -(-((k / 2)) + fabs(t - (double)((int)((t) / ((k / 2)))) * ((k / 2))));
                    }
                }
                y[i] = (SCSUINT32_COP)t;
            }
        }
        else
        {
            for (l = 0; l < ny; l++)
            {
                for (j = 0; j < my; j++)
                {
                    D = 0;
                    jl = j + l * my;
                    for (i = 0; i < mu; i++)
                    {
                        ji = j + i * my;

                        il = i + l * mu;
                        C = (double)(opar[ji]) * (double)(u[il]);
                        D = D + C;
                    }
                    t = D - (double)((int)(D / (k))) * ((k));
                    if ((t >= k / 2) | (-(t) >= k / 2))
                    {
                        if (t >= 0)
                        {
                            (t) = (-((k / 2)) + fabs(t - (double)((int)((t) / ((k / 2)))) * ((k / 2))));
                        }
                        else
                        {
                            (t) = -(-((k / 2)) + fabs(t - (double)((int)((t) / ((k / 2)))) * ((k / 2))));
                        }
                    }
                    y[jl] = (SCSUINT32_COP)t;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
