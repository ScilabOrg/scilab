/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "machine.h"

#ifndef _MSC_VER
 #ifdef HAVE_LIBINTL_H
  #include <libintl.h>
  #ifdef HAVE_LOCALE_H
   #include <locale.h>
  #else
   #error "Cannot find locale.h despite that libintl.h is available"
  #endif
 #endif
#else
 #include <libintl.h>
 #include <locale.h>
#endif

#include "MALLOC.h"
#include "InitializeLocalization.h"
#include "localization.h"
#include "setgetSCIpath.h"
#include "inisci-c.h"
#include "scilabDefaults.h"
#include "setgetlanguage.h"
#include "isdir.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "loadLanguagePref.h"
/*--------------------------------------------------------------------------*/ 

BOOL InitializeLocalization(void)
{
#ifdef HAVE_LIBINTL_H

	char *SCIpath = getSCIpath();
	char *pathLocales = NULL, *previousPathLocales = NULL;
	char *ret = NULL;

	/* set directory containing message catalogs */
	pathLocales=(char *)MALLOC(sizeof(char)*(strlen(SCIpath)+strlen(PATHLOCALIZATIONFILE)+1));

	strcpy(pathLocales, SCIpath);
	strcat(pathLocales, PATHLOCALIZATIONFILE);

	if (bindtextdomain(NAMELOCALIZATIONDOMAIN,pathLocales)==NULL || !isdir(pathLocales))
	{ 
		/* source tree and classic build */
		previousPathLocales = strdup(pathLocales);
		FREE(pathLocales);

		pathLocales=(char *)MALLOC(sizeof(char)*(strlen(SCIpath)+strlen("/..")+strlen(PATHLOCALIZATIONFILE)+1));
		strcpy(pathLocales, SCIpath);
		strcat(pathLocales, "/..");
		strcat(pathLocales, PATHLOCALIZATIONFILE);
		if (bindtextdomain(NAMELOCALIZATIONDOMAIN,pathLocales)==NULL || !isdir(pathLocales))
		{ 
			/* when it is installed on the system for example /usr/share/locale/ */
			fprintf(stderr, "Warning: Localization issue: Error while binding the domain from %s or %s: Switch to the default language (English).\n", pathLocales, previousPathLocales);
			FREE(previousPathLocales);
			FREE(pathLocales);
			return FALSE;
		}
		FREE(previousPathLocales);
		FREE(pathLocales);
		
	}

	/* set domain for future gettext() calls */
	ret = textdomain(NAMELOCALIZATIONDOMAIN);
	if (ret == NULL)
	{
		fprintf(stderr, "Localization: Error while declaring the text domain %s\n", NAMELOCALIZATIONDOMAIN);
		return FALSE;
	}

	/* We look if a file .language exists in SCIHOME */
	/* If not exists the "" means that we will try to use the language of the system.*/
	{
		char *loadLanguage = loadLanguagePref();
		setlanguage(loadLanguage);
		if (loadLanguage) {FREE(loadLanguage); loadLanguage = NULL;}
	}
	
	return TRUE;
#else
	fprintf(stderr, "Localization: setlocale didn't exist on the computer used to compile Scilab ! This is abnormal ! No localization will be working for this distribution of Scilab.\n");
	return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/ 

