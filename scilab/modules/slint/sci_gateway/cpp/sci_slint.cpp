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

#include <iostream>
#include <string>

#include "SLint.hxx"
#include "output/SLintScilabResult.hxx"
#include "output/SLintXmlResult.hxx"
#include "config/XMLConfig.hxx"

#include "string.hxx"
#include "slint_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace std;
using namespace types;
using namespace ast;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_slint(types::typed_list & in, int _iRetCount, types::typed_list & out)
{
    slint::SLintResult * results = nullptr;
    const unsigned size = in.size();
    if (size == 0 || size >= 4)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "slint", 1);
        return Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "slint" , 1);
        return Function::Error;
    }

    if (size == 2 && (!in[1]->isString() || in[1]->getAs<types::String>()->getSize() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "slint" , 2);
        return Function::Error;
    }

    if (size == 3 && (!in[2]->isString() || in[2]->getAs<types::String>()->getSize() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "slint" , 3);
        return Function::Error;
    }

    try
    {
	if (size != 1)
	{
	    results = new slint::SLintXmlResult(in[1]->getAs<types::String>()->get(0));
	}
	else
	{
	    results = new slint::SLintScilabResult();
	}
        slint::SLint slint(*results);
        slint.setFiles(in[0]->getAs<types::String>());
        slint::SLintOptions options;

	if (size != 3)
	{
	    slint::XMLConfig::getOptions(L"SCI/modules/slint/etc/slint.xml", options);
	}
	else
	{
	    slint::XMLConfig::getOptions(in[2]->getAs<types::String>()->get(0), options);
	}
	
        slint.setOptions(options);
        slint.check();
        results->finalize();
    }
    catch (slint::PCREException & e)
    {
	delete results;
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return Function::Error;
    }
    catch (slint::FileException & e)
    {
	delete results;
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return Function::Error;
    }
    catch (slint::SLintXMLException & e)
    {
	delete results;
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return Function::Error;
    }
    
    delete results;

    return Function::OK;
}
