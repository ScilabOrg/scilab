/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_external_objects.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_percent_EObj_e, "%_EObj_e"},
    {sci_percent_EObj_e, "%_EObj_6"},
    {sci_percent_EClass_e, "%_EClass_6"},
    {sci_percent_EObj_p, "%_EObj_p"},
    {sci_percent_EClass_e, "%_EClass_e"},
    {sci_percent_EObj_p, "%_EClass_p"},
    {sci_percent_foo_i_EObj, "%c_i__EObj"},
    {sci_percent_foo_i_EObj, "%s_i__EObj"},
    {sci_percent_foo_i_EObj, "%b_i__EObj"},
    {sci_percent_foo_i_EObj, "%i_i__EObj"},
    {sci_invoke_lu, "invoke_lu"},
    {sci_doubleExclam_invoke_, "!!_invoke_"},
    {sci_percent_EObj_disp, "%_EObj_disp"},
    {sci_pyImport, "pyImport"},
    {sci_pyAutounwrap, "pyAutounwrap"},
    {sci_pyGetMethods, "pyGetMethods"},
    {sci_pyGetFields, "pyGetFields"},
    {sci_pyDeff, "pyDeff"},
    {sci_pyGetRepr, "pyGetRepr"},
    {sci_pyExists, "pyExists"},
    {sci_pyGetField, "pyGetField"},
    {sci_pyRemove, "pyRemove"},
    {sci_pyUnwrap, "pyUnwrap"},
    {sci_pyWrap, "pyWrap"},
    {sci_pyAllowClassReloading, "pyAllowClassReloading"},
    {sci_pyGetInfos, "pyGetInfos"},
    {sci_pyGetPath, "pyGetPath"},
    {sci_pyAddToPath, "pyAddToPath"},
    {sci_pyTuple, "pyTuple"},
    {sci_pyDict, "pyDict"},
    {sci_pyList, "pyList"},
    {sci_pySet, "pySet"},
    {sci_pyXrange, "pyXrange"},
    {sci_pySlice, "pySlice"},
    {sci_pyGetName, "pyGetName"},
    {sci_pyEvalStr, "pyEvalStr"},
    {sci_pyWrapAsRef, "pyWrapAsRef"},
    {sci_pyUnwrapRem, "pyUnwrapRem"},
    {sci_pyByCopy, "pyByCopy"},
    {sci_pyUseNumpy, "pyUseNumpy"},
    {sci_pyUseScilabIndex, "pyUseScilabIndex"},
    {sci_pyGetEnvId, "pyGetEnvId"},
    {sci_pyKill, "pyKill"},
    {sci_pyTrace, "pyTrace"},
    {sci_pyAddVar, "pyAddVar"}
};

/*--------------------------------------------------------------------------*/
int gw_external_objects(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx *) MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char *)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}

/*--------------------------------------------------------------------------*/
