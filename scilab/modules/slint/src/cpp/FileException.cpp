/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FileException.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
}

#include <iostream>

namespace slint
{

FileException::FileException(const std::wstring & filename, const std::wstring & error)
{
    char * _filename = wide_string_to_UTF8(filename.c_str());
    char * _error = wide_string_to_UTF8(error.c_str());
    msg = std::string("Error with file ") + _filename + ":\n" + _error;
    FREE(_filename);
    FREE(_error);
}

const char * FileException::what() const noexcept
{
    return msg.c_str();
}

} // namespace slint
