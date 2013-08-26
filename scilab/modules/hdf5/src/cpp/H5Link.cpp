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

#include "H5Link.hxx"
#include "H5SoftLink.hxx"
#include "H5ExternalLink.hxx"
#include "H5HardLink.hxx"
#include "H5Object.hxx"

namespace org_modules_hdf5
{

H5Link::H5Link(H5Object & _parent, const std::string & _name) : H5Object(_parent, _name)
{
    if (H5Lexists(_parent.getH5Id(), name.c_str(), H5P_DEFAULT) <= 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("The link %s does not exist."), name.c_str());
    }
}

H5Link::~H5Link()
{

}

H5Link & H5Link::getLink(H5Object & _parent, const std::string & _name)
{
    return getLink(_parent, _name.c_str());
}

H5Link & H5Link::getLink(H5Object & _parent, const char * _name)
{
    herr_t err;
    H5L_info_t info;
    H5Link * link = 0;
    err = H5Lget_info(_parent.getH5Id(), _name, &info, H5P_DEFAULT);

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link info"));
    }

    switch (info.type)
    {
        case H5L_TYPE_HARD:
            link = new H5HardLink(_parent, _name);
            break;
        case H5L_TYPE_SOFT:
            link = new H5SoftLink(_parent, _name);
            break;
        case H5L_TYPE_EXTERNAL:
            link = new H5ExternalLink(_parent, _name);
            break;
        case H5L_TYPE_ERROR:
        default:
            throw H5Exception(__LINE__, __FILE__, _("Invalid link type: %s."), _name);
    }

    return *link;
}
}
