/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_EXTERNAL_OBJECTS_H__
#define __GW_EXTERNAL_OBJECTS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_external_objects_java_scilab.h"
/*--------------------------------------------------------------------------*/
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int gw_external_objects_java(void);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jimport(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetclassname(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jwrap(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetmethods(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetfields(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetfield(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jcompile(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jnewInstance(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jremove(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jexists(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_junwrap(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_junwraprem(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jenableTrace(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jdisableTrace(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jdeff(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jarray(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jcast(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jallowClassReloading(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jwrapinfloat(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jsetfield(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jautoUnwrap(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jautoTranspose(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jinvoke(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jconvMatrixMethod(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetinfo(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jinvoke_db(char *fname, void* pvApiCtx);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jcreatejar(char *fname, void* pvApiCtx);

/*--------------------------------------------------------------------------*/
#endif /* __GW_EXTERNAL_OBJECTS_H__ */
/*--------------------------------------------------------------------------*/
