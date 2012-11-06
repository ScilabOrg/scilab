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

#include "internal.hxx"
#include "struct.hxx"
#include "list.hxx"
#include "exp.hxx"

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

EXTERN_AST bool getStructFromExp(const ast::Exp* _pExp, types::InternalType** _pMain, types::InternalType** _pCurrent, types::typed_list** _pArgs, types::InternalType* _pIT);

EXTERN_AST void callOnPrompt(void);

EXTERN_AST types::List* getPropertyTree(ast::Exp* e, types::List* pList);


#endif //!AST_VISITOR_COMMON_HXX
