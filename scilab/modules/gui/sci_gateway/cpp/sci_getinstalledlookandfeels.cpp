/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "LookAndFeelManager.hxx"

extern "C"
{
#include <stdlib.h>
#include "gw_gui.h"
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_getinstalledlookandfeels(char *fname,unsigned long fname_len)
{

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,1);

	org_scilab_modules_gui_utils::LookAndFeelManager *lnf = new org_scilab_modules_gui_utils::LookAndFeelManager(getScilabJavaVM());
	if (lnf)
	{
		char **lookandfeels = NULL;
		int nbElems = 0;
		int nbCol = 0;

		lookandfeels = lnf->getInstalledLookAndFeels();
		nbElems = lnf->numbersOfInstalledLookAndFeels();
		delete lnf;

		nbCol = 1;
		CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &nbElems, &nbCol,lookandfeels );

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

		if (lookandfeels)
		{
			int i = 0;
			for (i=0;i<nbElems;i++)
			{
				if (lookandfeels[i])
				{
					free(lookandfeels[i]); /* free because giws uses malloc */
					lookandfeels[i] = NULL;
				}
			}
			free(lookandfeels);
			lookandfeels=NULL;
		}
	}
	else
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
}
/* END OF extern "C" */
/*--------------------------------------------------------------------------*/


