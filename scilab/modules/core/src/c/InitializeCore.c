/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include "InitializeCore.h"
#include "SetScilabEnvironment.h"
#include "getmodules.h"
#include "hashtable_core.h"
#include "sciprint.h"
#include "LoadFunctionsTab.h"
#include "prompt.h"
#include "sciprint.h"
#include "filesmanagement.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL InitializeCore(void)
{
	int pause = 0;

	InitializeScilabFilesList();

	if (getmodules() == NULL)
    {
        return FALSE;
    }

	if ( create_hashtable_scilab_functions() == FALSE ) 
	{
		sciprint(_("Fatal Error: Can't create table for scilab functions.\n"));
		return FALSE;
	}

	LoadFunctionsTab();

	/* Set prompt level */
	C2F(setprlev)(&pause);

	return TRUE;
}
/*--------------------------------------------------------------------------*/ 

