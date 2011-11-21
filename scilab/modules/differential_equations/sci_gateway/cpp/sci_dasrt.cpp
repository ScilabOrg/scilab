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
#include "configvariable.hxx"
#include "runvisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "scifunctions.h"
#include "elem_common.h"
#include "checkodeerror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_dasrt(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input args
    types::Double* pDblX0   = NULL;
    types::Double* pDblT0   = NULL;
    types::Double* pDblT    = NULL;
    types::Double* pDblRtol = NULL;
    types::Double* pDblAtol = NULL;
    types::Double* pDblHd   = NULL;
    types::Double* pDblNg   = NULL;

    // x0 = [y0, ydot0]
    double* pdYData         = NULL; // contain y0 following by all args data in list case.
    double* pdYdotData      = NULL;
    int sizeOfpdYData       = 0;

    int sizeOfYSize = 1;
    int* YSize      = NULL;    // YSize(1) = size of y0,
                               // YSize(n) = size of Args(n) in list case.
    int iPos    = 0;
    int one     = 1;

    int info[15]    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    double tstop    = 0;
    double maxstep  = 0;
    double stepin   = 0;
    int ng          = 0;
    int mu          = 0;
    int ml          = 0;

    // Indicate if the function is given.
    bool bFuncF     = false;
    bool bFuncJac   = false;
    bool bFuncG     = false;

    // Indicate if info list is given.
    bool bListInfo  = false;

// *** check the minimal number of input args. ***
    if(in.size() < 6 || in.size() > 11)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"dasrt", 6, 11);
        return types::Function::Error;
    }

// *** check number of output args ***
    if(_iRetCount != 3 && _iRetCount != 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"dasrt", 2, 3);
        return types::Function::Error;
    }

