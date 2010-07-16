/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Palette.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "callxcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}

extern int
readVectorString( int* _piKey,int rhsPosition, char*** out, int* vectorLength, char* fname);
extern int
readSingleBoolean( int* _piKey,int rhsPosition, bool* out, const char* fname);

using namespace org_scilab_modules_xcos_palette;

int
sci_xcosPalCategoryAdd(char *fname, int* _piKey)
{
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    char** name = NULL;
    int nameLength = 0;
    bool visible = false;

    /* name setup */
    if (readVectorString(_piKey,1, &name, &nameLength, fname))
    {
        return 0;
    }

    /* visible setup */
    if (Rhs > 2)
    {
        if (readSingleBoolean(_piKey,2, &visible, fname))
        {
            return 0;
        }
    }
    else
    {
        visible = true;
    }

    /* Call the java implementation */
    try
    {
        Palette::addCategory(getScilabJavaVM(), name, nameLength,
                visible);
    }
    catch (GiwsException::JniCallMethodException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.what());
        return 0;
    }

    PutLhsVar();
    return 0;
}

