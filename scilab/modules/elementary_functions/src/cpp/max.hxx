/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MAX_H__
#define __MAX_H__

#include "double.hxx"
#include "sparse.hxx"

void max(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut);
void max(std::vector<types::Sparse*> vectIn, int iOrientation, types::Sparse* pDblIndex, types::Sparse* pOut);
void max(types::Sparse* sparseIn, types::Double* pDblIndex, types::Double* pOut);// find the max value between all elements of the sparse. (iOrientation = 0)

#endif /* __MAX_H__ */
