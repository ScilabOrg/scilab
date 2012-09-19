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

#include "H5File.hxx"
#include "H5Dataspace.hxx"

namespace org_modules_hdf5
{

H5Dataspace::H5Dataspace(H5Object & _parent, hid_t _space) : H5Object(_parent), space(_space)
{
}

H5Dataspace::~H5Dataspace()
{
    if (space >= 0)
    {
        H5Sclose(space);
    }
}

hid_t H5Dataspace::getH5Id()
{
    return space;
}

std::string H5Dataspace::getTypeName() const
{
    H5S_class_t _class = H5Sget_simple_extent_type(space);
    switch (_class)
    {
        case H5S_SCALAR:
            return "scalar";
        case H5S_SIMPLE:
            return "simple";
        case H5S_NULL:
            return "null";
        case H5S_NO_CLASS:
            return "no class";
        default:
            return std::string(_("unknown dataspace"));
    }
}

std::vector<unsigned int> H5Dataspace::getDims() const
{
    hsize_t dims[64];
    hsize_t maxdims[64];
    int ndims;
    std::vector<unsigned int> vdims;
    H5S_class_t _class = H5Sget_simple_extent_type(space);

    switch (_class)
    {
        case H5S_SCALAR:
            vdims.push_back(1);
            break;
        case H5S_SIMPLE:
            ndims = H5Sget_simple_extent_dims(space, (hsize_t *)dims, (hsize_t *)maxdims);
            vdims.reserve(ndims);
            for (int i = 0; i < ndims; i++)
            {
                vdims.push_back(static_cast<unsigned int>(dims[i]));
            }
            break;
        case H5S_NULL:
            vdims.push_back(0);
            break;
        case H5S_NO_CLASS:
        default:
            throw H5Exception(__LINE__, __FILE__, _("Unknown dataspace: cannot get its dimensions"));
    }

    return vdims;
}

void H5Dataspace::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "dimensions")
    {
        std::vector<unsigned int> dims = getDims();
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, dims.size(), &(dims[0]));
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an array of integer on the stack."));
        }

        return;
    }
    else if (lower == "type")
    {
        std::string type = getTypeName();
        const char * _type = type.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_type);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5Dataspace::dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    hsize_t dims[64];
    hsize_t maxdims[64];
    int ndims;
    H5S_class_t _class = H5Sget_simple_extent_type(space);

    os << H5Object::getIndentString(indentLevel);

    switch (_class)
    {
        case H5S_SCALAR:
            os << "DATASPACE SCALAR";
            break;
        case H5S_SIMPLE:
            os << "DATASPACE SIMPLE { ";
            ndims = H5Sget_simple_extent_dims(space, (hsize_t *)dims, (hsize_t *)maxdims);
            os << "( ";
            for (int i = 0; i < ndims - 1; i++)
            {
                os << dims[i] << ", ";
            }
            os << dims[ndims - 1] << " ) / ( ";
            for (int i = 0; i < ndims - 1; i++)
            {
                if (maxdims[i] == H5S_UNLIMITED)
                {
                    os << "H5S_UNLIMITED, ";
                }
                else
                {
                    os << maxdims[i] << ", ";
                }
            }
            if (maxdims[ndims - 1] == H5S_UNLIMITED)
            {
                os << "H5S_UNLIMITED ) }";
            }
            else
            {
                os << maxdims[ndims - 1] << " ) }";
            }
            break;
        case H5S_NULL:
            os << "DATASPACE NULL";
            break;
        case H5S_NO_CLASS:
        default:
            os << _("Unknown dataspace");
    }

    os << std::endl;

    return os.str();
}

std::string H5Dataspace::toString(unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);
    std::string type = getTypeName();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Dataspace" << std::endl
       << indentString << _("Filename") << ": " << getParent().getFile().getFileName() << std::endl
       << indentString << _("Name") << ": " << getName() << std::endl
       << indentString << _("Path") << ": " << getCompletePath() << std::endl
       << indentString << _("Type") << ": " << getTypeName();

    if (type == "simple")
    {
        os << std::endl << indentString << _("Dimensions") << ": [1 x " << getDims().size() << "]";
    }

    return os.str();
}
}
