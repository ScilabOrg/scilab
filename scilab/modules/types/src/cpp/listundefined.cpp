/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include <cstdio>

#include "listundefined.hxx"
#include "localization.h"

namespace types
{
	ListUndefined::~ListUndefined()
	{
	}

	void ListUndefined::whoAmI()
	{
		std::cout << "types::ListUndefined";
	}

	InternalType* ListUndefined::clone(void)
	{
		return new ListUndefined();
	}

	InternalType::RealType ListUndefined::getType(void)
	{
		return RealListUndefinedOperation;
	}

    std::wstring ListUndefined::toString()
	{
        std::wostringstream ostr;

	  //FIXME : Implement me.
	  ostr << L"    Undefined" << std::endl;

	  return ostr.str();
	}
}
