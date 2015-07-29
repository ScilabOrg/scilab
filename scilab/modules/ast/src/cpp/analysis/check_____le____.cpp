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
    TIType Checkers::check_____le____(GVN & gvn, const TIType & in0, const TIType & in1)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            switch (in1.type)
            {
            case TIType::EMPTY :
            {
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::BOOLEAN :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in1;
                }
                return TIType(gvn);
            }
            case TIType::COMPLEX :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::DOUBLE :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::INT16 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::INT32 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::INT64 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::INT8 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::STRING :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::UINT16 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::UINT32 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::UINT64 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::UINT8 :
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
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
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in0;
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in0;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            default :
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::STRING :
        {
            switch (in1.type)
            {
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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
            case TIType::EMPTY :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
                return TIType(gvn);
            }
            case TIType::BOOLEAN :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return in1;
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return in1;
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::INT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::STRING :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT16 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT32 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT64 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
            }
            case TIType::UINT8 :
            {
                if (in0.rows == 1 && in0.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in1.rows, in1.cols);
                }
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                if (in1.rows == in0.rows && in1.cols == in0.cols)
                {
                    return TIType(gvn, TIType::BOOLEAN, in0.rows, in0.cols);
                }
                return TIType(gvn, TIType::BOOLEAN, 1, 1);
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