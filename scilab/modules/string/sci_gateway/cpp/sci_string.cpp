/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <math.h>
#include <sstream>
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"
#include "tostring_common.hxx"

extern "C"
{
#include "os_wcsdup.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

static void DoubleComplexMatrix2String(std::wostringstream *_postr,  double _dblR, double _dblI)
{
    /*
    if R && !C -> R
    if R && C -> R + Ci
    if !R && !C -> 0
    if(!R aa C	-> Ci
    */
    DoubleFormat dfR, dfI;
    getDoubleFormat(_dblR, &dfR);
    getDoubleFormat(_dblI, &dfI);

    dfR.bPrintPoint = false;
    dfR.bPaddSign = false;
    dfR.iSignLen = 0;

    dfI.bPrintPoint = false;
    dfI.bPaddSign = false;
    dfI.iSignLen = 0;


    if(_dblR == 0)
    {//no real part
        if(_dblI == 0)
        {//no imaginary part

            //0
            addDoubleValue(_postr, 0, &dfR);
        }
        else
        {//imaginary part

            //I
            *_postr << (_dblI < 0 ? L"-" : L"");
            *_postr << L"%i";
            if(fabs(_dblI) != 1)
            {//specail case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
    else
    {//real part
        if(_dblI == 0)
        {//no imaginary part

            //R
            addDoubleValue(_postr, _dblR, &dfR);
        }
        else
        {//imaginary part

            //R
            addDoubleValue(_postr, _dblR, &dfR);
            //I
            *_postr << (_dblI < 0 ? L"-%i" : L"+%i");
            if(fabs(_dblI) != 1)
            {//special case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                addDoubleValue(_postr, fabs(_dblI), &dfI);
            }
        }
    }
}

Function::ReturnValue sci_string(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"string", 1);
        return Function::Error;
    }

    switch(in[0]->getType())
    {
    case GenericType::RealDouble :
        {
            int iRows = in[0]->getAs<Double>()->getRows();
            int iCols = in[0]->getAs<Double>()->getCols();

            // Special case string([]) == []
            if(iRows == 0 && iCols == 0)
            {
                out.push_back(Double::Empty());
                return Function::OK;
            }
            else if(iRows == -1 && iCols == -1)
            {
                out.push_back(new String(L""));
                return Function::OK;
            }


            String *pstOutput = new String(iRows, iCols);
            for (int i = 0; i < iRows * iCols; ++i)
            {
                std::wostringstream ostr;
                double dblReal = in[0]->getAs<Double>()->getReal()[i];
                double dblImg  = 0.0;
                if (in[0]->getAs<Double>()->isComplex() == true)
                {
                    dblImg  = in[0]->getAs<Double>()->getImg()[i];
                }
                DoubleComplexMatrix2String(&ostr, dblReal, dblImg);
                pstOutput->set(i, ostr.str().c_str());
            }
            out.push_back(pstOutput);
            break;
        }
    case GenericType::RealString :
        {
            out.push_back(in[0]);
            break;
        }
    default :
        {
            out.push_back(new types::String((in[0]->toString()).c_str()));
            break;
        }
    }
    return Function::OK;
}
