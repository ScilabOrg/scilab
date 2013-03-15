
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
<<<<<<< HEAD
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
=======
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
>>>>>>> 630de55... rewrite mex fortran gateway of cacsd with API Scilab
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_CACSD0__
#define __GW_CACSD0__
/*--------------------------------------------------------------------------*/
#include "dynlib_cacsd.h"
/*--------------------------------------------------------------------------*/
CACSD_IMPEXP int gw_cacsd0(void);
/*--------------------------------------------------------------------------*/
int sci_gschur(char *fname, unsigned long fname_len);
int sci_gspec(char *fname, unsigned long fname_len);
int sci_ereduc(char *fname, unsigned long fname_len);
int sci_fstair(char *fname, unsigned long fname_len);
int sci_sorder(char *fname, unsigned long fname_len);
int sci_findbd(char *fname, unsigned long fname_len);
int sci_sident(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#define CSIZE 64000

#define CHECK_PARAM(CTX, POS) \
    if(checkParam(CTX, POS, fname)) return 0;

int checkParam(void* _pvCtx, int _iPos, char* fname);
char getIntegerValue(void* _pvCtx, int _iPos);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CACSD0__ */
/*--------------------------------------------------------------------------*/
