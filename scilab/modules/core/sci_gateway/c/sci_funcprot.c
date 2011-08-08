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
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "stack-c.h"
#include "funcprot.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_funcprot)(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	CheckLhs(1,1);
	CheckRhs(0,1);

	if (Rhs == 0)
	{
		int m_out = 0, n_out = 0;
		double dOut = (double) getfuncprot();

		m_out = 1;  n_out = 1;
		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, &dOut);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1; 
		PutLhsVar();
	}
	else if (Rhs == 1)
	{
		int ilevel = 0;
		int m1 = 0, n1 = 0;
		int iType1						= 0;
		int *piAddressVarOne	= NULL;
		double *pdVarOne			= NULL;

		/* get Address of inputs */
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		/* check input type */
		if ( iType1 != sci_matrix )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname,1);
			return 0;
		}

		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne,&m1,&n1,&pdVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//if ((m1 > 0) && (n1>0))
		//if ( (m1 != n1) && (n1 != 1) ) 
		if( n1 != 1 || m1 != 1)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
			return 0;
		}


		ilevel = (int) *pdVarOne;

		if (*pdVarOne != (double)ilevel)
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: 1,2 or 3 expected.\n"),fname,1);
			return 0;
		}

		if ( !setfuncprot(ilevel) )
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: 1,2 or 3 expected.\n"),fname,1);
		}
		else
		{
			LhsVar(1) = 0; 
			PutLhsVar();
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
