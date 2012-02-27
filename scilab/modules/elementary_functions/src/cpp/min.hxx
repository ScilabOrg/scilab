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

#ifndef __MIN_H__
#define __MIN_H__

#include "double.hxx"
#include "sparse.hxx"

void min(std::vector<types::Double*> vectIn, int iOrientation, types::Double* pDblIndex, types::Double* pOut);
void min(std::vector<types::Sparse*> vectIn, int iOrientation, types::Sparse* pSparseIndex, types::Sparse* pOut);
void min(types::Sparse* sparseIn, types::Double* pDblIndex, types::Double* pOut);// find the min value between all elements of the sparse. (iOrientation = 0)

#endif /* __MIN_H__ */
