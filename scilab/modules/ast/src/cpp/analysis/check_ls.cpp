/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

// This file has been generated, so don't modify it by hand !!

#include "checkers/Checkers.hxx"

namespace analysis
{
    TIType Checkers::check_ls(GVN & gvn)
    {
        return TIType(gvn, TIType::STRING, -2, -2);
    }

    TIType Checkers::check_ls(GVN & gvn, const TIType & in0)
    {
        if (in0.type == TIType::STRING)
        {
            return TIType(gvn, TIType::STRING, -2, -2);
        }
        else
        {
            return TIType(gvn);
        }
        return TIType(gvn);
    }

} // namespace analysis