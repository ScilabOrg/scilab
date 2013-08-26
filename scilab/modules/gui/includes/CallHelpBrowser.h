#ifndef __CALLHELPBROWSER_H__
#define __CALLHELPBROWSER_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"

/**
 * Launch a Scilab Help Browser
 *
 * @param helps help chapters and directories
 * @param helpsSize number of entries in helps
 * @param language Scilab current locale
 */
int launchHelpBrowser(char **helps, int helpsSize, char *language);

/**
 * Search a keyword in Scilab Help Browser
 *
 * @param helps help chapters and directories
 * @param helpsSize number of entries in helps
 * @param keyword the keyword to search
 * @param language Scilab current locale
 * @param fullText true to execute a full-text search
 */
int searchKeyword(char **helps, int helpsSize, char *keyword, char *language, BOOL fullText);

#endif /* !__CALLHELPBROWSER_H__ */

