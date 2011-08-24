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

#ifndef __VARIABLESCOPE_HXX__
#define __VARIABLESCOPE_HXX__

#include <iostream>
#include <map>
#include <stack>

extern "C"
{
#include <libxml/xmlmemory.h>
}

namespace org_modules_xml
{
    class XMLObject;

    class VariableScope
    {

        XMLObject ** scope;
        int position;
        int initialSize;
        std::stack<int> * freePlaces;
        static std::map<void *, XMLObject *> * mapLibXMLToXMLObject;
        static xmlFreeFunc XMLFreeFunc;

    public :
        VariableScope(int initialSize);
        ~VariableScope();

        int getVariableId(const XMLObject & obj);
        XMLObject * getVariableFromId(int id);

        static void unregisterPointer(void * libxml);
        static void registerPointers(void * libxml, XMLObject * obj);

        template <class T> void removeId(int id)
            {
                if (id >= 0 && id < initialSize && scope[id])
                {
                    removeDependencies<T>(scope[id]);
                    scope[id] = static_cast<XMLObject *>(0);
                    freePlaces->push(id);
                }
            }

    private :
        static void _xmlFreeFunc(void * mem);
        static void initXMLMemory();
        static xmlFreeFunc getFreeFunc(xmlFreeFunc freeFunc);
        template <class T> void removeDependencies(XMLObject * obj)
            {
                for (int i = 0; i <= position; i++)
                {
                    if (scope[i] && reinterpret_cast<T *>(scope[i])->getXMLObjectParent() == obj)
                    {
                        delete reinterpret_cast<T *>(scope[i]);
                    }
                }
            }
    };
}

#endif
