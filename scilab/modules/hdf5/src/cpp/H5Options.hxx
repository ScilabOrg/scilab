/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5OPTIONS_HXX__
#define __H5OPTIONS_HXX__

#include <algorithm>
#include <string>
#include <vector>

#include "H5Exception.hxx"

extern "C"
{
#include "localization.h"
}

namespace org_modules_hdf5
{

class H5Options
{
    enum RowOrder {
        C,
        FORTRAN
    };

    static RowOrder writeStyle;
    static RowOrder readStyle;

public:

    static void setOption(const std::string & op, const std::string & style)
    {
        std::string upperStyle(style);
        std::transform(style.begin(), style.end(), upperStyle.begin(), toupper);
        std::string upperOp(op);
        std::transform(op.begin(), op.end(), upperOp.begin(), toupper);
        RowOrder order;

        if (upperStyle == "C")
        {
            order = C;
        }
        else if (upperStyle == "FORTRAN")
        {
            order = FORTRAN;
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid option: must be 'C' or 'Fortran'."));
        }

        if (upperOp == "READ")
        {
            setReadStyle(order);
        }
        else if (upperOp == "WRITE")
        {
            setWriteStyle(order);
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid option: must be 'read' or 'write'."));
        }
    }

    static std::vector<std::string> getOptions()
    {
        std::vector<std::string> v;

        v.push_back("read");
        v.push_back("write");

        v.push_back(isReadFlip() ? "Fortran" : "C");
        v.push_back(isWriteFlip() ? "Fortran" : "C");

        return v;
    }

    static bool isWriteFlip()
    {
        return writeStyle == FORTRAN;
    }

    static bool isReadFlip()
    {
        return readStyle == FORTRAN;
    }

private:

    static void setWriteStyle(const RowOrder order)
    {
        writeStyle = order;
    }

    static void setReadStyle(const RowOrder order)
    {
        readStyle = order;
    }
};
}

#endif // __H5OPTIONS_HXX__
