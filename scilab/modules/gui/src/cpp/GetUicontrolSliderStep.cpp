/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Get the SliderStep property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetUicontrolSliderStep.hxx"

void* GetUicontrolSliderStep(void* _pvCtx, int iObjUID)
{
    double *sliderStep = NULL;
    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_SLIDERSTEP__, jni_double_vector, (void**) &sliderStep);

    if (sliderStep != NULL)
    {
        status = sciReturnRowVector(sliderStep, 2);
        delete[] sliderStep;
        return status;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "SliderStep");
        return NULL;
    }
}
