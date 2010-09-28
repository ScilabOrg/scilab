/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <math.h>
#include <string.h>
#include <wchar.h>
#include "version.h"
#include "sciprint.h"
#include "banier.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static char *line = "        ___________________________________________        ";
/*--------------------------------------------------------------------------*/
void banner(void)
{
        int i;

        sciprint("%s\n",line);

        centerPrint(SCI_VERSION_STRING);
        sciprint("\n\n");

        centerPrint(_("Consortium Scilab (DIGITEO)\n"));
        centerPrint(_("Copyright (c) 1989-2010 (INRIA)\n"));
        centerPrint(_("Copyright (c) 1989-2007 (ENPC)\n"));

        sciprint("%s\n",line);

        #if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
                sciprint("\n\n");
                sciprint(_("Warning: the operational team of the Scilab Consortium\ndoesn't provide and doesn't support this version of Scilab built with\n"));
                sciprint("   ");
        #endif
}
/*--------------------------------------------------------------------------*/
void centerLine(char *str)
{
        int i, start;
        size_t size = strlen(str) + 1;
        wchar_t *wstr = MALLOC(sizeof(wchar_t) * size);

        mbstowcs(wstr, str, size);
        start = (int)(floor((double)(strlen(line)/2)) - floor((double)(((int)wcslen(wstr) - 1)/2)));
        FREE(wstr);

        char *whites = MALLOC(sizeof(char) * (start + 1));

        /* To center the string */
        for( i = 0 ; i < start ; i++ )
        {
          whites[i] = ' ';
        }
        whites[start] = '\0';
        sciprint(whites);
        FREE(whites);
}
/*--------------------------------------------------------------------------*/
void centerPrint(char *str)
{
        centerLine(str);
        sciprint(str);
}
/*--------------------------------------------------------------------------*/
int C2F(banier)(int *flag)
{
        if (*flag != 999)
        {
                banner();
        }
        return 0;
}
/*--------------------------------------------------------------------------*/
