/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "dynlib_tclsci.h"
#include "tclsci_gw.hxx"

extern "C"
{
#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"
    /*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
    int XTKsocket = 0;
#endif
}

/*--------------------------------------------------------------------------*/
TCLSCI_IMPEXP int TclsciModule::Load()
{
    return 0;
}
/*--------------------------------------------------------------------------*/

extern "C"
{
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP int CloseTCLsci(void)
    {
        return 0;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath)
    {
        return TRUE;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP BOOL Set_TK_LIBRARY_PATH(char *DefaultPath)
    {
        return TRUE;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP int gw_tclsci(void)
    {
        Scierror(999, _("Scilab TCL/TK module not installed.\n"));
        return 0;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP void initTCLTK(void)
    {
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP int sciDestroyGed( int figureId )
    {
        return 1;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP int isGedOpenedOn( int figureId )
    {
        return FALSE;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP int setenvtcl(char *string, char *value)
    {
        return TRUE;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP BOOL InitializeTclTk(void)
    {
        return FALSE;
    }
    /*--------------------------------------------------------------------------*/
    TCLSCI_IMPEXP BOOL TerminateTclTk(void)
    {
        return TRUE;
    }
    /*--------------------------------------------------------------------------*/
}