// *** check type of input args and get it. ***
    // x0 = [y0, yd0]
    if(in[iPos]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"dasrt", iPos+1);
        return types::Function::Error;
    }

    pDblX0 = in[iPos]->getAs<types::Double>();

    if(pDblX0->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"dasrt", iPos+1);
        return types::Function::Error;
    }

    if(pDblX0->getCols() > 2)
    {
        ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A real matrix with %d to %d colomn(s) expected.\n"), L"dasrt", iPos+1, 1, 2);
        return types::Function::Error;
    }

    if(pDblX0->getCols() == 1)
    {
        info[10] = 1;
    }

    // t0
    iPos++;
    if(in[iPos]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A scalar expected.\n"), L"dasrt", iPos+1);
        return types::Function::Error;
    }

    pDblT0 = in[iPos]->getAs<types::Double>();

    if(pDblT0->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A scalar expected.\n"), L"dasrt", iPos+1);
        return types::Function::Error;
    }

    // t
    iPos++;
    if(in[iPos]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"dasrt", iPos+1);
        return types::Function::Error;
    }

    pDblT = in[iPos]->getAs<types::Double>();

    if(pDblT->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"dasrt", iPos+1);
        return types::Function::Error;
    }

    // get next inputs
    DifferentialEquationFunctions* deFunctionsManager = new DifferentialEquationFunctions(L"dasrt");
    ConfigVariable::addDifferentialEquationFunctions(deFunctionsManager);

    YSize = (int*)malloc(sizeOfYSize * sizeof(int));
    *YSize = pDblX0->getRows();

    pdYData = (double*)malloc(*YSize * sizeof(double));
    pdYdotData = (double*)malloc(*YSize * sizeof(double));

    C2F(dcopy)(YSize, pDblX0->get(), &one, pdYData, &one);
    if(pDblX0->getCols() == 2)
    {
        C2F(dcopy)(YSize, pDblX0->get() + *YSize, &one, pdYdotData, &one);
    }
    else
    {
        memset(pdYdotData, 0x00, *YSize);
    }

    deFunctionsManager->setOdeYRows(pDblX0->getRows());

    for(iPos++; iPos < in.size(); iPos++)
    {
        if(in[iPos]->isDouble())
        {
            if(pDblAtol == NULL && bFuncF == false)
            {
                pDblAtol = in[iPos]->getAs<types::Double>();
                if(pDblAtol->getSize() != pDblX0->getRows() && pDblAtol->isScalar() == false)
                {
                    ScierrorW(267,_W("%ls: Wrong size for input argument #%d : A scalar or a matrix of size %d expected.\n"), L"dasrt", iPos+1, pDblX0->getRows());
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }
            }
            else if(pDblRtol == NULL && bFuncF == false)
            {
                pDblRtol = in[iPos]->getAs<types::Double>();
                if(pDblAtol->getSize() != pDblRtol->getSize())
                {
                    ScierrorW(267,_W("%ls: Wrong size for input argument #%d : Atol and Rtol must have the same size.\n"), L"dasrt", iPos+1, pDblX0->getRows());
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }
            }
            else if(pDblNg == NULL && bFuncF == true)
            {
                pDblNg = in[iPos]->getAs<types::Double>();
                if(pDblNg->isScalar() == false)
                {
                    ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A scalar expected.\n"), L"dasrt", iPos+1);
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }
                ng = (int)pDblNg->get(0);
            }
            else if(pDblHd == NULL && bFuncF == true)
            {
                pDblHd = in[iPos]->getAs<types::Double>();
                if(in.size() != iPos+1)
                {
                    ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"dasrt", iPos+1);
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A function expected.\n"), L"dasrt", iPos+1);
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }
        }
        else if(in[iPos]->isCallable())
        {
            types::Callable* pCall = in[iPos]->getAs<types::Callable>();
            if(bFuncF == false)
            {
                deFunctionsManager->setFFunction(pCall);
                bFuncF = true;
            }
            else if(bFuncJac == false && pDblNg == NULL)
            {
                deFunctionsManager->setJacFunction(pCall);
                bFuncJac = true;
            }
            else if(bFuncG == false && pDblNg)
            {
                deFunctionsManager->setGFunction(pCall);
                bFuncG = true;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix or a list expected.\n"), L"dasrt", iPos+1);
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }
        }
        else if(in[iPos]->isString())
        {
            types::String* pStr = in[iPos]->getAs<types::String>();
            bool bOK = false;

            if(bFuncF == false)
            {
                bOK = deFunctionsManager->setFFunction(pStr);
                bFuncF = true;
            }
            else if(bFuncJac == false && pDblNg == NULL)
            {
                bOK = deFunctionsManager->setJacFunction(pStr);
                bFuncJac = true;
            }
            else if(bFuncG == false && pDblNg)
            {
                bOK = deFunctionsManager->setGFunction(pStr);
                bFuncG = true;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix or a list expected.\n"), L"dasrt", iPos+1);
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }

            if(bOK == false)
            {
                ScierrorW(50,_W("%ls: Subroutine not found: %ls\n"), L"dasrt", pStr->get(0));
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }
        }
        else if(in[iPos]->isList())
        {
            types::List* pList = in[iPos]->getAs<types::List>();

            if(pList->getSize() == 0)
            {
                ScierrorW(50,_W("%ls: Argument #%d : Subroutine not found in list: %ls\n"), L"dasrt", iPos+1, L"(string empty)");
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }

            if(bFuncF && bListInfo)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"dasrt", iPos+1);
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }

            if(pList->get(0)->isString())
            {
                types::String* pStr = pList->get(0)->getAs<types::String>();
                bool bOK = false;

                if(bFuncF == false)
                {
                    bFuncF = true;
                    bOK = deFunctionsManager->setFFunction(pStr);
                    sizeOfpdYData = *YSize;
                }
                else if(bFuncJac == false && pDblNg == NULL)
                {
                    bFuncJac = true;
                    bOK = deFunctionsManager->setJacFunction(pStr);
                    if(sizeOfpdYData == 0)
                    {
                        sizeOfpdYData = *YSize;
                    }
                }
                else if(bFuncG == false && pDblNg)
                {
                    bFuncG = true;
                    bOK = deFunctionsManager->setGFunction(pStr);
                    if(sizeOfpdYData == 0)
                    {
                        sizeOfpdYData = *YSize;
                    }
                }

                if(bOK == false)
                {
                    ScierrorW(50,_W("%ls: Argument #%d : Subroutine not found in list: %ls\n"), L"dasrt", iPos+1, pStr->get(0));
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }

                int* sizeTemp = YSize;
                int totalSize = sizeOfpdYData;

                YSize = (int*)malloc((sizeOfYSize + pList->getSize() - 1) * sizeof(int));
                memcpy(YSize, sizeTemp, sizeOfYSize * sizeof(int));

                std::vector<types::Double*> vpDbl;
                for(int iter = 0; iter < pList->getSize() - 1; iter++)
                {
                    if(pList->get(iter + 1)->isDouble() == false)
                    {
                        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : Argument %d in the list must be a matrix.\n"), L"dasrt", iPos+1, iter+1);
                        ConfigVariable::removeDifferentialEquationFunctions();
                        free(pdYdotData);free(pdYData);free(YSize);
                        return types::Function::Error;
                    }

                    vpDbl.push_back(pList->get(iter+1)->getAs<types::Double>());
                    YSize[sizeOfYSize + iter] = vpDbl[iter]->getSize();
                    totalSize += YSize[sizeOfYSize + iter];
                }

                double* pdYDataTemp = pdYData;
                pdYData = (double*)malloc(totalSize * sizeof(double));
                C2F(dcopy)(&sizeOfpdYData, pdYDataTemp, &one, pdYData, &one);

                int position = sizeOfpdYData;
                for(int iter = 0; iter < pList->getSize()-1; iter++)
                {
                    C2F(dcopy)(&YSize[sizeOfYSize + iter], vpDbl[iter]->get(), &one, &pdYData[position], &one);
                    position += vpDbl[iter]->getSize();
                }
                vpDbl.clear();
                sizeOfpdYData = totalSize;
                sizeOfYSize += pList->getSize() - 1;
                free(pdYDataTemp);
                free(sizeTemp);
            }
            else if(pList->get(0)->isCallable())
            {
                if(bFuncF == false)
                {
                    bFuncF = true;
                    deFunctionsManager->setFFunction(pList->get(0)->getAs<types::Callable>());
                    for(int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager->setFArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if(bFuncJac == false && pDblNg == NULL)
                {
                    bFuncJac = true;
                    deFunctionsManager->setJacFunction(pList->get(0)->getAs<types::Callable>());
                    for(int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager->setJacArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if(bFuncG == false && pDblNg)
                {
                    bFuncG = true;
                    deFunctionsManager->setGFunction(pList->get(0)->getAs<types::Callable>());
                    for(int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager->setGArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
            }
            else if(pList->get(0)->isDouble() && bFuncF == true)
            {
                if(pList->getSize() != 7)
                {
                    ScierrorW(267,_W("%ls: Wrong size for input argument #%d : A list of size %d expected.\n"), L"dasrt", iPos+1, 7);
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }

                for(int i = 0; i < 7; i++) // info = list([],0,[],[],[],0,0)
                {
                    if(pList->get(i)->isDouble() == false || (pList->get(i)->getAs<types::Double>()->isScalar() == false && (i == 1 || i == 5 || i == 6)))
                    {
                        if(i == 1 || i == 5 || i == 6)
                        {
                            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : Element %d in the info list must be a scalar.\n"), L"dasrt", iPos+1, i);
                        }
                        else
                        {
                            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : Element %d in the info list must be a matrix.\n"), L"dasrt", iPos+1, i);
                        }
                        ConfigVariable::removeDifferentialEquationFunctions();
                        free(pdYdotData);free(pdYData);free(YSize);
                        return types::Function::Error;  
                    }
                }

                types::Double* pDblTemp = pList->get(0)->getAs<types::Double>();
                if(pDblTemp->getSize() != 0)
                {
                    info[3] = 1;
                    tstop = pDblTemp->get(0);
                }

                info[2] = (int)pList->get(1)->getAs<types::Double>()->get(0);

                pDblTemp = pList->get(2)->getAs<types::Double>();
                if(pDblTemp->getSize() == 2)
                {
                    info[5] = 1;
                    ml = (int)pDblTemp->get(0);
                    mu = (int)pDblTemp->get(1);
                    deFunctionsManager->setMl(ml);
                    deFunctionsManager->setMu(mu);
                }
                else if(pDblTemp->getSize() != 0)
                {
                    ScierrorW(267,_W("%ls: Wrong size for input argument #%d : Argument %d in te list must be of size %d.\n"), L"dasrt", iPos+1, 3, 2);
                    ConfigVariable::removeDifferentialEquationFunctions();
                    free(pdYdotData);free(pdYData);free(YSize);
                    return types::Function::Error;
                }

                pDblTemp = pList->get(3)->getAs<types::Double>();
                if(pDblTemp->getSize() != 0)
                {
                    info[6] = 1;
                    maxstep = pDblTemp->get(0);
                }

                pDblTemp = pList->get(4)->getAs<types::Double>();
                if(pDblTemp->getSize() != 0)
                {
                    info[7] = 1;
                    stepin = pDblTemp->get(0);
                }

                info[9]  = (int)pList->get(5)->getAs<types::Double>()->get(0);
                if(pList->get(6)->getAs<types::Double>()->get(0) == 1)
                {
                    info[10] = 1;
                }

                bListInfo = true;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : The first argument in the list must be a string, a function or a matrix in case of argument info.\n"), L"dasrt", iPos+1);
                ConfigVariable::removeDifferentialEquationFunctions();
                free(pdYdotData);free(pdYData);free(YSize);
                return types::Function::Error;
            }
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix or a function expected.\n"), L"dasrt", iPos+1);
            ConfigVariable::removeDifferentialEquationFunctions();
            free(pdYdotData);free(pdYData);free(YSize);
            return types::Function::Error;
        }
    }

    if(bFuncF == false)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"dasrt", in.size() + 3);
        ConfigVariable::removeDifferentialEquationFunctions();
        free(pdYdotData);free(pdYData);free(YSize);
        return types::Function::Error;
    }

    if(pDblNg == NULL)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"dasrt", in.size() + 2);
        ConfigVariable::removeDifferentialEquationFunctions();
        free(pdYdotData);free(pdYData);free(YSize);
        return types::Function::Error;
    }

    if(bFuncG == false)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"dasrt", in.size() + 1);
        ConfigVariable::removeDifferentialEquationFunctions();
        free(pdYdotData);free(pdYData);free(YSize);
        return types::Function::Error;
    }

    if(bFuncJac == true)
    {
        info[4] = 1;
    }

