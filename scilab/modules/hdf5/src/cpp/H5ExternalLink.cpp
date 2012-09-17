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

#include "H5ExternalLink.hxx"

namespace org_modules_hdf5
{

std::vector<std::string> H5ExternalLink::getLinkTargets() const
{
    herr_t err;
    H5L_info_t info;
    char * buf = 0;
    std::vector<std::string> ret;
    const char * filename = 0;
    const char * obj_path = 0;
    unsigned int flags;

    err = H5Lget_info(getParent().getH5Id(), name.c_str(), &info, H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link info"));
    }

    buf = new char[info.u.val_size];
    err = H5Lget_val(getParent().getH5Id(), name.c_str(), static_cast<void *>(buf), info.u.val_size, H5P_DEFAULT);
    if (err < 0)
    {
        delete[] buf;
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link target"));
    }

    // According to the doc the flags arg is useless
    // For the future: follow the evolution of this argument.
    err = H5Lunpack_elink_val(buf, info.u.val_size, &flags, &filename, &obj_path);
    if (err < 0)
    {
        delete[] buf;
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link target"));
    }

    ret.reserve(2);
    ret[0] = std::string(filename);
    ret[1] = std::string(obj_path);

    delete[] buf;

    return ret;
}

std::string H5ExternalLink::getLinkType() const
{
    return "external";
}

void H5ExternalLink::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "type")
    {
        const char * _type = getLinkType().c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_type);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "target")
    {
        const char * _target[2];
        std::vector<std::string> target = getLinkTargets();
        _target[0] = target[0].c_str();
        _target[1] = target[1].c_str();

        err = createMatrixOfString(pvApiCtx, pos, 1, 2, _target);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5ExternalLink::dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::vector<std::string> target = getLinkTargets();

    os << H5Object::getIndentString(indentLevel) << "EXTERNAL_LINK \"" << name << "\" {" << std::endl
       << H5Object::getIndentString(indentLevel + 1) << "TARGETFILE \"" << target[0] << "\"" << std::endl
       << H5Object::getIndentString(indentLevel + 1) << "TARGETPATH \"" << target[1] << "\"" << std::endl
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    return os.str();
}

std::string H5ExternalLink::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::vector<std::string> target = getLinkTargets();
    std::string indentString = H5Object::getIndentString(indentLevel);

    os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Link type") << ": " << getLinkType() << std::endl
       << indentString << _("Link name") << ": " << name << std::endl
       << indentString << _("Link path") << ": " << getCompletePath() << std::endl
       << indentString << _("Link target file") << ": " << target[0] << std::endl
       << indentString << _("Link target path") << ": " << target[1] << std::endl;

    return os.str();
}
}
