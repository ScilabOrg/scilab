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

using namespace org_modules_external_objects;

namespace org_modules_external_objects_python
{
const unsigned int PythonOptionsSetter::USENUMPY = LAST_OPTION_INDEX + 1;
const unsigned int PythonOptionsSetter::SHOWPRIVATE = LAST_OPTION_INDEX + 2;
const unsigned int PythonOptionsSetter::ATTACHMODULE = LAST_OPTION_INDEX + 3;

PythonOptionsSetter::PythonOptionsSetter(PythonOptionsHelper & _helper, unsigned int _type) : ScilabOptionsSetter(_helper, _type) { }

void PythonOptionsSetter::set(const bool state)
{
    switch (type)
    {
        case USENUMPY:
            reinterpret_cast<PythonOptionsHelper *>(&helper)->setUseNumpy(state);
            break;
        case SHOWPRIVATE:
            reinterpret_cast<PythonOptionsHelper *>(&helper)->setShowPrivate(state);
            break;
        case ATTACHMODULE:
            reinterpret_cast<PythonOptionsHelper *>(&helper)->setAttachModule(state);
            break;
        default:
            ScilabOptionsSetter::set(state);
    }
}

bool PythonOptionsSetter::get() const
{
    switch (type)
    {
        case USENUMPY:
            return reinterpret_cast<PythonOptionsHelper *>(&helper)->getUseNumpy();
        case SHOWPRIVATE:
            return reinterpret_cast<PythonOptionsHelper *>(&helper)->getShowPrivate();
        case ATTACHMODULE:
            return reinterpret_cast<PythonOptionsHelper *>(&helper)->getAttachModule();
        default:
            return ScilabOptionsSetter::get();
    }
}
}
