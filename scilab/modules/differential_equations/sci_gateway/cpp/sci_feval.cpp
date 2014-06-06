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

#include "differential_equations_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "callable.hxx"
#include "differentialequationfunctions.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "scifunctions.h"
#include "feval.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_feval(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iPos = 0;
    int nn   = 1;

    //input
    types::Double* pDblX = NULL;
    types::Double* pDblY = NULL;

    // output
    types::Double* pDblOut = NULL;

    // *** check the minimal number of input args. ***
    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "feval", 2, 3);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "feval", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // X
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "feval", iPos + 1);
        return types::Function::Error;
    }
    pDblX = in[iPos]->getAs<types::Double>();
    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "feval", iPos + 1);
        return types::Function::Error;
    }
    iPos++;

    // Y
    if (in.size() == 3)
    {
        if (in[iPos]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "feval", iPos + 1);
            return types::Function::Error;
        }
        pDblY = in[iPos]->getAs<types::Double>();
        if (pDblY->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "feval", iPos + 1);
            return types::Function::Error;
        }
        iPos++;
        nn = 2;
    }

    // function
    DifferentialEquationFunctions* deFunctionsManager = new DifferentialEquationFunctions(L"feval");
    DifferentialEquation::addDifferentialEquationFunctions(deFunctionsManager);

    if (in[iPos]->isCallable())
    {
        types::Callable* pCall = in[iPos]->getAs<types::Callable>();
        deFunctionsManager->setFFunction(pCall);
    }
    else if (in[iPos]->isString())
    {
        bool bOK = false;
        types::String* pStr = in[iPos]->getAs<types::String>();
        bOK = deFunctionsManager->setFFunction(pStr);

        if (bOK == false)
        {
            char* pst = wide_string_to_UTF8(pStr->get(0));
            Scierror(50, _("%s: Subroutine not found: %s\n"), "feval", pst);
            FREE(pst);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else if (in[iPos]->isList())
    {
        types::List* pList = in[iPos]->getAs<types::List>();

        if (pList->getSize() == 0)
        {
            Scierror(50, _("%s: Argument #%d : Subroutine not found in list: %s\n"), "feval", iPos + 1, "(string empty)");
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (pList->get(0)->isCallable())
        {
            deFunctionsManager->setFFunction(pList->get(0)->getAs<types::Callable>());
            for (int iter = 1; iter < pList->getSize(); iter++)
            {
                deFunctionsManager->setFArgs(pList->get(iter)->getAs<types::InternalType>());
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : The first argument in the list must be a Scilab function.\n"), "feval", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A function expected.\n"), "feval", iPos + 1);
        DifferentialEquation::removeDifferentialEquationFunctions();
        return types::Function::Error;
    }

    // *** Perform operation. ***
    int itype       = 0; // output value
    double* res     = (double*)MALLOC(2 * sizeof(double));
    int sizeOfY     = pDblY ? pDblY->getSize() : 1;

    if (nn == 2)
    {
        pDblOut = new types::Double(pDblX->getSize(), sizeOfY);
    }
    else
    {
        pDblOut = new types::Double(pDblX->getRows(), pDblX->getCols());
    }

    for (int y = 0; y < sizeOfY; y++)
    {
        for (int x = 0; x < pDblX->getSize(); x++)
        {
            double valX = pDblX->get(x);
            // if pDblY == NULL, nn == 1 so valY will be never used.
            double valY = pDblY ? pDblY->get(y) : 0;

            try
            {
                deFunctionsManager->execFevalF(&nn, &valX, &valY, res, &itype);
            }
            catch (ast::ScilabError &e)
            {
                char* pstrMsg = wide_string_to_UTF8(e.GetErrorMessage().c_str());
                sciprint(_("%s: exception caught in '%s' subroutine.\n"), "feval", "execFevalF");
                Scierror(999, pstrMsg);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(res);
                delete pDblOut;
                return types::Function::Error;
            }

            if (itype) // is complex
            {
                pDblOut->setComplex(true);
                pDblOut->set(x + y * pDblX->getSize(), res[0]);
                pDblOut->setImg(x + y * pDblX->getSize(), res[1]);
            }
            else
            {
                pDblOut->set(x + y * pDblX->getSize(), res[0]);
            }
        }
    }

    // *** Return result in Scilab. ***

    out.push_back(pDblOut);

    FREE(res);
    DifferentialEquation::removeDifferentialEquationFunctions();

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

