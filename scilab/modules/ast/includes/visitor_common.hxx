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

#define bsiz	4096

EXTERN_AST bool bConditionState(types::InternalType *_pITResult);

//EXTERN_AST void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList);

EXTERN_AST types::InternalType* AddElementToVariable(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols);

EXTERN_AST types::InternalType* AddElementToVariableFromCol(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols, int *_piCols);

EXTERN_AST types::InternalType* AddElementToVariableFromRow(
    types::InternalType* _poDest,
    types::InternalType* _poSource,
    int _iRows, int _iCols, int *_piRows);

EXTERN_AST const std::wstring* getStructNameFromExp(const ast::Exp* _pExp);

EXTERN_AST bool getFieldsFromExp(ast::Exp* _pExp, std::list<ExpHistory*>& fields);

EXTERN_AST types::InternalType* evaluateFields(const ast::Exp* _pExp, std::list<ExpHistory*>& fields, types::InternalType* pIT);

EXTERN_AST types::InternalType* callOverload(const ast::Exp& e,
        std::wstring _strType,
        types::typed_list* _pArgs,
        types::InternalType* _source,
        types::InternalType* _dest);

EXTERN_AST types::InternalType* callOverload(std::wstring strType, types::InternalType* _paramL, types::InternalType* _paramR);

EXTERN_AST void callOnPrompt(void);

EXTERN_AST types::List* getPropertyTree(ast::Exp* e, types::List* pList);

EXTERN_AST types::InternalType* insertionCall(const ast::Exp& e, types::typed_list* _pArgs, types::InternalType* _pVar, types::InternalType* _pInsert);
EXTERN_AST ast::Exp* callTyper(ast::Exp* _tree, std::wstring _msg = std::wstring(L""));

#endif //!AST_VISITOR_COMMON_HXX
