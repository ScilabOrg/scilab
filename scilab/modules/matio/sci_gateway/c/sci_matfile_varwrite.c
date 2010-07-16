/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_matio.h"
#include "GetMatlabVariable.h"

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"

#define MATIO_ERROR if(_SciErr.iErr)	     \
    {					     \
      printError(&_SciErr, 0);		     \
      return 1;				     \
    }

enum matfile_errors {
  NO_MORE_VARIABLES = -1,
  UNKNOWN_VARIABLE_TYPE = 0
};

int sci_matfile_varwrite(char* fname, int* _piKey)
{
  int nbRow = 0, nbCol = 0;
  mat_t *matfile = NULL;
  matvar_t *matvar = NULL;
  int fileIndex = 0;
  char *varname = NULL;
  int flag = 0;
  int compressionFlag = 0;
  int var_type;
  int * fd_addr = NULL, * name_addr = NULL, * cp_flag_addr = NULL;
  double tmp_dbl;
  SciErr _SciErr;
  
  CheckRhs(4, 4);
  CheckLhs(1, 1);
  
  /* Input argument is the index of the file to write */
  
  _SciErr = getVarAddressFromPosition(_piKey, 1, &fd_addr); MATIO_ERROR;
  _SciErr = getVarType(_piKey, fd_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_matrix)
    {
      getScalarDouble(_piKey, fd_addr, &tmp_dbl);
      if (!isScalar(_piKey, fd_addr))
	{
	  Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
	  return 1;
	}
      fileIndex = (int)tmp_dbl;
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
      return 1;
    }
  
  /* Gets the corresponding matfile */
  matfile_manager(MATFILEMANAGER_GETFILE, &fileIndex, &matfile);
  
  /* Second argument is the variable name */
  
  _SciErr = getVarAddressFromPosition(_piKey, 2, &name_addr); MATIO_ERROR;
  _SciErr = getVarType(_piKey, name_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_strings)
    {
      getAllocatedSingleString(_piKey, name_addr, &varname);
      _SciErr = getVarDimension(_piKey, name_addr, &nbRow, &nbCol); MATIO_ERROR;
      if (nbCol != 1)
	{
	  Scierror(999, _("%s: Wrong size for second input argument: Single string expected.\n"), fname);
	  
	  freeAllocatedSingleString(varname);
	  
	  return 1;
	}
    }
  else
    {
      Scierror(999, _("%s: Wrong type for second input argument: Single string expected.\n"), fname);
      
      freeAllocatedSingleString(varname);
      
      return 1;
    }
  
  /* Third argument is the variable data */
  matvar = GetMatlabVariable(_piKey, 3, varname, matfile->version, NULL, -1);
  
  /* Fourth argument is the compression flag */
  
  _SciErr = getVarAddressFromPosition(_piKey, 4, &cp_flag_addr); MATIO_ERROR;
  _SciErr = getVarType(_piKey, cp_flag_addr, &var_type); MATIO_ERROR;
  
  if (var_type == sci_boolean)
    {
      getScalarBoolean(_piKey, cp_flag_addr, &compressionFlag);
      if (!isScalar(_piKey, cp_flag_addr))
	{
	  Scierror(999, _("%s: Wrong size for fourth input argument: Single boolean expected.\n"), fname);
	  
	  freeAllocatedSingleString(varname);
	  
	  return 1;
	}
    }
  else
    {
      Scierror(999, _("%s: Wrong type for fourth input argument: Single boolean expected.\n"), fname);
      
      freeAllocatedSingleString(varname);
      
      return 1;
    }
  
  flag = Mat_VarWrite(matfile, matvar, compressionFlag);
  
  /* Return execution flag */
  var_type = (flag==0);
  createScalarBoolean(_piKey, Rhs+1, var_type);
  
  LhsVar(1) = Rhs+1;
  
  PutLhsVar();
  
  freeAllocatedSingleString(varname);
  
  return 0;
}
