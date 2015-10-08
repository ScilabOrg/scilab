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
TIType Checkers::check_____oror____(GVN & gvn, const TIType & in0, const TIType & in1)
{
    switch (in0.type)
    {
        case TIType::EMPTY :
        {
            switch (in1.type)
            {
                case TIType::BOOLEAN :
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                case TIType::COMPLEX :
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                case TIType::DOUBLE :
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::BOOLEAN :
        {
            switch (in1.type)
            {
                case TIType::EMPTY :
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                case TIType::BOOLEAN :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                case TIType::COMPLEX :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                case TIType::DOUBLE :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::COMPLEX :
        {
            switch (in1.type)
            {
                case TIType::EMPTY :
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                case TIType::BOOLEAN :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                case TIType::COMPLEX :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::BOOLEAN, 1, 1);
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                case TIType::DOUBLE :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::BOOLEAN, 1, 1);
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::DOUBLE :
        {
            switch (in1.type)
            {
                case TIType::EMPTY :
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                case TIType::BOOLEAN :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                case TIType::COMPLEX :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::BOOLEAN, 1, 1);
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                case TIType::DOUBLE :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::BOOLEAN, 1, 1);
                    }
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT16 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT16, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::INT32, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT16, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT16, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT32 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT32, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT32, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT32, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT32, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT32, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT64 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT8 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::INT16, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::INT32, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::INT8, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT8, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT16 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT32, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT32 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT64 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT8 :
        {
            switch (in1.type)
            {
                case TIType::INT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT16, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::INT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT32, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::INT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, 1, 1);
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::INT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT8, -2, -2);
                }
                case TIType::UINT16 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
                case TIType::UINT32 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
                case TIType::UINT64 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in1;
                    }
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
                case TIType::UINT8 :
                {
                    if (in0.rows == 1 && in0.cols == 1 && in1.rows == 1 && in1.cols == 1)
                    {
                        return in0;
                    }
                    return TIType(gvn, TIType::UINT8, -2, -2);
                }
                default :
                    return TIType(gvn);
            }
            return TIType(gvn);
        }
        default :
            return TIType(gvn);
    }
}

} // namespace analysis