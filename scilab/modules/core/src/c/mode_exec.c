/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include "mode_exec.h"
#include "stack-def.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
BOOL setExecMode(int _mode)
{
	int *Lct = C2F(iop).lct - 1;
	if ( _mode != Lct[4] )
	{
		Lct[4] = _mode;
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
int getExecMode(void)
{
	int *Lct = C2F(iop).lct - 1;
	return Lct[4];
}
/*--------------------------------------------------------------------------*/ 
