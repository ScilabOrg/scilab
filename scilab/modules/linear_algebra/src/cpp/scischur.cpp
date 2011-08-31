/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "configvariable.hxx"
#include "callable.hxx"
#include "execvisitor.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
    #include "scischur.h"
}

int schur_dgees(double* _real, double* _complex)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if(pCall)
    {
        typed_list in;
        typed_list out;
        int iRetCount = 1;
        ast::ExecVisitor execFunc;

        types::Double* pDbl = new types::Double(*_real, *_complex);
        pDbl->IncreaseRef();
        in.push_back(pDbl);

        bool bOk = pCall->call(in, iRetCount, out, &execFunc) != types::Function::OK;
        pDbl->DecreaseRef();
        delete pDbl;
        pDbl = NULL;

        if(bOk == false)
        {
            return 0;
        }

        if(out.size() != 1)
        {
            return 0;
        }

        if(out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if(out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

int schur_dgges(double* _real, double* _complex, double* _beta)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if(pCall)
    {
        typed_list in;
        typed_list out;
        int iRetCount = 1;
        ast::ExecVisitor execFunc;

        types::Double* pDblAlpha = new types::Double(*_real, *_complex);
        types::Double* pDblBeta  = new types::Double(*_beta);
        in.push_back(pDblAlpha);
        in.push_back(pDblBeta);

        bool bOk = pCall->call(in, iRetCount, out, &execFunc) != types::Function::OK;
        delete pDblAlpha;
        pDblAlpha = NULL;
        delete pDblBeta;
        pDblBeta = NULL;

        if(bOk == false)
        {
            return 0;
        }

        if(out.size() != 1)
        {
            return 0;
        }
        
        if(out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if(out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

int schur_zgees(doublecomplex* _complex)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if(pCall)
    {
        typed_list in;
        typed_list out;
        int iRetCount = 1;
        ast::ExecVisitor execFunc;

        types::Double* pDbl = new types::Double(_complex->r, _complex->i);
        in.push_back(pDbl);

        bool bOk = pCall->call(in, iRetCount, out, &execFunc) != types::Function::OK;
        delete pDbl;
        pDbl = NULL;

        if(bOk == false)
        {
            return 0;
        }

        if(out.size() != 1)
        {
            return 0;
        }
        
        if(out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if(out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

int schur_zgges(doublecomplex* _alpha, doublecomplex* _beta)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if(pCall)
    {
        typed_list in;
        typed_list out;
        int iRetCount = 1;
        ast::ExecVisitor execFunc;

        types::Double* pDblAlpha = new types::Double(_alpha->r, _alpha->i);
        types::Double* pDblBeta  = new types::Double(_beta->r, _beta->i);
        in.push_back(pDblAlpha);
        in.push_back(pDblBeta);

        bool bOk = pCall->call(in, iRetCount, out, &execFunc) != types::Function::OK;
        delete pDblAlpha;
        pDblAlpha = NULL;
        delete pDblBeta;
        pDblBeta = NULL;

        if(bOk == false)
        {
            return 0;
        }

        if(out.size() != 1)
        {
            return 0;
        }
        
        if(out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if(out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

