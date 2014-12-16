/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "list.hxx"
#include "function.hxx"

#include "Controller.hxx"
#include "LoggerView.hxx"
#include "utilities.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace org_scilab_modules_scicos;

static const std::string funame = "scicos_log";
static LoggerView logger;

types::Function::ReturnValue sci_scicos_log(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (!(0 <= in.size() && in.size() <= 2))
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), funame.data(), 0, 2);
        return types::Function::Error;
    }
    if (!(0 <= _iRetCount && _iRetCount <= 1))
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), funame.data(), 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        enum LogLevel previous = logger.getLevel();

        if (_iRetCount == 1)
        {
            out.push_back(new types::String(LoggerView::toString(previous)));
        }

        return types::Function::OK;
    }

    types::InternalType* internLevel = in[0];
    if (internLevel->getType() != types::InternalType::ScilabString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }
    types::String* strLevel = internLevel->getAs<types::String>();
    if (strLevel->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), funame.data(), 1);
        return types::Function::Error;
    }

    enum LogLevel logLevel = LoggerView::indexOf(strLevel->get(0));
    if (logLevel < 0)
    {
        std::wstringstream buffer;
        for (int i = 0; i <= LOG_TRACE; i++)
        {
            buffer << LoggerView::toString(static_cast<enum LogLevel>(i));
            if (i != LOG_TRACE)
            {
                buffer << L", ";
            }
        }
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set  {%ls}.\n"), funame.data(), 1, buffer.str().data());
        return types::Function::Error;
    }

    if (in.size() == 1)
    {
        /*
         * Configure the logger value mode and return the previous log level
         */
        enum LogLevel previous = logger.getLevel();
        logger.setLevel(logLevel);

        if (_iRetCount == 1)
        {
            out.push_back(new types::String(LoggerView::toString(previous)));
        }
    }
    else
    {
        /*
         * Log a message for a specific level and return the log level
         */
        types::InternalType* internMsg = in[1];
        if (internMsg->getType() != types::InternalType::ScilabString)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), funame.data(), 2);
            return types::Function::Error;
        }
        types::String* strMsg = internMsg->getAs<types::String>();
        if (strMsg->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), funame.data(), 2);
            return types::Function::Error;
        }

        logger.log(logLevel, strMsg->get(0));

        if (_iRetCount == 1)
        {
            out.push_back(new types::String(LoggerView::toString(logLevel)));
        }
    }

    return types::Function::OK;
}
