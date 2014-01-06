/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "NoMoreScilabMemoryException.hxx"
/*--------------------------------------------------------------------------*/
namespace org_scilab_modules_external_objects_java
{

NoMoreScilabMemoryException::NoMoreScilabMemoryException(void) throw() : exception() { }

NoMoreScilabMemoryException::~NoMoreScilabMemoryException(void) throw() { }

/**
 * @return a description of the exception
 */
const char * NoMoreScilabMemoryException::what(void) const throw()
{
    return NOMOREMEMORY;
}
}
/*--------------------------------------------------------------------------*/
