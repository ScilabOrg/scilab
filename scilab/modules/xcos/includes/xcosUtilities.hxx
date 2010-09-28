/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XCOS_UTILITIES_HXX__
#define __XCOS_UTILITIES_HXX__

/*
** Read a Single String using API Scilab
*/
int readSingleString(void *pvApiCtx, int rhsPosition, char** out, const char* fname);

/*
** Read a String Vector using API Scilab
*/
int readVectorString(void *pvApiCtx, int rhsPosition, char*** out, int* vectorLength, char* fname);

#endif /* !__XCOS_UTILITIES_HXX__ */
