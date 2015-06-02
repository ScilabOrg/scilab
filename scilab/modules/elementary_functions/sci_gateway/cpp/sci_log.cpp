/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"


extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "elem_common.h"
#include "log.h"
    int C2F(wlog)(double*, double*, double*, double*);
}
/*
clear a;nb = 2500;a = rand(nb, nb);tic();log(a);toc
clear a;nb = 2500;a = -rand(nb, nb);tic();log(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();log(a);toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_log(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iAlert = 1;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "log", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "log", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        ast::ExecVisitor exec;
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_log";
        return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
    }

    types::Double* pDblIn = in[0]->getAs<types::Double>();
    types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

    double* pInR = pDblIn->get();
    double* pOutR = pDblOut->get();
    int size = pDblIn->getSize();
    if (pDblIn->isComplex())
    {
        double* pInI = pDblIn->getImg();
        double* pOutI = pDblOut->getImg();
        for (int i = 0; i < size; i++)
        {
            //If the value is less than precision (eps).
            if (iAlert && pInR[i] == 0 && pInI[i] == 0)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d : Singularity of the function.\n"), "log", 1);
                    return types::Function::Error;
                }
                else if (ConfigVariable::getIeee() == 1)
                {
                    if (ConfigVariable::getWarningMode())
                    {
                        sciprint(_("%ls: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), "log", 1);
                    }
                }

                iAlert = 0;
            }

            C2F(wlog)(pInR + i, pInI + i, pOutR + i, pOutI + i);
        }
    }
    else
    {
        bool bIsLessZero = false;
        for (int i = 0; i < size; i++)
        {
            if (iAlert && pInR[i] == 0)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d : Singularity of the function.\n"), "log", 1);
                    return types::Function::Error;
                }
                else if (ConfigVariable::getIeee() == 1)
                {
                    if (ConfigVariable::getWarningMode())
                    {
                        sciprint(_("%ls: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), "log", 1);
                    }
                }
                iAlert = 0;
            }
            else if (pInR[i] < 0)
            {
                bIsLessZero = true;
            }
        }

        if (bIsLessZero)
        {
            pDblOut->setComplex(true);
            double zero = 0;
            double* pOutI = pDblOut->getImg();

            for (int i = 0; i < size; i++)
            {
                C2F(wlog)(pInR + i, &zero, pOutR + i, pOutI + i);
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pOutR[i] = std::log(pInR[i]);
            }
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
