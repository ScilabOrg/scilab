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

#ifndef __SCILABOPTIONSSETTER_HXX__
#define __SCILABOPTIONSSETTER_HXX__

namespace org_modules_external_objects
{

typedef enum OptionType {METHODOFCONV, ALLOWRELOAD, AUTOUNWRAP, USECOPY} OptionType;

class

    class ScilabOptionsSetter
    {

        OptionsHelper & helper;
        OptionType type;

    public:

        ScilabOptionsSetter(const OptionsHelper & _helper, OptionType _type) : helper(_helper), type(_type) { }
        ~ScilabOptionsSetter() { }

        virtual void set(const bool state)
        {
            switch (type)
            {
                case METHODOFCONV:
                    helper.setMethodOfConv(state);
                    break;
                case ALLOWRELOAD:
                    helper.setAllowReload(state);
                    break;
                case AUTOUNWRAP:
                    helper.setAutoUnwrap(state);
                    break;
                case USECOPY:
                    helper.setUseCopy(state);
                    break;
            }
        }

        virtual bool get()
        {
            switch (type)
            {
                case METHODOFCONV:
                    return helper.getMethodOfConv();
                case ALLOWRELOAD:
                    return helper.getAllowReload();
                case AUTOUNWRAP:
                    return helper.getAutoUnwrap();
                case USECOPY:
                    return helper.getUseCopy();
            }
        }
    };
}

#endif //__SCILABOPTIONSSETTER_HXX__
