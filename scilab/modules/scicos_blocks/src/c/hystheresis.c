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
#include "scicos_block.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void  hystheresis(scicos_block *block, int flag)
{
    if (flag == 1)
    {
        if (get_phase_simulation() == 1 || block->ng == 0)
        {
            if (*block->inptr[0] >= block->rpar[0])
            {
                block->outptr[0][0] = block->rpar[2];
            }
            else if (*block->inptr[0] <= block->rpar[1])
            {
                block->outptr[0][0] = block->rpar[3];
            }
            else if ((block->outptr[0][0] != block->rpar[3]) && (block->outptr[0][0] != block->rpar[2]))
            {
                block->outptr[0][0] = block->rpar[3];
                /* Handling sitauations where all zero-crossings are
                   suppressed in discrete models. In this case, initial state
                   is initialised to OFF*/
            }
        }
        else
        {
            /* compatibility with simulink: when input value is located
             between two margines the OFF state is selected. Initial
             Mode is OFF (mode==0)*/
            if (block->mode[0] == 2)
            {
                block->outptr[0][0] = block->rpar[2];
            }
            else
            {
                block->outptr[0][0] = block->rpar[3];
            }
        }
    }
    else if (flag == 9)
    {
        block->g[0] = *block->inptr[0] - (block->rpar[0]);
        block->g[1] = *block->inptr[0] - (block->rpar[1]);
        if (get_phase_simulation() == 1)
        {
            if (block->g[0] >= 0)
            {
                block->mode[0] = 2;
            }
            else if (block->g[1] <= 0)
            {
                block->mode[0] = 1;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
