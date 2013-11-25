/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __EOFIELDSGETTER_HXX__
#define __EOFIELDSGETTER_HXX__

#include <string>
#include <cstring>
#include <vector>
#include "FieldsGetter.hxx"

extern "C"
{
#include "SCIMALLOC.h"
#include "os_strdup.h"
}

namespace org_modules_completion
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to manage the retrievment of the fields name in objects wrapped by External Objects module
 */
class EOFieldsGetter : FieldsGetter
{

public :

    virtual const char ** getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const;

    /**
     * Initialization
     */
    static void initializeEO()
    {
        EOFieldsGetter * getter = new EOFieldsGetter();
        FieldsManager::addFieldsGetter(std::string("_EClass"), getter);
        FieldsManager::addFieldsGetter(std::string("_EObj"), getter);
    }

private :

    EOFieldsGetter() { }

    virtual ~EOFieldsGetter() { }
};
}

#endif
