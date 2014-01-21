
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
#ifndef __GW_JVM_H__
#define __GW_JVM_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_jvm.h"
/*--------------------------------------------------------------------------*/
JVM_IMPEXP int gw_jvm(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
JVM_IMPEXP int sci_system_setproperty(char *fname, unsigned long fname_len);
JVM_IMPEXP int sci_system_getproperty(char *fname, unsigned long fname_len);
JVM_IMPEXP int sci_javaclasspath(char *fname, unsigned long fname_len);
JVM_IMPEXP int sci_javalibrarypath(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_JVM_H__ */


