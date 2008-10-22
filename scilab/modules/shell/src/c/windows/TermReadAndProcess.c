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

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "HistoryManager.h"
#include "TermReadAndProcess.h"
#include "MALLOC.h"
#include "prompt.h"
#include "readline_nw.h"
/*--------------------------------------------------------------------------*/
#define strdup _strdup
/*--------------------------------------------------------------------------*/
char * TermReadAndProcess(void)
{
	static char save_prompt[10];
	char *returnedline = NULL;
	char *UTFLine;

    if (GetTemporaryPrompt()!=NULL) /* Input function is used */
    {
		returnedline = readline_nw(GetTemporaryPrompt());
        ClearTemporaryPrompt();
	}
	else
	{
		GetCurrentPrompt(save_prompt);
		returnedline = readline_nw (save_prompt);
	}

	strcpy(save_prompt,"");
	return returnedline;
}
/*--------------------------------------------------------------------------*/
