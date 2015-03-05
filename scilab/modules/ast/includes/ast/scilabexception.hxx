/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_SCILABEXCEPTION_HXX
#define AST_SCILABEXCEPTION_HXX

#include <string>
#include <stdexcept>
#include "location.hxx"
#include "localization.hxx"

extern "C"
{
#include "lasterror.h"
#include "sci_malloc.h"
#include "localization.h"
}

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

namespace ast
{
class ScilabException : public std::exception
{
public :
    ScilabException()
    {
        createScilabException(L"", 0, Location());
    }

    ScilabException(std::wstring _wstErrorMesssage)
    {
        setLastError(999, _wstErrorMesssage.c_str(), 0, NULL);
        createScilabException(_wstErrorMesssage, 0, Location());
    }

    ScilabException(std::string _stErrorMesssage)
    {
        wchar_t* pwst = to_wide_string(_stErrorMesssage.c_str());
        setLastError(999, pwst, 0, NULL);
        createScilabException(pwst, 0, Location());
        FREE(pwst);
    }
    ScilabException(const Location& _ErrorLocation)
    {
        createScilabException(L"", 0, _ErrorLocation);
    }

    ScilabException(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
    {
        setLastError(_iErrorNumber, _wstErrorMesssage.c_str(), _ErrorLocation.first_line, NULL);
        createScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation);
    }

    virtual ~ScilabException() throw() {};

    void SetErrorMessage(std::wstring _wstErrorMesssage)
    {
        m_wstErrorMessage = _wstErrorMesssage;
    }

    std::wstring GetErrorMessage(void) const
    {
        return m_wstErrorMessage;
    }

    void SetErrorNumber(int _iErrorNumber)
    {
        m_iErrorNumber = _iErrorNumber;
    }

    int GetErrorNumber(void) const
    {
        return m_iErrorNumber;
    }

    void SetErrorLocation(const Location& _ErrorLocation)
    {
        m_ErrorLocation = _ErrorLocation;
    }

    const Location& GetErrorLocation(void) const
    {
        return m_ErrorLocation;
    }
protected :
    std::wstring m_wstErrorMessage;

    int m_iErrorNumber;
    Location m_ErrorLocation;

protected :
    void createScilabException(std::wstring _wstErrorMessage, int _iErrorNumber, const Location& _ErrorLocation)
    {
        m_wstErrorMessage = _wstErrorMessage;
        m_iErrorNumber = _iErrorNumber;
        m_ErrorLocation = _ErrorLocation;
    }
};

class ScilabError : public ScilabException
{
public :
    ScilabError() : ScilabException() {}
    ScilabError(std::wstring _wstErrorMesssage) : ScilabException(_wstErrorMesssage) {}
    ScilabError(std::string _stErrorMesssage) : ScilabException(_stErrorMesssage) {}
    ScilabError(const Location& _ErrorLocation) : ScilabException(_ErrorLocation) {}
    ScilabError(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation) : ScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation) {}
};

class InternalAbort : public ScilabException
{
public :
    InternalAbort() {}
};

class ScilabMessage : public ScilabException
{
public :
    ScilabMessage() : ScilabException() {}
    ScilabMessage(std::wstring _wstErrorMesssage)
    {
        createScilabException(_wstErrorMesssage, 0, Location());
    }

    ScilabMessage(std::string _stErrorMesssage)
    {

        wchar_t* pwst = to_wide_string(_stErrorMesssage.c_str());
        createScilabException(pwst, 0, Location());
        FREE(pwst);
    }

    ScilabMessage(const Location& _ErrorLocation) : ScilabException(_ErrorLocation) {}
    ScilabMessage(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation)
    {
        createScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation);
    }
};
}
#endif // !AST_SCILABEXCEPTION_HXX
