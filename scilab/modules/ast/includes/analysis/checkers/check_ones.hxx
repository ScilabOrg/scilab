/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This file has been generated, so don't modify it by hand !!

#ifndef __CHECK_ONES_HXX__
#define __CHECK_ONES_HXX__

#include "TIType.hxx"

namespace analysis
{
    inline static TIType check_ones(GVN & gvn)
    {
        return TIType(gvn, TIType::DOUBLE, 1, 1);
    }

    inline static TIType check_ones(GVN & gvn, const TIType & in0)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            return in0;
        }
        case TIType::BOOLEAN :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::COMPLEX :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::DOUBLE :
        {
            return in0;
        }
        case TIType::INT16 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::INT32 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::INT64 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::INT8 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::STRING :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::UINT16 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::UINT32 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::UINT64 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        case TIType::UINT8 :
        {
            return TIType(gvn, TIType::DOUBLE, in0.rows, in0.cols);
        }
        default :
            return TIType(gvn);
        }
    }

    inline static TIType check_ones(GVN & gvn, const TIType & in0, const TIType & in1)
    {
        if (in0.type == TIType::DOUBLE)
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        else
        {
            return TIType(gvn);
        }
        return TIType(gvn);
    }

} // namespace analysis

#endif // __CHECK_ones_HXX__