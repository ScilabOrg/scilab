/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef BASEOBJECT_HXX_
#define BASEOBJECT_HXX_

#include <string>
extern "C" {
#include <stdlib.h>
}

namespace org_scilab_modules_xcos
{
namespace model
{

class BaseObject
{
public:
    BaseObject(const std::string& uid) : _uid(uid), _id(create_id_from_uid(uid)) {}
    virtual ~BaseObject ();

    const std::string uid();

private:
    /**
     * An UID is unique to all Xcos diagram and use hardware address achieve that purpose.
     */
    const std::string _uid;

    /**
     * An id is used as a key on the diagram and may probably be sufficient for most of the diagrams
     */
    const int _id;

    /**
     * helper used to re-create an int_fast id from the string uid
     */
    inline int create_id_from_uid(const std::string& uid)
    {
        size_t index_found = uid.rfind(':');
        if (index_found < uid.length())
        {
            return a64l(uid.data() + index_found);
        }
        return 0;
    }
};

} /* namespace model */
} /* namespace org_scilab_modules_xcos */

#endif /* BASEOBJECT_HXX_ */
