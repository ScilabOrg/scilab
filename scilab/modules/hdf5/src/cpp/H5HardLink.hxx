/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __H5HARDLINK_HXX__
#define __H5HARDLINK_HXX__

#include "H5Link.hxx"

namespace org_modules_hdf5
{

class H5Object;

class H5HardLink : public H5Link
{
public:

    H5HardLink(H5Object & _parent, const char * name) : H5Link(_parent, name) { }
    H5HardLink(H5Object & _parent, const std::string & name) : H5Link(_parent, name) { }

    virtual ~H5HardLink() { }

    virtual std::string toString(const unsigned int indentLevel) const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const;

    H5Object & getLinkedObject() const;
    virtual std::string getLinkType() const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;
};
}

#endif // __H5HARDLINK_HXX__
