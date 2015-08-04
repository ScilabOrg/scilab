/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This file has been generated, so don't modify it by hand !!

#include "checkers/Checkers.hxx"

namespace analysis
{
    TIType Checkers::check_____transp____(GVN & gvn, const TIType & in0)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            return in0;
        }
        case TIType::BOOLEAN :
        {
            return TIType(gvn, TIType::BOOLEAN, in0.cols, in0.rows);
        }
        case TIType::COMPLEX :
        {
            return TIType(gvn, TIType::COMPLEX, in0.cols, in0.rows);
        }
        case TIType::DOUBLE :
        {
            return TIType(gvn, TIType::DOUBLE, in0.cols, in0.rows);
        }
        case TIType::INT16 :
        {
            return TIType(gvn, TIType::INT16, in0.cols, in0.rows);
        }
        case TIType::INT32 :
        {
            return TIType(gvn, TIType::INT32, in0.cols, in0.rows);
        }
        case TIType::INT64 :
        {
            return TIType(gvn, TIType::INT64, in0.cols, in0.rows);
        }
        case TIType::INT8 :
        {
            return TIType(gvn, TIType::INT8, in0.cols, in0.rows);
        }
        case TIType::STRING :
        {
            return TIType(gvn, TIType::STRING, in0.cols, in0.rows);
        }
        case TIType::UINT16 :
        {
            return TIType(gvn, TIType::UINT16, in0.cols, in0.rows);
        }
        case TIType::UINT32 :
        {
            return TIType(gvn, TIType::UINT32, in0.cols, in0.rows);
        }
        case TIType::UINT64 :
        {
            return TIType(gvn, TIType::UINT64, in0.cols, in0.rows);
        }
        case TIType::UINT8 :
        {
            return TIType(gvn, TIType::UINT8, in0.cols, in0.rows);
        }
        default :
            return TIType(gvn);
        }
    }

} // namespace analysis