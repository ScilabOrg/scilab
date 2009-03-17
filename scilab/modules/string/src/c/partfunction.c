
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include "MALLOC.h"
#include "partfunction.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#define BLANK_CHAR ' '
/*--------------------------------------------------------------------------*/
char **partfunction(char** stringInput,int m,int n,int *vectInput,int row)
{
	wchar_t **wcparts = NULL;
	char **parts = NULL;
	int mn = m * n;

	wcparts = (wchar_t**)MALLOC(sizeof(wchar_t*)*(mn));

	if (wcparts)
	{
		int i = 0;

		parts = (char**)MALLOC(sizeof(char*)*(mn));

		for (i = 0;i < mn; i++)
		{
			int j = 0;
			wchar_t *wcInput = to_wide_string(stringInput[i]);
			int lengthstringInput = (int)wcslen(wcInput);

			wcparts[i] = (wchar_t*)MALLOC(sizeof(wchar_t)*((row)+1));
			if (wcparts[i] == NULL)
			{
				for (i = 0;i < mn; i++)
				{
					if (wcparts[i])
					{
						FREE(wcparts[i]);
						wcparts[i] = NULL;
					}
				}
				FREE(wcparts);
				wcparts = NULL;
				return NULL;
			}

			for (j = 0;j < row; j++)
			{
				if ( vectInput[j] > lengthstringInput )
				{
					wcparts[i][j] = L' ';
				}
				else
				{
					wcparts[i][j] = wcInput[vectInput[j]-1];
				}
			}
			wcparts[i][j] ='\0';

			parts[i] = wide_string_to_UTF8(wcparts[i]);
		}
	}
	return parts;
}
/*--------------------------------------------------------------------------*/
