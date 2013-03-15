
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_SLICOT_H__
#define __GW_SLICOT_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_cacsd.h"
/*--------------------------------------------------------------------------*/
CACSD_IMPEXP int gw_slicot(void);
/*--------------------------------------------------------------------------*/
int sci_sorder(char *fname, unsigned long fname_len);
int sci_sident(char *fname, unsigned long fname_len);
int sci_findbd(char *fname, unsigned long fname_len);
int sci_ricc2(char *fname, unsigned long fname_len);
int sci_dhinf(char *fname, unsigned long fname_len);
int sci_linmeq(char *fname, unsigned long fname_len);
int sci_mucomp(char *fname, unsigned long fname_len);
int sci_hinf(char *fname, unsigned long fname_len);
int sci_rankqr(char *fname, unsigned long fname_len);
int intab01od(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SLICOT_H__ */
/*--------------------------------------------------------------------------*/

