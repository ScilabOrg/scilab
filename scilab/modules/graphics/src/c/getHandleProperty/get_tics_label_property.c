/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_tics_labels_property.c                                       */
/* desc : function to retrieve in Scilab the tics_labels field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_tics_labels_property(char *pobjUID)
{
    char** labels;
    int iNumberTicksLabels = 0;
    int* piNumberTicksLabels = &iNumberTicksLabels;

#if 0
    if ( sciGetEntityType (pobj) != SCI_AXES )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_labels");
        return -1;
    }
#endif

    /*
     * Used if no user ticks have been specified. Computes the default ticks labels, according
     * to the axis interval. Deactivated for now and to be implemented using the MVC framework.
     */
#if 0
    if ( pAXES_FEATURE(pobj)->str == NULL )
    {
        int status = -1;

        /* tics_labels is allocated here */
        StringMatrix * tics_labels = computeDefaultTicsLabels( pobj ) ; /* actually it is vector */

        if ( tics_labels == NULL )
        {
            Scierror(999, _("%s: No more memory.\n"), "get_tics_labels_property");
            return -1;
        }

        status = sciReturnRowStringVector( getStrMatData( tics_labels ), pAXES_FEATURE (pobj)->nb_tics_labels );

        deleteMatrix( tics_labels );

        return status;
    }
    else
#endif
    {
        getGraphicObjectProperty(pobjUID, __GO_NUMBER_TICKS_LABELS__, jni_int, (void **) &piNumberTicksLabels);

        getGraphicObjectProperty(pobjUID, __GO_TICKS_LABELS__, jni_string_vector, (void **) &labels);

        if (piNumberTicksLabels == NULL || labels == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_labels");
            return -1;
        }

        /* User-specified ticks labels */
        return sciReturnRowStringVector( labels, iNumberTicksLabels);
    }
}
/*------------------------------------------------------------------------*/
