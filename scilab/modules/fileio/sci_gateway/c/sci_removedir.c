/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "removedir.h"
#include "isdir.h"
#include "Scierror.h"
#include "cluni0.h"
#include "localization.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_removedir)(char *fname,unsigned long l)
{
	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		BOOL bOK = FALSE;
		int m1 = 0, n1 = 0, l1 = 0;
		char expandedpath[PATH_MAX+1];
		int out_n = 0;
		char *VarName = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName = cstk(l1);

		C2F(cluni0)(VarName,expandedpath, &out_n,(int)strlen(VarName),PATH_MAX);

		if ( isdir(expandedpath) )
		{
			bOK = removedir(expandedpath);
		}

		m1 = 1; n1 = 1;
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1 ,&l1);
		*istk(l1) = bOK;

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"), fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
