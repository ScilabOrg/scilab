/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"

/*--------------------------------------------------------------------------*/
int C2F(intisequalvar)(char * fname, int *job, long int fl); /* the gateway */
/*--------------------------------------------------------------------------*/
int sci_isequalbitwise(char *fname, unsigned long fname_len)
{
    //int job= 0;
    //C2F(intisequalvar)(fname, (job=0,&job),(unsigned long)strlen(fname) );
    return 0;
}
/*--------------------------------------------------------------------------*/
