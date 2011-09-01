// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

#include <stdlib.h>
#include <api_scilab.h>
#include <Scierror.h>
#include <stack-c.h>
#include <stack-def.h>
#include <MALLOC.h>
#include <localization.h>
// =============================================================================
int sci_namedvariables(char *fname)
{
  #define PREFIX_VARNAME "VARIABLE_NAME_"
  #define NB_VARS 1000
  SciErr sciErr;
  char varnames[NB_VARS][nlgh];
  int i = 0;

  
  for (i = 0; i < NB_VARS; i++)
  {
    sprintf(varnames[i], "%s%d", PREFIX_VARNAME, i);
  }
  
  for (i = 0; i < NB_VARS; i++)
  {
    sciErr = createNamedMatrixOfString(pvApiCtx, varnames[i], 1, 1, &fname);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
  }
  return 0;
}
// =============================================================================
