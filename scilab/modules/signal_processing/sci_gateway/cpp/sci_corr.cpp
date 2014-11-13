/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "signal_gw.hxx"
#include "double.hxx"
#include "string.hxx"
#include "internal.hxx"
#include "function.hxx"
#include "signalprocessingfunctions.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

    //fortran prototypes
    //extern void C2F(tscccf)(double *x, double *y, int *length, double *cxy, double *xymean, int *lag, int *error);
    //  extern void C2F(cmpse2)(int *iSect, int *iTotalSize, int *iMode, void *pXFunction,void *pYFunction, double *xa, double *xr, double *xi, double *zr, double *zi, int *error);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_corr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //check input parameters
    if (in.size() < 2 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "corr", 2, 5);
        return types::Function::Error;
    }

    //call format
    if (in[0]->isString())
    {


        types::String* pS = in[0]->getAs<types::String>();
        if (pS->getSize() == 1 && pS->get(0)[0] == L'f')
        {
            //[cov,mean]=corr('fft',xmacro,[ymacro],n,sect)
            int iErr                    = 0;
            int iSect                   = 0;
            int iTotalSize              = 0;
            int iSize                   = 0;
            int iMode                   = 0;
            types::InternalType* pXFunction = NULL;
            types::InternalType* pYFunction = NULL;
            double * xa = NULL;
            double * xi = NULL;
            double * xr = NULL;
            double * zr = NULL;
            double * zi = NULL;
            char *dx = NULL;
            char *dy = NULL;
            bool bOK = false;

            //check input parameters
            if (in.size() < 4 || in.size() > 5)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "corr", 4, 5);
                return types::Function::Error;
            }

            //get parameter sect
            int iPos = (int)(in.size() - 1);
            if (in[iPos]->isDouble() == false || in[iPos]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
                return types::Function::Error;
            }

            iSect = (int)in[iPos]->getAs<types::Double>()->get(0);

            //get parameter n
            iPos--;
            if (in[iPos]->isDouble() == false || in[iPos]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
                return types::Function::Error;
            }

            iTotalSize = (int)in[iPos]->getAs<types::Double>()->get(0);

            Signalprocessingfunctions* deFunctionsManager = new Signalprocessingfunctions(L"corr");
            Signalprocessing::addSignalprocessingfunctions(deFunctionsManager);
            //get xmacro
            if (in[1]->isCallable())
            {
                pXFunction = in[1]->getAs<types::Callable>();
                deFunctionsManager->setDgetx(in[1]->getAs<types::Callable>());
            }
            else if (in[1]->isString())
            {
                pXFunction = in[1]->getAs<types::String>();
                deFunctionsManager->setDgetx(in[1]->getAs<types::String>());
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
                return types::Function::Error;
            }

            iMode = 2;

            if (in.size() == 5)
            {
                //get ymacro
                if (in[2]->isCallable())
                {
                    pYFunction = in[2]->getAs<types::Callable>();
                    deFunctionsManager->setDgety(in[2]->getAs<types::Callable>());
                }
                else if (in[2]->isString())
                {
                    pYFunction = in[2]->getAs<types::String>();
                    deFunctionsManager->setDgety(in[2]->getAs<types::String>());
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 2);
                    return types::Function::Error;
                }

                iMode = 3;
            }
            xa = new double[iSect];
            xi = new double[iSect];
            xr = new double[iSect];
            zr = new double[iSect / 2 + 1];
            zi = new double[iSect / 2 + 1];
            C2F(cmpse2)(&iSect, &iTotalSize, &iMode, (void*) dgetx_f, (void*) dgety_f, xa, xr, xi, zr, zi, &iErr);
            if (iErr > 0)
            {
                Scierror(999, _("fft call : needs power of two!"));
                return types::Function::Error;
            }
            types::Double *pDblOut1 = new types::Double(1, iSect);
            pDblOut1->set(xa);
            out.push_back(pDblOut1);
            types::Double *pDblOut2 = new types::Double(1, iMode - 1);
            pDblOut2->set(xr);

            out.push_back(pDblOut2);
            return types::Function::OK;

        }
        else if (pS->getSize() == 1 && pS->get(0)[0] == L'u')
        {
            //update
        }
        else
        {
            //error
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "corr", 1, "'fft', 'update'");
            return types::Function::Error;
        }
    }
    else
    {
        //usual case [cov,mean]=corr(x,[y],nlags)
        int iErr                        = 0;
        int iCorrelation                = 0;
        types::Double* pDblX            = NULL;
        types::Double* pDblY            = NULL;
        types::Double* pDblCorrelation  = NULL;
        types::Double* pDblMean         = NULL;
        int iSize                       = 0;
        double pdblMean[2];


        //check input parameters
        if (in.size() < 2 || in.size() > 3)
        {
            Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "corr", 2, 3);
            return types::Function::Error;
        }

        //get last parameter nlags
        int iPos = (int)(in.size() - 1);
        if (in[iPos]->isDouble() == false || in[iPos]->getAs<types::Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
            return types::Function::Error;
        }

        iCorrelation = (int)in[iPos]->getAs<types::Double>()->get(0);
        pDblCorrelation = new types::Double(1, iCorrelation);

        if (in.size() == 3)
        {
            if (in[1]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 2);
                return types::Function::Error;
            }

            pDblY = in[1]->getAs<types::Double>();

            if (in[0]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 1);
                return types::Function::Error;
            }

            pDblX = in[0]->getAs<types::Double>();

            if (pDblX->getSize() != pDblY->getSize())
            {
                Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "corr");
                return types::Function::Error;
            }
        }
        else
        {
            if (in[0]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 1);
                return types::Function::Error;
            }

            pDblX = in[0]->getAs<types::Double>();
            pDblY = pDblX;
        }

        iSize = pDblX->getSize();

        C2F(tscccf)(pDblX->get(), pDblY->get(), &iSize, pDblCorrelation->get(), pdblMean, &iCorrelation, &iErr);
        if (iErr == -1)
        {
            Scierror(999, _("%s: Too many coefficients are required.\n"), "corr");
            return types::Function::Error;
        }

        out.push_back(pDblCorrelation);

        if (_iRetCount == 2)
        {
            if (in.size() == 3)
            {
                pDblMean = new types::Double(1, 2);
            }
            else
            {
                pDblMean = new types::Double(1, 1);
            }

            pDblMean->set(pdblMean);
            out.push_back(pDblMean);
        }
    }
    return types::Function::OK;
}

