
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stringsstrrev.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*----------------------------------------------------------------------------*/
char **strings_strrev(char **Input_strings,int Dim_Input_strings)
{
	char **Output_strings = NULL;
	if (Input_strings)
	{
		Output_strings = (char **)MALLOC(sizeof(char*)*Dim_Input_strings);
		if (Output_strings)
		{
			int i = 0;	
			for (i = 0;i < Dim_Input_strings;i++)
			{
				Output_strings[i] = scistrrev(Input_strings[i]);
				if (!Output_strings[i])
				{
					freeArrayOfString(Output_strings,i);
					return Output_strings;
				}
			}
		}
	}
	return Output_strings;
}
/*----------------------------------------------------------------------------*/
char* scistrrev(char* str)
{
	char *reversestr = NULL;

	if (str == NULL) return NULL;

	reversestr = (char*)MALLOC(sizeof(char)*((int)strlen(str)+1));

	if (reversestr)
	{
		char *currentchar = str;
		int i = 0;
		while (*currentchar != 0)
		{
			int  charBytes = 0;
			char *UTFChar = readNextUTFChar(currentchar,&charBytes);
			currentchar += charBytes;

			if (i == 0)
			{
				sprintf(reversestr,"%s",UTFChar);
				i = 1;
			}
			else
			{
				char *prevpart = strdup(reversestr);
				if (prevpart)
				{
					sprintf(reversestr,"%s%s",UTFChar,prevpart);
					FREE(prevpart);
				}
			}
		}
	}
	return reversestr;
}
/*----------------------------------------------------------------------------*/
