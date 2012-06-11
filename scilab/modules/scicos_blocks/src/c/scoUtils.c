/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "machine.h"
#include "MALLOC.h"
#include "BOOL.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "scoUtils.h"
#include "scicos_block4.h"
#include "scicos.h"

#include <string.h>

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

// #include <stdio.h>
// #define LOG printf
#define LOG

char *findChildWithKindAt(char *parent, const char *type, const int position)
{
    char *child = NULL;

    int childrenCount;
    char **children;

    int i;
    char *childType;
    int typeCount;

    int *pChildrenCount = &childrenCount;
    getGraphicObjectProperty(parent, __GO_CHILDREN_COUNT__, jni_int, (void **)&pChildrenCount);
    getGraphicObjectProperty(parent, __GO_CHILDREN__, jni_string_vector, (void **)&children);

    for (typeCount = 0, i = childrenCount - 1; i >= 0; i--)
    {
        getGraphicObjectProperty(children[i], __GO_TYPE__, jni_string, (void **)&childType);

        if (strcmp(childType, type) == 0)
        {
            typeCount++;
        }

        releaseGraphicObjectProperty(__GO_TYPE__, jni_string, childType);

        if (typeCount == (position + 1))
        {
            child = strdup(children[i]);
            LOG("%s: found %s at %d : %s\n", "findChildWithKindAt", type, position, child);
            break;
        }
    }

    releaseGraphicObjectProperty(__GO_CHILDREN__, jni_string_vector, children);
    releaseGraphicObjectProperty(__GO_CHILDREN_COUNT__, jni_int, childrenCount);

    return child;
};

BOOL setLabel(char *pAxeUID, const char *_pstName, const char *label)
{
    char *pLabelUID;
    char *rwLabel = strdup(label);
    int dimensions[2];

    BOOL result = TRUE;

    getGraphicObjectProperty(pAxeUID, _pstName, jni_string, (void **)&pLabelUID);

    if (pLabelUID != NULL && strcmp(pLabelUID, "") != 0)
    {
        dimensions[0] = 1;
        dimensions[1] = 1;

        result = setGraphicObjectProperty(pLabelUID, __GO_TEXT_ARRAY_DIMENSIONS__, &dimensions, jni_int_vector, 2);
    }

    if (pLabelUID != NULL && result == TRUE)
    {
        result = setGraphicObjectProperty(pLabelUID, __GO_TEXT_STRINGS__, &rwLabel, jni_string_vector, 1);
    }

    FREE(rwLabel);
    releaseGraphicObjectProperty(_pstName, jni_string, pLabelUID);

    return result && pLabelUID != NULL;
}
