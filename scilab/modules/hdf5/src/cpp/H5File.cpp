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

#include "H5Group.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{

void H5File::init()
{
    if (!H5Fis_hdf5(filename.c_str()))
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s"), filename.c_str());
    }

    file = H5Fopen(filename.c_str(), readonly ? H5F_ACC_RDONLY : H5F_ACC_RDWR, H5P_DEFAULT);
    if (file < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s"), filename.c_str());
    }
}

H5File::H5File(const char * _filename, const char * _path, const bool _readonly) : H5Object(H5Object::getRoot()), filename(std::string(_filename)), path(std::string(_path)), readonly(_readonly)
{
    init();
}

H5File::H5File(const std::string & _filename, const std::string & _path, const bool _readonly) : H5Object(H5Object::getRoot()), filename(_filename), path(_path), readonly(_readonly)
{
    init();
}

H5File::~H5File()
{
    if (file >= 0)
    {
        H5Fclose(file);
    }
}

H5Group & H5File::getRootGroup()
{
    return *new H5Group(*this, path);
}

std::string H5File::getCompletePath() const
{
    return "";
}

hsize_t H5File::getFileSize() const
{
    herr_t err;
    hsize_t size = 0;

    err = H5Fget_filesize(file, &size);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file size: %s"), filename.c_str());
    }

    return size;
}

void H5File::getFileHDF5Version(unsigned int * out) const
{
    herr_t err = H5get_libversion(out, out + 1, out + 2);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file version: %s"), filename.c_str());
    }
}

void H5File::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "name")
    {
        const char * _filename = filename.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_filename);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "size")
    {
        unsigned int size = (unsigned int)getFileSize();
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, 1, &size);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an integer on the stack."));
        }

        return;
    }
    else if (lower == "version")
    {
        unsigned int version[3];
        getFileHDF5Version(version);
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, 3, (unsigned int *)version);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an array of integer on the stack."));
        }

        return;
    }
    else if (lower == "root")
    {
        H5Group & root = const_cast<H5File *>(this)->getRootGroup();
        root.createOnScilabStack(pos, pvApiCtx);

        return;
    }

    throw H5Exception(__LINE__, __FILE__, _("Invalid field %s."), _name.c_str());
}

std::string H5File::dump(std::set<haddr_t> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    H5Group & _root = const_cast<H5File *>(this)->getRootGroup();
    os << H5Object::getIndentString(indentLevel) << "HDF5 \"" << filename << "\" {" << std::endl
       << _root.dump(alreadyVisited, indentLevel + 1)
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    return os.str();
}

std::string H5File::toString(const unsigned int indentLevel) const
{
    herr_t err;
    hsize_t size = 0;
    std::ostringstream os;
    unsigned int major = 0, minor = 0, release = 0;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);

    err = H5Fget_filesize(file, &size);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file size: %s"), filename.c_str());
    }

    err = H5get_libversion(&major, &minor, &release);

    os << H5Object::getIndentString(indentLevel) << "HDF5 File" << std::endl
       << indentString << _("Filename") << ": " << filename << std::endl
       << indentString << _("HDF5 library version") << ": " << major << "." << minor << "." << release << std::endl
       << indentString << _("File size") << ": " << size << std::endl
       << indentString << _("Group name") << ": " << path << std::flush;

    return os.str();
}
}
