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
#include <vector>
#include "function.hxx"
#include "inspector.hxx"

extern "C"
{
#include "core_math.h"
#include "charEncoding.h"
#include "MALLOC.h"
}

namespace types
{
    Function *Function::createFunction(std::wstring _stName, GW_FUNC _pFunc, std::wstring _stModule)
    {
        return new Function(_stName, _pFunc, NULL, _stModule);
    }

    Function *Function::createFunction(std::wstring _stName, OLDGW_FUNC _pFunc, std::wstring _stModule)
    {
        return new WrapFunction(_stName, _pFunc, NULL, _stModule);
    }

    Function *Function::createFunction(std::wstring _stName, MEXGW_FUNC _pFunc, std::wstring _stModule)
    {
        return new WrapMexFunction(_stName, _pFunc, NULL, _stModule);
    }

    Function *Function::createFunction(std::wstring _stName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _stModule)
    {
        return new Function(_stName, _pFunc, _pLoadDeps, _stModule);
    }

    Function *Function::createFunction(std::wstring _stName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _stModule)
    {
        return new WrapFunction(_stName, _pFunc, _pLoadDeps, _stModule);
    }

    Function *Function::createFunction(std::wstring _stName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _stModule)
    {
        return new WrapMexFunction(_stName, _pFunc, _pLoadDeps, _stModule);
    }

    Function::Function(std::wstring _stName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _stModule) : Callable(), m_pFunc(_pFunc), m_pLoadDeps(_pLoadDeps)
    {
        setName(_stName);
        setModule(_stModule);
//#ifdef __ENABLE_INSPECTOR__
//        Inspector::addItem(this);
//#endif
    }

    Function::~Function()
    {
        if(isDeletable() == true)
        {
        }
//#ifdef __ENABLE_INSPECTOR__
//        Inspector::removeItem(this);
//#endif
    }

    Function* Function::getAsFunction(void)
    {
        return this;
    }

    Function::ReturnValue Function::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
    {
        if (m_pLoadDeps != NULL)
        {
            m_pLoadDeps();
        }
        return this->m_pFunc(in, _iRetCount, out);
    }

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

    InternalType* Function::clone()
    {
        IncreaseRef();
        return this;
    }

    WrapFunction::WrapFunction(std::wstring _stName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _stModule)
    {
        m_stName = _stName;
        m_pOldFunc = _pFunc;
        m_stModule = _stModule;
        m_pLoadDeps = _pLoadDeps;
    }

    WrapFunction::WrapFunction(WrapFunction* _pWrapFunction)
    {
        m_stModule  = _pWrapFunction->getModule();
        m_stName    = _pWrapFunction->getName();
        m_pOldFunc  = _pWrapFunction->getFunc();
        m_pLoadDeps = _pWrapFunction->getDeps();
    }

    InternalType* WrapFunction::clone()
    {
        return new WrapFunction(this);
    }

    Function::ReturnValue WrapFunction::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
    {
        if (m_pLoadDeps != NULL)
        {
            m_pLoadDeps();
        }

        ReturnValue retVal = Callable::OK;
        int iRet ;
        GatewayStruct gStr;
        _iRetCount = Max(1, _iRetCount);
        gStr.m_pIn = &in;
        typed_list::value_type tmpOut[MAX_OUTPUT_VARIABLE];
        std::fill_n(tmpOut, MAX_OUTPUT_VARIABLE, static_cast<typed_list::value_type>(0));
        gStr.m_pOut = tmpOut;
        gStr.m_piRetCount = &_iRetCount;
        gStr.m_pstName = const_cast<wchar_t*>(m_stName.c_str());
        gStr.m_pVisitor = execFunc;
        // we should use a stack array of the max size to avoid dynamic alloc.
        std::vector<int> outOrder(_iRetCount < 1 ? 1 : _iRetCount, -1);
        gStr.m_pOutOrder = &outOrder[0];

        char* pFunctionName = wide_string_to_UTF8(m_stName.c_str());
        //call gateway (thoses cast should looks  suspicious)
        iRet = m_pOldFunc(pFunctionName, reinterpret_cast<int*>(&gStr));
        FREE(pFunctionName);
        if(iRet != 0)
        {
            retVal = Callable::Error;
        }
        else
        {
            for(std::size_t i(0); i != _iRetCount && outOrder[i] != -1 && outOrder[i] != 0 ; ++i)
            {
                std::size_t const iPos(outOrder[i] - in.size() -1);
                out.push_back(tmpOut[iPos]);
                tmpOut[iPos]= 0;
            }
        }
        for(std::size_t i(0); i != MAX_OUTPUT_VARIABLE; ++i)
        {
            delete tmpOut[i];// delete 0 is safe cf.5.3.5/2
        }
        return retVal;
    }

    WrapMexFunction::WrapMexFunction(std::wstring _stName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _stModule)
    {
        m_stName = _stName;
        m_pOldFunc = _pFunc;
        m_stModule = _stModule;
        m_pLoadDeps = _pLoadDeps;
    }

    WrapMexFunction::WrapMexFunction(WrapMexFunction* _pWrapFunction)
    {
        m_stModule  = _pWrapFunction->getModule();
        m_stName    = _pWrapFunction->getName();
        m_pOldFunc  = _pWrapFunction->getFunc();
        m_pLoadDeps = _pWrapFunction->getDeps();
    }

    InternalType* WrapMexFunction::clone()
    {
        return new WrapMexFunction(this);
    }

    Function::ReturnValue WrapMexFunction::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
    {
        if (m_pLoadDeps != NULL)
        {
            m_pLoadDeps();
        }

        ReturnValue retVal = Callable::OK;

        int nlhs = _iRetCount;
        int** plhs = new int*[nlhs];
        memset(plhs, 0x00, sizeof(int*) * nlhs);

        int nrhs = (int)in.size();
        int** prhs = new int*[nrhs];
        for(int i = 0 ; i < nrhs ; i++)
        {
            prhs[i] = (int*)(in[i]);
        }

        m_pOldFunc(nlhs, plhs, nrhs, prhs);

        if(_iRetCount == 1 && plhs[0] == NULL)
        {//dont copy empty values, juste return "no value"
            return retVal;
        }

        for(int i = 0 ; i < nlhs ; i++)
        {
            out.push_back((types::InternalType*)plhs[i]);
        }

        return retVal;
    }
}
