/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include "api_handle.hxx"

namespace api_scilab
{
GraphicHandle::GraphicHandle(long long _handle)
{
    data = new types::GraphicHandle(_handle);
    created = true;
}

GraphicHandle::GraphicHandle(int _iRows, int _iCols)
{
    data = new types::GraphicHandle(_iRows, _iCols);
    created = true;
}

GraphicHandle::GraphicHandle(int _iDims, int* _piDims)
{
    data = new types::GraphicHandle(_iDims, _piDims);
    created = true;
}

GraphicHandle* GraphicHandle::getAsGraphicHandle(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isHandle())
    {
        return new GraphicHandle(*_pIT->getAs<types::GraphicHandle>());
    }

    return NULL;
}
}
