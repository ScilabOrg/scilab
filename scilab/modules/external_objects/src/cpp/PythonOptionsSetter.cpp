/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "PythonOptionsHelper.hxx"
#include "PythonOptionsSetter.hxx"

namespace org_modules_external_objects_python
{
const unsigned int PythonOptionsSetter::USENUMPY = ScilabOptionsSetter::LAST + 1;

PythonOptionsSetter::PythonOptionsSetter(PythonOptionsHelper & _helper, unsigned int _type) : ScilabOptionsSetter(_helper, _type) { }

void PythonOptionsSetter::set(const bool state)
{
    if (type == USENUMPY)
    {
        reinterpret_cast<PythonOptionsHelper *>(&helper)->setUseNumpy(state);
    }
    else
    {
        ScilabOptionsSetter::set(state);
    }
}

bool PythonOptionsSetter::get() const
{
    if (type == USENUMPY)
    {
        return reinterpret_cast<PythonOptionsHelper *>(&helper)->getUseNumpy();
    }
    else
    {
        return ScilabOptionsSetter::get();
    }
}
}
