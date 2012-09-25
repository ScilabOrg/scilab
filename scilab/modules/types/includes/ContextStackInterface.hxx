/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "internal.hxx"

namespace types
{

int allocStack(int _iSize);
int initStack(int _iSize);
int freeStack();

//convert context variable to old stack variable
int convertVarToStack(InternalType* _pIT, int _iPos);

//convert old stack to context variable
InternalType* convertStackToVar(int _iPos);


}
