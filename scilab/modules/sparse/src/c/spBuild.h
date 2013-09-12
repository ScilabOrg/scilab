/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SPBUILD_H__
#define __SPBUILD_H__

#include "spDefs.h"
/* TODO: move comments from spBuild.c */

ElementPtr spcFindElementInCol( MatrixPtr Matrix, register ElementPtr *LastAddr, register int Row, int Col, SPBOOLEAN CreateIfMissing );

ElementPtr spcCreateElement( MatrixPtr Matrix, int Row, register int Col, register ElementPtr  * LastAddr, SPBOOLEAN Fillin );

int spcLinkRows( MatrixPtr Matrix );

#endif /* __SPBUILD_H__ */