// *** Initialization. ***
    double t0   = pDblT0->get(0);
    double rpar = 0;
    int ipar    = 0;
    int idid    = 0;
    int maxord  = 5;

    //compute itol and set the tolerances rtol and atol.
    double* rtol = NULL;
    double* atol = NULL;

    if(pDblAtol)
    {
        if(pDblAtol->isScalar())
        {
            atol  = (double*)malloc(sizeof(double));
            *atol = pDblAtol->get(0);
        }
        else
        {
            atol    = pDblAtol->get();
            info[1] = 1;
        }
    }
    else
    {
        atol  = (double*)malloc(sizeof(double));
        *atol = 1.e-7;
    }

    if(pDblRtol)
    {
        if(pDblRtol->isScalar())
        {
            rtol  = (double*)malloc(sizeof(double));
            *rtol = pDblRtol->get(0);
        }
        else
        {
            rtol = pDblRtol->get();
        }
    }
    else // if rtol is not given atol will be used as a scalar.
    {
        if(pDblAtol && pDblAtol->isScalar() == false)// info[1] == 1
        {
            rtol = (double*)malloc(pDblAtol->getSize() * sizeof(double));
            memset(rtol, 1.e-9, pDblAtol->getSize());
        }
        else
        {
            rtol    = (double*)malloc(sizeof(double));
            *rtol   = 1.e-9;
        }
    }

    // Compute rwork, iwork size.
    // Create them.
    int iworksize   = 20 + pDblX0->getRows();
    int rworksize   = 0;
    int* iwork      = NULL;
    double* rwork   = NULL;
    int* root    = NULL;

    if(info[5] == 0)
    {
        rworksize = 50 + (maxord + 4) * pDblX0->getRows() + pDblX0->getRows() * pDblX0->getRows() + 3 * ng;
    }
    else if(info[4] == 1)
    {
        rworksize = 50 + (maxord + 4) * pDblX0->getRows() + (2 * ml + mu + 1) * pDblX0->getRows() + 3 * ng;
    }
    else if(info[4] == 0)
    {
        rworksize = 50 + (maxord + 4) * pDblX0->getRows() + (2 * ml + mu + 1) * pDblX0->getRows() + 2 * (pDblX0->getRows() / (ml + mu + 1) + 1) + 3 * ng;
    }

    iwork = (int*)malloc(iworksize * sizeof(int));
    rwork = (double*)malloc(rworksize * sizeof(double));
    root  = (int*)malloc(ng * sizeof(int));

    if(pDblHd != NULL)
    {
        if(iworksize + rworksize != pDblHd->getSize())
        {
            ScierrorW(77, _W("%ls: Wrong size for input argument(s) %d: %d expected.\n"), L"dasrt", in.size(), iworksize + rworksize);
            ConfigVariable::removeDifferentialEquationFunctions();
            free(pdYdotData);free(pdYData);free(YSize);
            free(iwork);free(rwork);
            if(pDblAtol == NULL || pDblAtol->isScalar()) free(atol);
            if(pDblRtol == NULL || pDblRtol->isScalar()) free(rtol);
            return types::Function::Error;
        }

        C2F(dcopy)(&rworksize, pDblHd->get(), &one, rwork, &one);

        for(int i = 0; i < iworksize; i++)
        {
            iwork[i] = (int)pDblHd->get(rworksize + i);
        }

        info[0] = 1;
    }

    if(info[3] == 1)
    {
        rwork[0] = tstop;
    }

    if(info[6] == 1)
    {
        rwork[1] = maxstep;
    }

    if(info[7] == 1)
    {
        rwork[2] = stepin;
    }

    if(info[5] == 1)
    {
        iwork[0] = ml;
        iwork[1] = mu;
    }

