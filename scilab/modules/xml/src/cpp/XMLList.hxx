/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XMLLIST_HXX__
#define __XMLLIST_HXX__

#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLObject;

    class XMLList : public XMLObject
    {

    public :
        XMLList();

        virtual const XMLObject * getListElement(int index) = 0;

        int getSize() const { return size; }
        const std::string toString() const;

    protected :
        int size;

        template <typename T>
        T * getListElement(int index, int * prev, T ** prevElem) const
            {
                if (index >= 1 && index <= size)
                {
                    if (index != *prev)
                    {
                        if (index < *prev)
                        {
                            for (int i = *prev; i > index; i--, *prevElem = (*prevElem)->prev);
                        }
                        else
                        {
                            for (int i = *prev; i < index; i++, *prevElem = (*prevElem)->next);
                        }
                        *prev = index;
                    }

                    return *prevElem;
                }

                return 0;
            }
    };
}

#endif

