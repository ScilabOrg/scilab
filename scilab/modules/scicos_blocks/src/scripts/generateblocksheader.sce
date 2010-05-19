//  Scicos
//
//  Copyright (C) DIGITEO - 2009 - Allan CORNET
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ./license.txt
//

cd SCI/modules/scicos_blocks/src/c;

BLOCK_C_SOURCES = gsort(ls('*.c'));

NON_BLOCK_C_SOURCES = ['scoGetProperty.c',
'scoSetProperty.c',
'scoMisc.c',
'scoMemoryScope.c',
'scoWindowScope.c',
'affich.c',
'DllmainScicos_blocks.c',
'readf4.c',
'writef4.c',
'scicos_evalhermite.c',
'scicos_indexfinder.c'];

BLOCK_C_SOURCES(grep(BLOCK_C_SOURCES,NON_BLOCK_C_SOURCES)) = [];
BLOCKS_C = strsubst(BLOCK_C_SOURCES,'.c','');

cd SCI/modules/scicos_blocks/src/fortran;

BLOCK_FORTRAN_SOURCES = gsort(ls('*.f'));
BLOCKS_FORTRAN = strsubst(BLOCK_FORTRAN_SOURCES,'.f','');

BLOCKS_H = [
'';
'/*  Scicos';
'*';
'*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>';
'*  Copyright (C) DIGITEO - 2009';
'*';
'* This program is free software; you can redistribute it and/or modify';
'* it under the terms of the GNU General Public License as published by';
'* the Free Software Foundation; either version 2 of the License, or';
'* (at your option) any later version.';
'*';
'* This program is distributed in the hope that it will be useful,';
'* but WITHOUT ANY WARRANTY; without even the implied warranty of';
'* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the';
'* GNU General Public License for more details.';
'*';
'* You should have received a copy of the GNU General Public License';
'* along with this program; if not, write to the Free Software';
'* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.';
'*';
'* See the file ./license.txt';
'*/';
'#ifndef __SCICOS_BLOCKS_H__';
'#define __SCICOS_BLOCKS_H__';
'';
'#include ""machine.h""';
'#include ""dynlib_scicos_blocks.h""';
'#include ""scicos.h""';
'';
'/******* Please do not edit *************/';
''];

BLOCKS_H = [BLOCKS_H;
'SCICOS_BLOCKS_IMPEXP void C2F(' + BLOCKS_FORTRAN + ')(ARGS_scicos);' ;
'SCICOS_BLOCKS_IMPEXP void ' + BLOCKS_C + '(ARGS_scicos);' ;
''];

SIZE_TAB_BLOCK_LIST = size(BLOCKS_C,'*') + size(BLOCKS_FORTRAN,'*') + 1;

BLOCKS_H = [BLOCKS_H;
'#define SIZE_TAB_BLOCK_LIST ' + string(SIZE_TAB_BLOCK_LIST);
'int ntabsim = SIZE_TAB_BLOCK_LIST ;'
'';
'OpTab tabsim[SIZE_TAB_BLOCK_LIST] ={'];

BLOCKS_H = [BLOCKS_H;
'{""' + BLOCKS_FORTRAN + '"",(ScicosF) C2F(' + BLOCKS_FORTRAN + ')},' ;
'{""' + BLOCKS_C + '"",(ScicosF) ' + BLOCKS_C + '},' ;
'{(char *) 0, (ScicosF) 0}};' ;
'#endif' ;
'/****************************************/' ;
''];

mputl(BLOCKS_H, SCI + '/modules/scicos_blocks/includes/blocks.h.vc');
mputl(BLOCKS_H, SCI + '/modules/scicos_blocks/includes/blocks.h');
