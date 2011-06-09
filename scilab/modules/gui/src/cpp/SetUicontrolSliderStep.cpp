/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the slider step property of an uicontrol object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolSliderStep.hxx"

int SetUicontrolSliderStep(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A 1 x %d real row vector expected.\n")), "SliderStep", 2);
        return SET_PROPERTY_ERROR;
    }

    if((nbRow > 1) || (nbCol != 2))
    {
        /* Wrong value size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A 1 x %d real row vector expected.\n")), "SliderStep", 2);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObj->UID, const_cast<char*>(__GO_UI_SLIDERSTEP__), getDoubleMatrixFromStack(stackPointer), jni_double_vector, 2);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "SliderStep");
        return SET_PROPERTY_ERROR;
    }
}

