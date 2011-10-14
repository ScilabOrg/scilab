/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "inspector.hxx"
#include "sciprint.h"

using namespace types;

Function::ReturnValue sci_inspectorGetItemCount(typed_list &in, int _iRetCount, typed_list &out)
{
    sciprintW(L"\n%d\tItems\n", Inspector::getItemCount());
    sciprintW(L"%d\tUnreferenced Items\n", Inspector::getUnreferencedItemCount());
    return Function::OK;
}
