/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_user_data_property.c                                         */
/* desc : function to retrieve in Scilab the user_data field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"
#include "api_scilab.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"


/*------------------------------------------------------------------------*/
void* get_user_data_property(void* _pvCtx, char* pobjUID)
{
    int iUserDataSize = 0;
    int *piUserDataSize = &iUserDataSize;
    int *piUserData = NULL;

    void* status = NULL;

    getGraphicObjectProperty(pobjUID, __GO_USER_DATA_SIZE__, jni_int, (void **)&piUserDataSize);

    getGraphicObjectProperty(pobjUID, __GO_USER_DATA__, jni_int_vector, (void **)&piUserData);

    if ((piUserData == NULL) || (piUserDataSize == 0))
    {
        int iSize = sizeof(void*) / sizeof(int);
        status = sciReturnEmptyMatrix();
        //force user_data to have something and not create each time a new variable
        setGraphicObjectProperty(pobjUID, __GO_USER_DATA__, &status, jni_int_vector, iSize);
        increaseValRef(_pvCtx, (int*)status);
    }
    else
    {
        status = sciReturnUserData(piUserData, iUserDataSize);
        free(piUserData);
    }

    return status;
}

/*------------------------------------------------------------------------*/
