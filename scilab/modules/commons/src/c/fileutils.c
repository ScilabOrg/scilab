/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sys/types.h>
#include <dirent.h>
#include "MALLOC.h"

int isEmptyDirectory(char * dirName)
{
    DIR *dir = NULL;
    struct dirent64 *ptr;
    struct dirent64 *result;
    int ret = 1;

    dir = opendir(dirName);
    if (dir == NULL)
    {
	return 0;
    }

    ptr = MALLOC(sizeof(struct dirent64) + (PATH_MAX + 1));
    if (ptr == NULL)
    {
        closedir(dir);
        return 0;
    }

    while ((readdir64_r(dir, ptr, &result) == 0)  && (result != NULL))
    {
        if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, ".."))
        {
	    continue;
	}
	
	ret = 0;
	break;
    }

    free(ptr);
    closedir(dir);

    return ret;
}
	
