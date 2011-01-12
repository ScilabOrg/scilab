/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include "function.hxx"
#include "core_math.h"

namespace types
{
    Function *Function::createFunction(std::wstring _stName, GW_FUNC _pFunc, std::wstring _stModule)
    {
        return new Function(_stName, _pFunc, _stModule);
    }

    Function *Function::createFunction(std::wstring _stName, OLDGW_FUNC _pFunc, std::wstring _stModule)
    {
        return new WrapFunction(_stName, _pFunc, _stModule);
    }

    Function::Function(std::wstring _stName, GW_FUNC _pFunc, std::wstring _stModule) : Callable(), m_pFunc(_pFunc)
    {
        setName(_stName);
        setModule(_stModule);
    }

    Function::~Function()
    {
        if(isDeletable() == true)
        {
        }
    }

    Function* Function::getAsFunction(void)
    {
        return this;
    }

    Function::ReturnValue Function::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
    {
        return this->m_pFunc(in, _iRetCount, out);
    }

    /*--------------*/
    /*		whoIAm		*/
    /*--------------*/
    void Function::whoAmI()
    {
        std::cout << "types::Function";
    }

    std::wstring Function::toString(int _iPrecision, int _iLineLen)
    {
        std::wostringstream ostr;

        // FIXME : Implement me.
        ostr << L"FIXME : Implement Function::toString" << std::endl;

        return ostr.str();
    }

    Function* Function::clone()
    {
        IncreaseRef();
        return this;
    }

    WrapFunction::WrapFunction(std::wstring _stName, OLDGW_FUNC _pFunc, std::wstring _stModule)
    {
        m_stName = _stName;
        m_pOldFunc = _pFunc;
        m_stModule = _stModule;
    }

    WrapFunction::WrapFunction(WrapFunction* _pWrapFunction)
    {
        m_stModule  = _pWrapFunction->getModule();
        m_stName    = _pWrapFunction->getName();
        m_pOldFunc  = _pWrapFunction->getFunc();
    }

    WrapFunction* WrapFunction::clone()
    {
        return new WrapFunction(this);
    }

    Function::ReturnValue WrapFunction::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
    {
        ReturnValue retVal = Callable::OK;
        GatewayStruct* pStr;
        int iRet ;
        pStr = new GatewayStruct();

        _iRetCount = Max(1, _iRetCount);
        pStr->m_pIn = &in;
        out.resize( MAX_OUTPUT_VARIABLE );
        pStr->m_pOut = &out[0];
        pStr->m_piRetCount = &_iRetCount;
        pStr->m_pstName = const_cast<wchar_t*>(m_stName.c_str());
        pStr->m_pOutOrder = new int[_iRetCount < 1 ? 1 : _iRetCount];
        memset(pStr->m_pOutOrder, 0xFF, (_iRetCount < 1 ? 1 : _iRetCount) * sizeof(int));
        memset(pStr->m_pOut, 0x00, MAX_OUTPUT_VARIABLE * sizeof(InternalType*));

        //call gateway
        iRet = m_pOldFunc((char*)m_stName.c_str(), (int*)pStr);

        if(iRet != 0)
        {
            retVal = Callable::Error;
            out.clear();
        }
        else
        {
            out.resize( _iRetCount);
        }
        delete[] pStr->m_pOutOrder;
        delete pStr;
        return retVal;
    }
}
