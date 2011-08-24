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

#ifndef __XMLRHSVALUE_HXX__
#define __XMLRHSVALUE_HXX__

#include "xmlObjects.h"

extern "C"
{
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
}

namespace org_modules_xml
{

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Base class for the XML objects.
     */
    class XMLRhsValue
    {

    public :

        /**
         * @anchor get
         * @param fname the function name
         * @param addr the Scilab's variable address
         * @param obj a pointer to the object to retrieve
         * @param ptrToFree a pointer to the pointer to freed
         * @return true if all is ok
         */
        static bool get(char * fname, int * addr, XMLDocument ** obj, void ** ptrToFree) { return get<XMLDocument>(fname, addr, obj); }

        /**
         * @ref get
         */
        static bool get(char * fname, int * addr, XMLElement ** obj, void ** ptrToFree) { return get<XMLElement>(fname, addr, obj); }

        /**
         * @ref get
         */
        static bool get(char * fname, int * addr, XMLAttr ** obj, void ** ptrToFree) { return get<XMLAttr>(fname, addr, obj); }

        /**
         * @ref get
         */
        static bool get(char * fname, int * addr, XMLNs ** obj, void ** ptrToFree) { return get<XMLNs>(fname, addr, obj); }

        /**
         * @ref get
         */
        static bool get(char * fname, int * addr, XMLNodeList ** obj, void ** ptrToFree) { return get<XMLNodeList>(fname, addr, obj); }

        /**
         * @ref get
         */
        static bool get(char * fname, int * addr, std::string ** obj, void ** ptrToFree)
            {
                char * str = 0;

                if (getAllocatedSingleString(pvApiCtx, addr, &str))
                {
                    return false;
                }
                *obj = new std::string(str);
                *ptrToFree = str;

                return true;
            }

    private :

        /**
         * @ref get
         */
        template <class T>
        static bool get(char * fname, int * addr, T ** obj)
            {
                int id = getXMLObjectId(addr);
                *obj = XMLObject::getFromId<T>(id);
                if (!*obj)
                {
                    Scierror(999, "%s: XML object does not exist\n", fname);
                    return false;
                }

                return true;
            }
    };
}

#endif
