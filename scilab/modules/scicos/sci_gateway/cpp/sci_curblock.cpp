/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_scicos.hxx"

#include "types.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "scicos-def.h"
}

extern CURBLK_struct C2F(curblk);

types::Function::ReturnValue sci_curblock(types::typed_list /*&in*/, int /*_iRetCount*/, types::typed_list &out)
{
    types::Double* kfun = new types::Double(C2F(curblk).kfun);
    out.push_back(kfun);
    return types::Function::OK;
}
