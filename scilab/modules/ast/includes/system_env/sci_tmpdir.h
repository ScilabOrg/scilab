/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __SCI_TMPDIR_H__
#define __SCI_TMPDIR_H__

#include <wchar.h>

#include "dynlib_ast.h"

AST_IMPEXP wchar_t* getTMPDIRW(void);
AST_IMPEXP char* getTMPDIR(void);

AST_IMPEXP void setTMPDIRW(const wchar_t* _sci_tmpdir);
AST_IMPEXP void setTMPDIR(const char* _sci_tmpdir);

AST_IMPEXP wchar_t* computeTMPDIRW();
AST_IMPEXP char* computeTMPDIR();

AST_IMPEXP wchar_t* getenvTMPDIRW(void);
AST_IMPEXP char* getenvTMPDIR(void);

AST_IMPEXP void putenvTMPDIRW(const wchar_t* _sci_tmpdir);
AST_IMPEXP void putenvTMPDIR(const char* _sci_tmpdir);

AST_IMPEXP void defineTMPDIR();
AST_IMPEXP void clearTMPDIR();

#endif // ! __SCI_TMPDIR_H__