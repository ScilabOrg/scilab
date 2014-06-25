/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include <wchar.h>
#include "BOOL.h"

/**
 * change language
 * @param lang
 */
BOOL setlanguage(const wchar_t *lang);


/**
 * returns current language
 * @return
 */
const wchar_t *getlanguage(void);

/**
 * returns current alias
 * example : en_US --> en
 * @return
 */
const wchar_t *getlanguagealias(void);

/**
 * return internal code associated with a language
 * @return
 */
int getcurrentlanguagecode(void);

/**
 * returns language of code
 * @param code
 * @return
 */
const wchar_t *getlanguagefromcode(int code);

/**
 * get code of a language
 * @param language
 * @return
 */
int getcodefromlanguage(const wchar_t *language);

/**
* try to convert strlanguage (alias) to a good language string
* @param language
* @return
*/
const wchar_t *convertlanguagealias(const wchar_t *strlanguage);

/**
* test if lang is defined language
* @param lang
* @return TRUE or FALSE
*/
BOOL LanguageIsOK(const wchar_t *lang);

/**
 * Check if the current language is the same we are trying to switch to
 *
 * @param language The language we want to switch to
 * @return If we need to change the language or not
 */
BOOL needtochangelanguage(const wchar_t *language);


/**
 * Export the locale to the system
 *
 * @param the locale we want to export
 * @return if is worked
 */
BOOL exportLocaleToSystem(const wchar_t *locale);

/**
 * Set the default language and save it in the registry (Windows)
 * @param lang the new language
 */
void setdefaultlanguage(const char * lang);

/**
 * Get the default language from the registry (Windows)
 * @return the default language
 */
const char * getdefaultlanguage();


#endif /* __SETGETLANGUAGE_H__ */
/*--------------------------------------------------------------------------*/
