/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "JavasciOutput.hxx"

extern "C"
{
#include "printToSystemOut.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_javasci;

void printToSystemOut(char * s)
{
    JavasciOutput::printToSystemOut(getScilabJavaVM(), s);
}
