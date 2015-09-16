/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_finite.hxx"

bool isDoubleFinite(types::Double *_pDouble)
{
    int iSize = _pDouble->getSize();
    if (matrix_finite( _pDouble->getReal(), iSize) == 1)
    {
        return false;
    }

    if (_pDouble->isComplex())
    {
        if (matrix_finite(_pDouble->getImg(), iSize) == 1)
        {
            return false;
        }
    }
    return true;
}
