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

/*--------------------------------------------------------------------------*/

#ifndef __PYTHONOPTIONSHELPER_HXX__
#define __PYTHONOPTIONSHELPER_HXX__

#include "OptionsHelper.hxx"
#include "PythonOptionsSetter.hxx"

#include <Python.h>

using namespace org_modules_external_objects;

namespace org_modules_external_objects_python
{

class PythonOptionsHelper : public OptionsHelper
{
    bool useNumpy;
    bool showPrivate;
    bool attachModule;
    bool wrapSingleWithNumpy;

public :

    PythonOptionsHelper () : useNumpy(false), showPrivate(false), attachModule(false), wrapSingleWithNumpy(false) { }

    ~PythonOptionsHelper() { }

    void setUseNumpy(const bool _useNumpy)
    {
        useNumpy = _useNumpy;
    }

    bool getUseNumpy() const
    {
        return useNumpy;
    }

    void setShowPrivate(const bool _showPrivate)
    {
        showPrivate = _showPrivate;
    }

    bool getShowPrivate() const
    {
        return showPrivate;
    }

    void setAttachModule(const bool _attachModule)
    {
        attachModule = _attachModule;
    }

    bool getAttachModule() const
    {
        return attachModule;
    }

    void setWrapSingleWithNumpy(const bool _wrapSingleWithNumpy)
    {
        wrapSingleWithNumpy  = _wrapSingleWithNumpy;
    }

    bool getWrapSingleWithNumpy() const
    {
        return wrapSingleWithNumpy;
    }

    PythonOptionsSetter getSetter(unsigned int type)
    {
        return PythonOptionsSetter(*this, type);
    }
};

}

#endif // __PYTHONOPTIONSHELPER_HXX__
