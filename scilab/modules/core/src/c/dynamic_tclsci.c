/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "dynamic_tclsci.h"
#include "gw_dynamic_generic.h"
#include "configvariable_interface.h"
#include "with_module.h"
#include "sci_malloc.h"

/*--------------------------------------------------------------------------*/
/* tclsci module */
#define TCLSCI_MODULE_NAME "tclsci"
static DynLibHandle hTclsciLib = NULL;
static void* ptr_gw_tclsci = NULL;
static char* dynlibname_tclsci = NULL;
static char* gatewayname_tclsci = NULL;
/*--------------------------------------------------------------------------*/
#define SETENVTCL_NAME "setenvtcl"
typedef int (*PROC_SETENVTCL) (const char *, const char *);
static PROC_SETENVTCL ptr_setenvtcl = NULL;
/*--------------------------------------------------------------------------*/
#define TERMINATETCLTK_NAME "TerminateTclTk"
typedef BOOL (*PROC_TERMINATETCLTK) (void);
static PROC_TERMINATETCLTK ptr_TerminatTclTk = NULL;
/*--------------------------------------------------------------------------*/
void setTclLibHandle(DynLibHandle _handle)
{
    hTclsciLib = _handle;
}
/*--------------------------------------------------------------------------*/
int dynamic_setenvtcl(const char *string, const char *value)
{
    if (hTclsciLib)
    {
        if (ptr_setenvtcl == NULL)
        {
            ptr_setenvtcl = (PROC_SETENVTCL) GetDynLibFuncPtr(hTclsciLib,
                            SETENVTCL_NAME);
            if (ptr_setenvtcl == NULL)
            {
                return 0;
            }
        }
        return (ptr_setenvtcl)(string , value);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
