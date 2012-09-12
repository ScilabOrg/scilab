/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "time_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "arrayof.hxx"
#include "timer.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}


types::Function::ReturnValue sci_sleepp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //    Timer toto;
    //    toto.start();

    types::String* pIn = NULL;

    int iSec = 0;

    if (in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"abs", 1);
        return types::Function::Error;
    }

    //    if(in[0]->isDouble() == false)
    //    {
    //        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"abs", 1);
    //        return types::Function::Error;
    //    }

    pIn = in[0]->getAs<types::String>();

    //    if(pIn->getSize() != 1)
    //    {
    //        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"abs", 1);
    //        return types::Function::Error;
    //    }

    iSec = 1;//(int)pIn->get(0);

    if (iSec <= 0)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"abs", 1);
        return types::Function::Error;
    }

#ifdef _MSC_VER
    {
        int ms = (iSec); /** time is specified in milliseconds in scilab**/
        if (ms > 0) Sleep(ms); /* Number of milliseconds to sleep. */
    }
#else
    {
        unsigned useconds;
        useconds = (unsigned) iSec;
        if (useconds != 0)
#ifdef HAVE_USLEEP
        {
            usleep(useconds * 1000);
        }
#else
#ifdef HAVE_SLEEP
        {
            sleep(useconds * 1000);
        }
#endif
#endif
    }
#endif

    out.push_back(pIn);

    //    toto.check("sleepp");
    return types::Function::OK;
}
