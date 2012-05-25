/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_EXTERNAL_OBJECTS_H__
#define __GW_EXTERNAL_OBJECTS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_external_objects_scilab.h"
/*--------------------------------------------------------------------------*/
EXTERNAL_OBJECTS_SCILAB_IMPEXP int gw_external_objects(void);

/*--------------------------------------------------------------------------*/
int sci_percent_EObj_e(char * fname, unsigned long fname_len);
int sci_percent_EObj_p(char * fname, unsigned long fname_len);
int sci_percent_EClass_e(char * fname, unsigned long fname_len);
int sci_percent_foo_i_EObj(char * fname, unsigned long fname_len);
int sci_invoke_lu(char * fname, unsigned long fname_len);
int sci_doubleExclam_invoke_(char * fname, unsigned long fname_len);
int sci_percent_EObj_disp(char * fname, unsigned long fname_len);
int sci_pyImport(char * fname, unsigned long fname_len);
int sci_pyAutounwrap(char * fname, unsigned long fname_len);
int sci_pyGetMethods(char * fname, unsigned long fname_len);
int sci_pyGetFields(char * fname, unsigned long fname_len);
int sci_pyDeff(char * fname, unsigned long fname_len);
int sci_pyGetRepr(char * fname, unsigned long fname_len);
int sci_pyExists(char * fname, unsigned long fname_len);
int sci_pyGetField(char * fname, unsigned long fname_len);
int sci_pyRemove(char * fname, unsigned long fname_len);
int sci_pyUnwrap(char * fname, unsigned long fname_len);
int sci_pyWrap(char * fname, unsigned long fname_len);
int sci_pyAllowClassReloading(char * fname, unsigned long fname_len);
int sci_pyGetInfos(char * fname, unsigned long fname_len);
int sci_pyGetPath(char * fname, unsigned long fname_len);
int sci_pyAddToPath(char * fname, unsigned long fname_len);
int sci_pyTuple(char * fname, unsigned long fname_len);
int sci_pyDict(char * fname, unsigned long fname_len);
int sci_pyList(char * fname, unsigned long fname_len);
int sci_pySet(char * fname, unsigned long fname_len);
int sci_pyXrange(char * fname, unsigned long fname_len);
int sci_pySlice(char * fname, unsigned long fname_len);
int sci_pyGetName(char * fname, unsigned long fname_len);
int sci_pyEvalStr(char * fname, unsigned long fname_len);
int sci_pyWrapAsRef(char * fname, unsigned long fname_len);
int sci_pyUnwrapRem(char * fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_EXTERNAL_OBJECTS_H__ */
/*--------------------------------------------------------------------------*/