// *** Perform operation. ***
    std::list<types::Double*> lpDblOut;
    int size = pDblX0->getRows();
    int rowsOut = 1 + pDblX0->getRows() * 2;

    for(int i = 0; i < pDblT->getSize(); i++)
    {
        types::Double* pDblOut = new types::Double(rowsOut, 1);
        lpDblOut.push_back(pDblOut);

        double t = pDblT->get(i);
        int pos  = 0;
        pDblOut->set(pos, t);

        if(t == t0)
        {
            pos++;
            C2F(dcopy)(&size, pdYData, &one, pDblOut->get() + pos, &one);
            pos += pDblX0->getRows();
            C2F(dcopy)(&size, pdYdotData, &one, pDblOut->get() + pos, &one);

            continue;
        }

        C2F(ddasrt)(dassl_f, YSize, &t0, pdYData, pdYdotData, &t, info, rtol, atol, &idid, rwork, &rworksize, iwork, &iworksize, &rpar, &ipar, dassl_jac, dasrt_g, &ng, root);

        int iret = checkDasslError(idid);

        if(iret == 1) // error
        {
            ScierrorW(999, _W("%ls: ddasrt return with state %d.\n"), L"dasrt", idid);
            lpDblOut.clear();
            ConfigVariable::removeDifferentialEquationFunctions();
            free(pdYdotData);free(pdYData);free(YSize);
            free(iwork);free(rwork);free(root);
            if(pDblAtol == NULL || pDblAtol->isScalar()) free(atol);
            if(pDblRtol == NULL || pDblRtol->isScalar()) free(rtol);
            return types::Function::Error;
        }

        pos++;
        C2F(dcopy)(&size, pdYData, &one, pDblOut->get() + pos, &one);
        pos += size;
        C2F(dcopy)(&size, pdYdotData, &one, pDblOut->get() + pos, &one);

        if(iret == 2) // warning
        {
            pDblOut->set(0, t0);
            break;
        }

        // iret == 0
        if(idid == 1)
        {
            pDblOut->set(0, t0);
            i--;
        }
        else if(idid == -2)
        {
            t0 = t;
            i--;
        }
        else
        {
            t0 = t;
        }

        info[0] = 1;
    }

