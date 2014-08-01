/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
 * 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "special_functions_gw.hxx"	
#include "function.hxx"	
#include "api_scilab.hxx"	
#include "execvisitor.hxx"
#include "double.hxx"

extern "C"
{
    #include <string.h>
    #include "basic_functions.h"
    #include "gw_special_functions.h"
    #include "machine.h"
    #include "Scierror.h"
    #include "localization.h"


/*--------------------------------------------------------------------------*/

    extern double C2F(psi)();
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_dlgamma(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	if (in.size() != 1)
    {	
        Scierror(77, _("%s: Wrong number of input argument: %d expected.\n"), "sqrt", 1);
        return types::Function::Error;	
    }

    if(in[0]->isList()||in[0]->isTList()||in[0]->isMList())
    {
    	 std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_dlgamma";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }

    if (!in[0]->isDouble())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A matrix expected.\n"), "dlgamma", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    types::Double* pDblIn = in[0]->getAs<types::Double>();
    
    if(pDblIn->isComplex() || (pDblIn->getImg()!=NULL))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "dlgamma", 1);
        return types::Function::Error;
    }

    if(pDblIn->getDims()>2)
    {
        std::wstring wstFuncName = L"%hm_dlgamma";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
    }
    types::Double* pDblOut = new types::Double(pDblIn->getDims(),pDblIn->getDimsArray());

    double* pDblValIn = pDblIn->getReal();
    double* pDblValOut =  pDblOut->getReal();
    /***** perform operation *****/
    

    for (int i = 0; i < pDblIn->getSize(); i++)
    {
        pDblValOut[i] = C2F(psi)(pDblValIn + i);
    }

    /***** return data *****/
    out.push_back(pDblOut);

    return types::Function::OK;
/*    SciErr sciErr;
    double* lX   = NULL;
    int* piAddrX = NULL;

    int iType1 = 0;
    int MX = 0, NX = 0, i = 0;

    nbInputArgument(pvApiCtx) = Max(0, nbInputArgument(pvApiCtx));

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddrX, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if ((iType1 == sci_list) || (iType1 == sci_tlist) || (iType1 == sci_mlist))
    {
        OverLoad(1);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddrX))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrX, &MX, &NX, &lX);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A matrix expected.\n"), fname, 1);
    }

    for (i = 0; i < MX * NX; i++)
    {
        lX[i] = C2F(psi)(lX + i);
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    returnArguments(pvApiCtx);
    return 0;*/
}
/*--------------------------------------------------------------------------*/
