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
#include "localization.h"
#include "Scierror.h"
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
    if(in.size() < 2 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"feval", 2, 3);
        return types::Function::Error;
    }

// *** check number of output args according the methode. ***
    if(_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"feval", 1);
        return types::Function::Error;
    }

// *** check type of input args and get it. ***
    // X
    if(in[iPos]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"feval", iPos+1);
        return types::Function::Error;
    }
    pDblX = in[iPos]->getAs<types::Double>();
    if(pDblX->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"feval", iPos+1);
        return types::Function::Error;
    }
    iPos++;

    // Y
    if(in.size() == 3)
    {
        if(in[iPos]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"feval", iPos+1);
            return types::Function::Error;
        }
        pDblY = in[iPos]->getAs<types::Double>();
        if(pDblY->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"feval", iPos+1);
            return types::Function::Error;
        }
        iPos++;
        nn = 2;
    }

    // function
    DifferentialEquationFunctions* deFunctionsManager = new DifferentialEquationFunctions(L"feval");
    DifferentialEquation::addDifferentialEquationFunctions(deFunctionsManager);

    if(in[iPos]->isCallable())
    {
        types::Callable* pCall = in[iPos]->getAs<types::Callable>();
        deFunctionsManager->setFFunction(pCall);

        // check function
        int iflag   = 0;
        double x    = 1;
        double y    = 1;
        double res[2];

        int ret  = deFunctionsManager->execFevalF(&nn, &x, &y, res, &iflag);
        if(ret)
        {
            ScierrorW(50,_W("%ls: Argument #%d : Variable returned by scilab argument function is incorrect.\n"), L"int3d", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else if(in[iPos]->isString())
    {
        bool bOK = false;
        types::String* pStr = in[iPos]->getAs<types::String>();
        bOK = deFunctionsManager->setFFunction(pStr);

        if(bOK == false)
        {
            ScierrorW(50,_W("%ls: Subroutine not found: %ls\n"), L"feval", pStr->get(0));
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else if(in[iPos]->isList())
    {
        types::List* pList = in[iPos]->getAs<types::List>();

        if(pList->getSize() == 0)
        {
            ScierrorW(50,_W("%ls: Argument #%d : Subroutine not found in list: %ls\n"), L"feval", iPos+1, L"(string empty)");
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if(pList->get(0)->isCallable())
        {
            deFunctionsManager->setFFunction(pList->get(0)->getAs<types::Callable>());
            for(int iter = 1; iter < pList->getSize(); iter++)
            {
                deFunctionsManager->setFArgs(pList->get(iter)->getAs<types::InternalType>());
            }
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : The first argument in the list must be a Scilab function.\n"), L"feval", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A function expected.\n"), L"feval", iPos+1);
        DifferentialEquation::removeDifferentialEquationFunctions();
        return types::Function::Error;
    }

// *** Perform operation. ***
    int itype       = 0; // output value
    double* res     = (double*)malloc(2 * sizeof(double));
    int sizeOfY     = pDblY ? pDblY->getSize() : 1;

    if(nn == 2)
    {
        pDblOut = new types::Double(pDblX->getSize(), sizeOfY);
    }
    else
    {
        pDblOut = new types::Double(pDblX->getRows(), pDblX->getCols());
    }

    for(int y = 0; y < sizeOfY; y++)
    {
        for(int x = 0; x < pDblX->getSize(); x++)
        {
            double valX = pDblX->get(x);
            // if pDblY == NULL, nn == 1 so valY will be never used.
            double valY = pDblY ? pDblY->get(y) : 0;

            int iret = deFunctionsManager->execFevalF(&nn, &valX, &valY, res, &itype);

            if(iret)
            {
                ScierrorW(999, _W("%ls: Error during the function execution.\n"), L"feval");
                DifferentialEquation::removeDifferentialEquationFunctions();
                delete pDblOut;
                return types::Function::Error;
            }

            if(itype)// is complex
            {
                pDblOut->setComplex(true);
                pDblOut->set(x + y*pDblX->getSize(), res[0]);
                pDblOut->setImg(x + y*pDblX->getSize(), res[1]);
            }
            else
            {
                pDblOut->set(x + y*pDblX->getSize(), res[0]);
            }
        }
    }

// *** Return result in Scilab. ***

    out.push_back(pDblOut);

    free(res);
    DifferentialEquation::removeDifferentialEquationFunctions();

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

