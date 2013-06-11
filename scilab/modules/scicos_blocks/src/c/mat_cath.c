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
#include <string.h>
#include <stdio.h>
#include "scicos_block4.h"
#include "matz_cath.h"
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void mat_cath(scicos_block *block, int flag)
{
    int mu = 0, nu = 0, nin = 0, so = 0, pointerposition = 0, ot = 0, i = 0;
    ot = GetOutType(block, 1);
    mu = GetInPortRows(block, 1);
    if (ot == SCSCOMPLEX_N)
    {
        matz_cath(block, flag);
    }
    else
    {
        void *u, *y;
        y = GetOutPortPtrs(block, 1);
        nin = GetNin(block);
        if ((flag == 1) || (flag == 6))
        {
            pointerposition = 0;
            for (i = 0; i < nin; i++)
            {
                u = GetInPortPtrs(block, i + 1);
                nu = GetInPortCols(block, i + 1);
                so = GetSizeOfIn(block, i + 1);
                memcpy((char*)y + pointerposition, u, mu * nu * so);
                pointerposition = pointerposition + mu * nu * so;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
