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

#include "listoperation.hxx"
#include "localization.h"

namespace types
{

	/*--------------*/
	/*	Destructor  */
	/*--------------*/
	ListOperation::~ListOperation()
	{
	}

	/*--------------*/
	/*	whoIAm		  */
	/*--------------*/
	void ListOperation::whoAmI() const
	{
		std::cout << "types::ListOperation";
	}

	ListOperation* ListOperation::clone(void) const
	{
		return new ListOperation();
	}

	ListOperation* ListOperation::getAsListOperation(void)
	{
		return this;
	}

	ListOperation const* ListOperation::getAsListOperation(void)const
	{
		return this;
	}

	InternalType::RealType ListOperation::getType(void)const
	{
		return RealListOperation;
	}

	std::string ListOperation::toString(int _iPrecision, int _iLineLen)const
	{
	  std::ostringstream ostr;

	  //FIXME : Implement me.
	  ostr << "FIXME : Implement ListOperation::toString" << std::endl;

	  return ostr.str();
	}
}
