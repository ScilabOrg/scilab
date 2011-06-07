/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "localization.h"
#include "Scierror.h"
#include "dynamic_link.h"
/*--------------------------------------------------------------------------*/
int sci_ulink(char *fname,int *_piKey)
{
    SciErr sciErr;
    int* piAddress;
    int iType;

    CheckRhs(0,1);
    CheckLhs(1,1);

    /* environment variable used (linux) to detect a PROFILING tools */
    /* in this case, we do not ulink external dynamic libraries */
    if (getenv("PROFILE_SCILAB_DYNAMIC_LINK") == NULL)
    {
        if (Rhs == 0)
        {
            unlinkallsharedlib();
        }
        else
        {
            sciErr = getVarAddressFromPosition(_piKey, 1, &piAddress);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }
            sciErr = getVarType(_piKey, piAddress, &iType);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            if ( iType == sci_matrix )
            {
                int l1 = 0,n1 = 0,m1 = 0;
                double *params = NULL;
                int i = 0;
                sciErr = getMatrixOfDouble(_piKey, piAddress, &m1, &n1, &params);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
                for(i = 0; i < m1*n1; i++)
                {
                    int ilib = (int) params[i];
                    unlinksharedlib(&ilib);
                }
            }
            else
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: Integer expected.\n"), fname,1);
                return 0;
            }
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;

}
/*--------------------------------------------------------------------------*/
