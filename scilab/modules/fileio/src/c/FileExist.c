/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <stdio.h>
#endif
#include "FileExist.h"
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
BOOL FileExist(char *filename)
{
#ifdef _MSC_VER
	{
		WIN32_FIND_DATAW FindFileData;
		wchar_t *wcFilename = to_wide_string(filename);
		if (wcFilename)
		{
			HANDLE handle = FindFirstFileW (wcFilename, &FindFileData);
			FREE(wcFilename);
			if (handle != INVALID_HANDLE_VALUE)
			{
				FindClose (handle);
				return TRUE;
			}
			else return FALSE;
		}
		else return FALSE;
	}
#else
	FILE* tmpFile=fopen(filename,"r");
	if(tmpFile) 
		{ 
			fclose(tmpFile); 
			return TRUE;
		} 
	else 
		{ 
			return FALSE;
		} 
#endif

}
/*--------------------------------------------------------------------------*/ 
