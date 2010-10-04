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
namespace ast
{
    class EXTERN_AST ScilabException : public std::exception
    {
    public :
        ScilabException();
        ScilabException(std::wstring _wstErrorMesssage);
        ScilabException(const Location& _ErrorLocation);
        ScilabException(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation);
        virtual         ~ScilabException(){}

        void            SetErrorMessage(std::wstring _wstErrorMesssage);
        std::wstring    GetErrorMessage(void);

        void            SetErrorNumber(int _iErrorNumber);
        int             GetErrorNumber(void);

        void            SetErrorLocation(const Location& _ErrorLocation);
        Location&       GetErrorLocation(void);
    protected :
        std::wstring    m_wstErrorMessage;

        int             m_iErrorNumber;
        Location        m_ErrorLocation;
    };

    class EXTERN_AST ScilabError : public ScilabException
    {
    public :
        ScilabError() : ScilabException(){}
        ScilabError(std::wstring _wstErrorMesssage) : ScilabException(_wstErrorMesssage){}
        ScilabError(const Location& _ErrorLocation) : ScilabException(_ErrorLocation){}
        ScilabError(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation) : ScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation){}
        virtual         ~ScilabError(){}
    };

    class EXTERN_AST InternalError : public ScilabException
    {
    public :
        InternalError(){}
        virtual         ~InternalError(){}
    };

    class EXTERN_AST ScilabMessage : public ScilabException
    {
    public :
        ScilabMessage() : ScilabException(){}
        ScilabMessage(std::wstring _wstErrorMesssage) : ScilabException(_wstErrorMesssage){}
        ScilabMessage(const Location& _ErrorLocation) : ScilabException(_ErrorLocation){}
        ScilabMessage(std::wstring _wstErrorMesssage, int _iErrorNumber, const Location& _ErrorLocation) : ScilabException(_wstErrorMesssage, _iErrorNumber, _ErrorLocation){}
        virtual         ~ScilabMessage(){}
    };
}
#endif // !AST_SCILABEXCEPTION_HXX
