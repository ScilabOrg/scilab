/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_io.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsave)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{

{NULL, ""}, //setenv
{sci_read,"read"},
{NULL, ""}, //getenv
{sci_getio,"getio"},
{NULL,""},
{NULL,""},
{sci_write,"write"},
{NULL,"rat"},
{NULL, ""}, //file
{NULL ,""}, //host
{NULL, ""}, //unix
{sci_readb,"readb"},
{sci_writb,"writb"},
{sci_getpid,"getpid"},
{sci_read4b,"read4b"},
{sci_write4b,"write4b"},
{sci_save,"save"},
{NULL, ""} //load
};
/*--------------------------------------------------------------------------*/
int gw_io(void)
{
    /* Recursion from a function */
    if ( isRecursionCallToFunction() )
    {
        switch ( getRecursionFunctionToCall() )
        {
        case RECURSION_CALL_SAVE:
            {
                //C2F(intsave)();
                return 0;
            }
            break;
        case RECURSION_CALL_LOAD:
            {
                //sci_load("load",(unsigned long)strlen("load"));
                return 0;
            }
            break;
        default:
            break;
        }
    }
    else
    {
        Rhs = Max(0, Rhs);
        callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
