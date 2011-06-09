/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009-2010 - Vincent COUVERT <vincent.couvert@scilab.org>
 * Copyright (C) DIGITEO - 2010-2011 - Clément DAVID <clement.david@scilab.org>
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "Scierror.h"
#include "getScilabJavaVM.h"
#include "scilabmode.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int sci_endXcosSession(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 0);
    CheckLhs(0, 0);

    // only if xcos was already opened and with supported mode
    if ((getScilabMode() != SCILAB_NWNI) && xcosStarted())
    {
        try
        {
            Xcos::endXcosSession(getScilabJavaVM());
        } catch (GiwsException::JniCallMethodException exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
            return 0;
        } catch (GiwsException::JniException exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.what());
            return 0;
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
