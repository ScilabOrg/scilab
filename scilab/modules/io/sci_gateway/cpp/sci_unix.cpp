/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */


#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"

extern "C"
{
#include "systemc.h"
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_unix(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return sci_host(in, _iRetCount, out);
}
