/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __TERMINATETCLTK_H__
#define __TERMINATETCLTK_H__

#include "dynlib_tclsci.h"
#include "BOOL.h"

/**
 * Stops the TCL engine in Scilab
 *
 * @return The result of the stop
 */
TCLSCI_IMPEXP BOOL TerminateTclTk(void);

#endif /* _TERMINATETCLTK_H__ */
/*--------------------------------------------------------------------------*/
