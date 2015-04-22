/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>

#include "CoverModule.hxx"

#include "functions_gw.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "context.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace std;
using namespace types;
using namespace ast;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_coverage(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        coverage::CoverModule * cover = coverage::CoverModule::getInstance();
        if (cover)
        {
            cover->print();
            return Function::OK;
        }
    }

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "coverage" , 2);
        return Function::Error;
    }

    if (!in[0]->isString() || in[0]->getAs<types::String>()->getCols() != 2)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A two-columns string matrix expected.\n"), "coverage" , 1);
        return Function::Error;
    }

    types::String * strs = in[0]->getAs<types::String>();
    std::vector<std::pair<std::wstring, std::wstring>> paths_mods;
    const unsigned int rows = strs->getRows();
    paths_mods.reserve(rows);

    for (unsigned int i = 0; i < rows; ++i)
    {
        paths_mods.emplace_back(strs->get(i, 0), strs->get(i, 1));
    }

    coverage::CoverModule::createInstance(paths_mods);

    return Function::OK;
}
