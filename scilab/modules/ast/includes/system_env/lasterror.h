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

#ifndef __SCI_LASTERROR_H__
#define __SCI_LASTERROR_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_ast.h"

AST_IMPEXP int getLastErrorNumber(void);
AST_IMPEXP wchar_t* getLastErrorMessage(void);
AST_IMPEXP int getLastErrorLine(void);
AST_IMPEXP wchar_t* getLastErrorFunction(void);

AST_IMPEXP void setLastErrorNumber(int _iError);
AST_IMPEXP void setLastErrorMessage(const wchar_t* _pwstError);
AST_IMPEXP void setLastErrorLine(int _iLine);
AST_IMPEXP void setLastErrorFunction(const wchar_t* _pwstFunction);

AST_IMPEXP void setLastError(int _iError, const wchar_t* _pwstError, int _iLine, const wchar_t* _pwstFunction);

AST_IMPEXP void SetError();
AST_IMPEXP int isError();
AST_IMPEXP void resetError();

#endif // ! __SCI_LASTERROR_H__
