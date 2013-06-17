/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 * Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __INITSCILAB_H__
#define __INITSCILAB_H__

int StartScilabEngine(
    bool _bNoStart = false,
    bool _bJvm = true,
    bool _bConsole = true,
    char* _pstLang = NULL,
    char* _pstExec = NULL,
    char* _pstFile = NULL);

int RunScilabEngine(char* _pstFile = NULL);

void StopScilabEngine(bool _bNoStart = false, bool _bConsole = true);

#endif /* !__INITSCILAB_H__ */
