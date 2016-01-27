/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
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
