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

#include "PCREException.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace slint
{

PCREException::PCREException(const std::wstring & pattern, const std::string & error, const int offset)
{
    char * _pattern = wide_string_to_UTF8(pattern.c_str());
    msg = std::string("Error in regex pattern: ") + error + "\n" + std::string(_pattern) + "\n" + std::string(offset, ' ') + "^";
    FREE(_pattern);
}

const char * PCREException::what() const
{
    return msg.c_str();
}

} // namespace slint
