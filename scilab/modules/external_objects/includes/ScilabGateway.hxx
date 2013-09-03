/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SCILABGATEWAY_HXX__
#define __SCILABGATEWAY_HXX__

#include <sstream>

#include "ScilabObjects.hxx"
#include "ScilabAbstractInvoker.hxx"
#include "ScilabAbstractMemoryAllocator.hxx"
#include "ScilabAbstractOptionsSetter.hxx"
#include "ScilabStream.hxx"
#include "dynlib_external_objects_scilab.h"

extern "C" {
#include "MALLOC.h"
}

namespace org_modules_external_objects
{

class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabGateway
{

public:

    static int getEnvId(char * fname, const int envId, void * pvApiCtx);

    static int getsetOptions(char * fname, const int envId, ScilabAbstractOptionsSetter & setter, void * pvApiCtx);

    static int addToClasspath(char * fname, const int envId, void * pvApiCtx);

    static int getClasspath(char * fname, const int envId, void * pvApiCtx);

    static int addNamedVariable(char * fname, const int envId, void * pvApiCtx);

    static int getNamedVariable(char * fname, const int envId, void * pvApiCtx);

    static int evalString(char * fname, const int envId, void * pvApiCtx);

    static int array(char * fname, const int envId, void * pvApiCtx);

    static int cast(char * fname, const int envId, void * pvApiCtx);

    static int compile(char * fname, const int envId, void * pvApiCtx);

    static int convMatrixMethod(char * fname, const int envId, void * pvApiCtx);

    static int deff(char * fname, const int envId, void * pvApiCtx);

    static int display(char * fname, const int envId, void * pvApiCtx);

    static int doubleExclam_invoke(char * fname, const int envId, void * pvApiCtx);

    static int classExtract(char * fname, const int envId, void * pvApiCtx);

    static int objectExtract(char * fname, const int envId, void * pvApiCtx);

    static int objectInsert(char * fname, const int envId, void * pvApiCtx);

    static int objectPrint(char * fname, const int envId, void * pvApiCtx);

    static int exists(char * fname, const int envId, void * pvApiCtx);

    static int getClassName(char * fname, const int envId, void * pvApiCtx);

    static int getEnvironmentName(char * fname, const int envId, void * pvApiCtx);

    static int getEnvironmentInfos(char * fname, const int envId, void * pvApiCtx);

    static int getField(char * fname, const int envId, void * pvApiCtx);

    static int getFields(char * fname, const int envId, void * pvApiCtx);

    static int getMethods(char * fname, const int envId, void * pvApiCtx);

    static int getRepresentation(char * fname, const int envId, void * pvApiCtx);

    static int import(char * fname, const int envId, void * pvApiCtx);

    static int operation(char * fname, const int envId, const OperatorsType type, void * pvApiCtx);

    static int invoke(char * fname, const int envId, void * pvApiCtx);

    static int invoke_asref(char * fname, const int envId, void * pvApiCtx);

    static int invoke(char * fname, const int envId, ScilabAbstractInvoker & invoker, void * pvApiCtx);

    static int invoke_lu(char * fname, const int envId, void * pvApiCtx);

    static int trace(char * fname, const int envId, void * pvApiCtx);

    static int newInstance(char * fname, const int envId, void * pvApiCtx);

    static int remove(char * fname, const int envId, void * pvApiCtx);

    static int setField(char * fname, const int envId, void * pvApiCtx);

    static int unwrap(char * fname, const int envId, void * pvApiCtx);

    static int unwrapremove(char * fname, const int envId, void * pvApiCtx);

    //static int unwrapAndRemove(char * fname, const int envId, void * pvApiCtx);

    static int wrap(char * fname, const int envId, void * pvApiCtx);

    static int wrapAsRef(char * fname, const int envId, void * pvApiCtx);

    static int wrapInFloat(char * fname, const int envId, void * pvApiCtx);
};
}

#endif // __SCILABGATEWAY_HXX__
