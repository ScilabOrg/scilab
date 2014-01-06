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
#ifndef __NOMORESCILABMEMORYEXCEPTION__
#define __NOMORESCILABMEMORYEXCEPTION__
#include <exception>
#include "noMoreMemory.h"

namespace org_scilab_modules_external_objects_java
{

/**
 * Parent class for exceptions which may occure in JNI code.
 */
class NoMoreScilabMemoryException : public std::exception
{

public:

    NoMoreScilabMemoryException(void) throw() ;

    ~NoMoreScilabMemoryException(void) throw();

    /**
     * @return a description of the exception
     */
    const char * what(void) const throw();
};
}
#endif
/*--------------------------------------------------------------------------*/
