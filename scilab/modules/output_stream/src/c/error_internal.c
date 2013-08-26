/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <string.h>
#include "error_internal.h"
#include "sciprint.h"
#include "msgstore.h"
#include "BOOL.h"
#include "stack-def.h"
#include "errmds.h"
#include "lasterror.h"
#include "MALLOC.h"
#include "strsubst.h"
/*--------------------------------------------------------------------------*/
extern int C2F(errloc) (int *n);    /* fortran */
extern int C2F(errmgr) ();      /* fortran */
extern int C2F(errcontext) ();  /* fortran */
extern int C2F(whatln) (int *lpt1, int *lpt2, int *lpt6, int *nct, int *idebut, int *ifin); /* fortran */

/*--------------------------------------------------------------------------*/
int error_internal(int *n, char *buffer)
{
    int len = 0;
    int num = 0;
    int lct1 = 0;
    int imode = 0;
    int imess = 0;
    int errtyp = 0;

    /* extract error modes out of errct variable */
    C2F(errmds) (&num, &imess, &imode);

    /* de-activate output control */
    lct1 = C2F(iop).lct[0];
    C2F(iop).lct[0] = 0;

    /* errors are recoverable */
    errtyp = 0;

    if (C2F(errgst).err1 == 0)
    {
        BOOL trace = !((num < 0 || num == *n) && imess != 0);

        /* locate the error in the current statement */
        if (trace)
        {
            C2F(errloc) (n);
        }
        else
        {
            /* get the number of the line where the error occurs */
            int nlc = 0;
            int l1 = 0;
            int ifin = 0;

            C2F(whatln) (C2F(iop).lpt, C2F(iop).lpt + 1, C2F(iop).lpt + 5, &nlc, &l1, &ifin);
            C2F(iop).lct[7] = C2F(iop).lct[7] - nlc;
            /* disable error display */
            C2F(iop).lct[0] = -1;
        }

        len = (int)strlen(buffer);

        /* free message table */
        clearInternalLastError();

        /* store error number */
        setInternalLastErrorValue(*n);

        /* store message */

        C2F(msgstore) (buffer, &len);

        /* display error */
        if (C2F(iop).lct[0] != -1)
        {
            char *msgTmp = strsub(buffer, "%%", "%");

            if (msgTmp)
            {
                sciprint("%s", msgTmp);
                FREE(msgTmp);
                msgTmp = NULL;
            }
        }

        C2F(iop).lct[0] = 0;
    }
    C2F(errcontext) ();
    /* handle the error */
    C2F(errmgr) (n, &errtyp);

    /* re-activate output control */
    C2F(iop).lct[0] = lct1;

    return 0;
}

/*--------------------------------------------------------------------------*/