// *** Return result in Scilab. ***
    types::Double* pDblOut = new types::Double(rowsOut, lpDblOut.size());

    int sizeOfList = lpDblOut.size();
    for(int i = 0; i < sizeOfList; i++)
    {
        int pos = i * rowsOut;
        C2F(dcopy)(&rowsOut, lpDblOut.front()->get(), &one, pDblOut->get() + pos, &one);
        lpDblOut.pop_front();
    }
    out.push_back(pDblOut);


    int sizeOfRoot = 1;
    for(int i = 0; i < ng; i++)
    {
        if(root[i])
        {
            sizeOfRoot++;
        }
    }
    types::Double* pDblRoot = new types::Double(1, sizeOfRoot);
    pDblRoot->set(0,t0);
    int j = 0;
    for(int i = 0; i < ng; i++)
    {
        if(root[i])
        {
            j++;
            pDblRoot->set(j, i+1);
        }
    }
    out.push_back(pDblRoot);

    if(_iRetCount == 3)
    {
        types::Double* pDblHdOut = new types::Double(rworksize + iworksize, 1);
        C2F(dcopy)(&rworksize, rwork, &one, pDblHdOut->get(), &one);

        for(int i = 0; i < iworksize; i++)
        {
            pDblHdOut->set(rworksize + i, (double)iwork[i]);
        }

        out.push_back(pDblHdOut);
    }

// *** free. ***
    if(pDblAtol == NULL || pDblAtol->isScalar())
    {
        free(atol);
    }

    if(pDblRtol == NULL || pDblRtol->isScalar())
    {
        free(rtol);
    }

    free(pdYData);
    free(pdYdotData);
    free(YSize);
    free(rwork);
    free(iwork);
    free(root);

    ConfigVariable::removeDifferentialEquationFunctions();

    return types::Function::OK;
}

