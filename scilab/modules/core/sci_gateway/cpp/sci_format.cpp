/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "core_gw.hxx"
#include "function.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
#define e_type_format L"e"
#define v_type_format L"v"
#define format_MIN 2
#define format_MAX 25
#define format_e_MIN 8
#define mode_variable 1
#define mode_e 0

types::Function::ReturnValue sci_format(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iType = -1;
    int iLen = -1;

    if(in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected."), L"format", 0, 2);
        return types::Function::Error;
    }

    if(_iRetCount != 1)
    {
        ScierrorW(78,_W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"format", 0, 1);
        return types::Function::Error;
    }

    if(in.size() == 0)
    {//get
        types::Double* pD = new types::Double(1,2);
        pD->set(0, ConfigVariable::getFormatMode());
        pD->set(1, ConfigVariable::getFormatSize());
        out.push_back(pD);
        return types::Function::OK;
    }
    else if(in.size() == 1 && in[0]->isString())
    {//'e' or 'v'
        types::String *pS = in[0]->getAs<types::String>();
        if(pS->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"format", 1, e_type_format, v_type_format);
            return types::Function::Error;
        }

        wchar_t* pwst = pS->get(0);
        if(wcscmp(pwst, e_type_format) == 0)
        {
            iType = 0;
        }
        else if(wcscmp(pwst, v_type_format) == 0)
        {
            iType = 1;
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"format", 1, e_type_format, v_type_format);
            return types::Function::Error;
        }
    }
    else if(in.size() == 1 && in[0]->isDouble())
    {
        types::Double* pD = in[0]->getAs<types::Double>();
        if(pD->isScalar())
        {//set length
            iLen = (int)pD->get(0);
        }
        else if(pD->getSize() == 2)
        {//set length and type
            iLen = (int)pD->get(0);
            iType = (int)pD->get(1);
            if(iType != 0 && iType != 1)
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d.\n"), L"format", 1);
                return types::Function::Error;
            }
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d.\n"), L"format", 1);
            return types::Function::Error;
        }
    }
    else if(in.size() == 2)
    {
        //first argument
        if(in[0]->isString() == false && in[0]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected."), L"format", 1);
            return types::Function::Error;
        }

        if(in[0]->isString())
        {
            types::String *pS = in[0]->getAs<types::String>();
            if(pS->isScalar() == false)
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"format", 1, e_type_format, v_type_format);
                return types::Function::Error;
            }

            wchar_t* pwst = pS->get(0);
            if(wcscmp(pwst, e_type_format) == 0)
            {
                iType = 0;
            }
            else if(wcscmp(pwst, v_type_format) == 0)
            {
                iType = 1;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"format", 1, e_type_format, v_type_format);
                return types::Function::Error;
            }


            //second argument
            if(in[1]->isDouble() == false)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: An integer expected."), L"format", 2);
                return types::Function::Error;
            }

            types::Double* pD = in[1]->getAs<types::Double>();
            if(pD->isScalar() == false)
            {//set length
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A scalar expected.\n"), L"format", 2);
                return types::Function::Error;
            }

            iLen = (int)pD->get(0);
        }
        else //in[0]->isDouble() == true
        {
            types::Double* pD = in[0]->getAs<types::Double>();
            if(pD->isScalar() == false)
            {//set length
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A scalar expected.\n"), L"format", 1);
                return types::Function::Error;
            }

            iLen = (int)pD->get(0);

            if(in[1]->isString() == false)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected."), L"format", 2);
                return types::Function::Error;
            }

            types::String *pS = in[1]->getAs<types::String>();
            if(pS->isScalar() == false)
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"format", 2, e_type_format, v_type_format);
                return types::Function::Error;
            }

            wchar_t* pwst = pS->get(0);
            if(wcscmp(pwst, e_type_format) == 0)
            {
                iType = 0;
            }
            else if(wcscmp(pwst, v_type_format) == 0)
            {
                iType = 1;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%ls' or '%ls' expected.\n"), L"format", 2, e_type_format, v_type_format);
                return types::Function::Error;
            }
        }
    }
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: a String or Integer expected.\n"), L"format", 1);
        return types::Function::Error;
    }

    if(iType == -1)
    {
        //get current type
        iType = ConfigVariable::getFormatMode();
    }

    if(iLen == -1)
    {
        iLen = ConfigVariable::getFormatSize();
        ConfigVariable::setFormatMode(iType);
        if(iType == 1)
        {
            if(iLen < format_MIN)
            {
                ConfigVariable::setFormatSize(format_MIN);
            }
            else if(iLen > format_MAX)
            {
                ConfigVariable::setFormatSize(format_MAX);
            }
        }
        else
        {
            if(iLen < format_e_MIN)
            {
                ConfigVariable::setFormatSize(format_e_MIN);
            }
            else if(iLen > format_MAX)
            {
                ConfigVariable::setFormatSize(format_MAX);
            }
        }
    }

    if(iType == 1)
    {
        if(iLen < format_MIN || iLen > format_MAX)
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), L"format", 2, format_MIN, format_MAX);
            return types::Function::Error;
        }
        
        ConfigVariable::setFormatSize(iLen);
        ConfigVariable::setFormatMode(iType);
    }
    else
    {//iType == 0
        if(iLen < format_e_MIN || iLen > format_MAX)
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), L"format", 2, format_e_MIN, format_MAX);
            return types::Function::Error;
        }
        
        ConfigVariable::setFormatSize(iLen);
        ConfigVariable::setFormatMode(iType);
    }

    return types::Function::OK;
}
