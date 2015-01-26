/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "double.hxx"
#include "cell.hxx"
#include "function.hxx"
#include "funcmanager.hxx"

extern "C"
{
#include "Scierror.h"
}

using namespace types;

int prodDims(Double* _pVar);

Function::ReturnValue sci_makecell(typed_list &in, int _piRetCount, typed_list &out)
{

    if (in.size() < 2)
    {
        return Function::Error;
    }

    //manage dims

    //check data type
    if (in[0]->isDouble() == false)
    {
        //FIXME: call overload function %_makecell to manage ohter input types
    }

    Double* pD = in[0]->getAs<Double>();

    //check vector format
    if (pD->getRows() != 1 && pD->getCols() != 1 || pD->getSize() == 1)
    {
        return Function::Error;
    }

    //check input parameters count
    if (prodDims(pD) != (in.size() - 1))
    {
        Scierror(999, _("%s: Wrong input arguments: Dimensions given as first argument do not match specified cell contents.\n"), "makecell");
        return Function::Error;
    }

    Cell *pC  = NULL;
    int iDims = pD->getSize();
    int* piDims = new int[iDims];
    int isizeHype = 1;
    for (int i = 0; i < iDims; i++)
    {
        piDims[i] = pD->get(i);
        isizeHype = isizeHype * piDims[i];
    }

    int isize = pD->get(0) * pD->get(1);
    isizeHype = isizeHype / isize;

    pC = new Cell(iDims, piDims);

    int iPost = 0;
    int iReadData = 0;
    for (int k = 0; k < isizeHype; k++)
    {
        for (int j = 0; j < pD->get(0); j++)
        {
            for (int i = 0; i < pD->get(1); i++)
            {
                iPost = i * static_cast<int>(pD->get(0)) + j + k * isize;
                iReadData = i + j * static_cast<int>(pD->get(1)) + 1 + k * isize;
                pC->set(iPost, in[iReadData]);
            }
        }
    }


    delete piDims;


    out.push_back(pC);
    return Function::OK;
}

int prodDims(Double* _pVar)
{
    int iProd = 1;
    for (int i = 0 ; i < _pVar->getSize() ; i++)
    {
        iProd *= static_cast<int>(_pVar->getReal()[i]);
    }
    return iProd;
}
