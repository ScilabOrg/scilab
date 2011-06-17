/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "gw_io.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "getenvc.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
/*--------------------------------------------------------------------------*/
int sci_getenv(char *fname, int* _piKey)
{
	SciErr sciErr;
	int ierr = 0;
	char *default_env_value = NULL;
	char *env_value = NULL;
	int length_env = 0;
	char *env_name = NULL;

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	int iType1	= 0;
	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	int m2 = 0, n2 = 0;
	int *piAddressVarTwo = NULL;
	int iType2	= 0;
	char *pStVarTwo = NULL;
	int lenStVarTwo = 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(Rhs == 2)
	{//second parameter
		sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 1;
		}

        if(isStringType(_piKey, piAddressVarTwo) == FALSE || isScalar(_piKey, piAddressVarTwo) == FALSE)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
			return 1;
		}
        
        if(getAllocatedSingleString(_piKey, piAddressVarTwo, &pStVarTwo))
        {
            return 1;
        }
	}

    //first parameter
	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}

    if(isStringType(_piKey, piAddressVarOne) == FALSE || isScalar(_piKey, piAddressVarOne) == FALSE)
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    if(getAllocatedSingleString(_piKey, piAddressVarOne, &pStVarOne))
    {
        return 1;
    }

	#ifdef _MSC_VER
	length_env = _MAX_ENV;
	#else
	length_env = bsiz;
	#endif

	default_env_value =  pStVarTwo;
	env_name = pStVarOne;

	env_value = (char*)MALLOC( (length_env + 1) *sizeof(char) );

	if(env_value == NULL)
	{
		if(default_env_value) {FREE(default_env_value); default_env_value = NULL;}
		if(env_name) {FREE(env_name); env_name = NULL;}

		Scierror(999,_("%s: No more memory.\n"), fname);
		return 1;
	}
	else
	{
		int m_out = 1, n_out = 1;
		int iflag = 0;

		getenvc(&ierr, env_name, env_value, &length_env, &iflag);

		if(ierr == 0)
		{
			sciErr = createMatrixOfString(_piKey, Rhs + 1, m_out, n_out, &env_value);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 1;
			}

			LhsVar(1) = Rhs + 1;
            PutLhsVar();	
		}
		else
		{
			if(default_env_value)
			{
				sciErr = createMatrixOfString(_piKey, Rhs + 1, m_out, n_out, &default_env_value);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 1;
				}

				LhsVar(1) = Rhs + 1;
                PutLhsVar();	
			}
			else
			{
				Scierror(999,_("%s: Undefined environment variable %s.\n"), fname, env_name);
			}
		}

		if(default_env_value) {FREE(default_env_value); default_env_value = NULL;}
		if(env_name) {FREE(env_name); env_name = NULL;}
		if(env_value) {FREE(env_value); env_value = NULL;}

	}
	return 0;
}
/*--------------------------------------------------------------------------*/
