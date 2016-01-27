/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef AST_VISITOR_COMMON_HXX
#define AST_VISITOR_COMMON_HXX


#define ENABLE_EXTERNAL_TYPER

#include "internal.hxx"
#include "struct.hxx"
#include "list.hxx"
#include "exp.hxx"
#include "symbol.hxx"
#include <list>
#include "expHistory.hxx"
#include "dynlib_ast.h"
#define bsiz	4096

//void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList);

types::InternalType* AddElementToVariable(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols);

types::InternalType* AddElementToVariableFromCol(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols, int *_piCols);

types::InternalType* AddElementToVariableFromRow(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols, int *_piRows);

const std::wstring* getStructNameFromExp(const ast::Exp* _pExp);

bool getFieldsFromExp(ast::Exp* _pExp, std::list<ExpHistory*>& fields);

types::InternalType* evaluateFields(const ast::Exp* _pExp, std::list<ExpHistory*>& fields, types::InternalType* pIT);

types::InternalType* callOverload(const ast::Exp& e,
                                  const std::wstring& _strType,
                                  types::typed_list* _pArgs,
                                  types::InternalType* _source,
                                  types::InternalType* _dest);

types::InternalType* callOverload(const std::wstring& strType, types::InternalType* _paramL, types::InternalType* _paramR);

types::InternalType* insertionCall(const ast::Exp& e, types::typed_list* _pArgs, types::InternalType* _pVar, types::InternalType* _pInsert);

AST_IMPEXP void callOnPrompt(void);
AST_IMPEXP ast::Exp* callTyper(ast::Exp* _tree, std::wstring _msg = std::wstring(L""));

void printLine(const std::string& _stPrompt, const std::string& _stLine, bool _bLF);
std::string printExp(std::ifstream& _File, ast::Exp* _pExp, const std::string& _stPrompt, int* _piLine /* in/out */, int* _piCol /* in/out */, std::string& _stPreviousBuffer);


#endif //!AST_VISITOR_COMMON_HXX
