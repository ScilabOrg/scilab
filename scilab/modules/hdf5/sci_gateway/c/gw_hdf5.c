/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_hdf5.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_export_to_hdf5, "export_to_hdf5"},
    {sci_import_from_hdf5, "import_from_hdf5"},
    {sci_listvar_in_hdf5, "listvar_in_hdf5"},
    {sci_is_hdf5_file, "is_hdf5_file"},
    {sci_h5dump, "h5dump"},
    {sci_h5open, "h5open"},
    {sci_h5close, "h5close"},
    {sci_h5read, "h5read"},
    {sci_h5ls, "h5ls"},
    {sci_h5print, "%H5Object_p"},
    {sci_percent_H5Object_e, "%H5Object_e"}
};
/*--------------------------------------------------------------------------*/
int gw_hdf5(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/

