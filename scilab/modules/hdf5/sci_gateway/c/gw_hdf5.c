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
#define HDF5_TAB_SIZE 2
static gw_generic_table Tab[HDF5_TAB_SIZE]={
	{NULL, ""}, //export_to_hdf5
	{NULL, ""} //import_from_hdf5
	};
/*--------------------------------------------------------------------------*/ 
int gw_hdf5(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/ 

