/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#ifndef __SCILABOPTIONSSETTER_HXX__
#define __SCILABOPTIONSSETTER_HXX__

#include "ScilabAbstractOptionsSetter.hxx"
#include "dynlib_external_objects_scilab.h"

#define LAST_OPTION_INDEX 4

namespace org_modules_external_objects
{

class OptionsHelper;

class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabOptionsSetter : public ScilabAbstractOptionsSetter
{

protected:

    OptionsHelper & helper;
    unsigned int type;

public:

    static const unsigned int METHODOFCONV;
    static const unsigned int ALLOWRELOAD;
    static const unsigned int AUTOUNWRAP;
    static const unsigned int USECOPY;
    static const unsigned int USESCILABINDEX;

    ScilabOptionsSetter(OptionsHelper & _helper, unsigned int _type) : helper(_helper), type(_type) { }

    ScilabOptionsSetter(const ScilabOptionsSetter & setter) : helper(setter.helper), type(setter.type) { }

    ~ScilabOptionsSetter() { }

    virtual void set(const bool state);

    virtual bool get() const;
};
}

#endif //__SCILABOPTIONSSETTER_HXX__
