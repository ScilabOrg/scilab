/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <CoreFoundation/CoreFoundation.h>
#include "getLocaleInfo_Apple.h"

char *getLocaleUserInfo(void)
{
    char *cUserLanguage = NULL;
    CFLocaleRef userLocaleRef = CFLocaleCopyCurrent();
    CFStringRef userLanguage = CFLocaleGetIdentifier(userLocaleRef);

    if (getenv( "LANG" ))
    {
        /* Mac OS X does not respect the LANG variable. We do it ourself. */
        return getenv("LANG");
    }
    else
    {
        cUserLanguage = (char *) malloc(((int) CFStringGetLength(userLanguage) + 1) * sizeof(char));
        CFStringGetCString(userLanguage, cUserLanguage, ((int) CFStringGetLength(userLanguage)) + 1, kCFStringEncodingUTF8);
    }
    return cUserLanguage;
}
