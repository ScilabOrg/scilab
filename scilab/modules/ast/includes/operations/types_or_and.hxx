/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_BITWISEOP_H__
#define __TYPES_BITWISEOP_H__

#include "generic_operations.hxx"
#include "types.hxx"
#include "bool.hxx"
//#include "sparse.hxx"

// &&
int IntAndInt(types::InternalType* _pL, types::Bool** _pOut);
int BoolAndBool(types::Bool* _pL, types::Bool** _pOut);
int SparseBoolAndSparseBool(types::InternalType* _pL, types::Bool** _pOut);

// &
int IntLogicalAndInt(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut);
int BoolLogicalAndBool(types::Bool* _pL, types::Bool*  _pR, types::Bool** _pOut);
int SparseBoolLogicalAndSparseBool(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut);

// ||
int IntOrInt(types::InternalType* _pL, types::Bool** _pOut);
int BoolOrBool(types::Bool* _pI1, types::Bool** _pOut);
int SparseBoolOrSparseBool(types::InternalType* _pL, types::Bool** _pOut);

// |
int IntLogicalOrInt(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut);
int BoolLogicalOrBool(types::Bool* _pL, types::Bool*  _pR, types::Bool** _pOut);
int SparseBoolLogicalOrSparseBool(types::InternalType* _pL, types::InternalType*  _pR, types::InternalType** _pOut);

#endif /* __TYPES_BITWISEOP_H__ */
