/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_gui.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "createGraphicObject.h"
#include "BuildObjects.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "FigureList.h"
#include "HandleManagement.h"
#include "SetHashTable.h"
#include "stricmp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "sciprint.h"
#include "addColor.h"

/*--------------------------------------------------------------------------*/
int setDefaultProperties(int _iFig, BOOL bDefaultAxes);
/*--------------------------------------------------------------------------*/
int sci_figure(char * fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iFig = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    int iId = 0;
    int iPos = 0;
    int i = 0;
    int iNewId = -1;
    int iAxes = 0;
    int iPropertyOffset = 0;
    BOOL bDoCreation = TRUE;
    BOOL bVisible = TRUE; // Create a visible figure by default
    BOOL bDockable = TRUE; // Create a dockable figure by default
    BOOL bDefaultAxes = TRUE; // Create an Axes by default
    int iMenubarType = 1; // Create a 'figure' menubar by default
    int iToolbarType = 1; // Create a 'figure' toolbar by default
    double dblId = 0;
    BOOL status = FALSE;

    //figure(num) -> scf(num)
    //figure() -> scf()

    //figure(x, "...", ...)

    // figure()
    if (iRhs == 0) // Auto ID
    {
        iFig = createNewFigureWithAxes();
        iAxes = setDefaultProperties(iFig, TRUE);
        createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (iRhs == 1)
    {
        //figure(x);
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarMatrixType(pvApiCtx, piAddr) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddr, &dblId))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iId = (int)(dblId + 0.5); //avoid 1.999 -> 1

        //get current fig from id
        iFig = getFigureFromIndex(iId);
        if (iFig == 0) // Figure does not exists, create a new one
        {
            iFig = createNewFigureWithAxes();
            setGraphicObjectProperty(iFig, __GO_ID__, &iId, jni_int,  1);
            iAxes = setDefaultProperties(iFig, TRUE);
        }

        createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    // Prepare property analysis
    if (iRhs % 2 == 0)
    {
        //get highest value of winsid to create the new windows @ + 1
        iNewId = getValidDefaultFigureId();
        iPos = 0;
    }
    else
    {
        iPos = 1;
        //figure(x, ...);
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarMatrixType(pvApiCtx, piAddr) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddr, &dblId))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iNewId = (int)(dblId + 0.5); //avoid 1.999 -> 1
        //get current fig from id
        iFig = getFigureFromIndex(iId);
        if (iFig != 0) // Figure already exists
        {
            bDoCreation = TRUE;
        }
    }

    if (bDoCreation)
    {
        int* piAddrProp = NULL;
        char* pstProName = NULL;
        int* piAddrData = NULL;
        char* pstPropVal = NULL;
        for (i = iPos + 1 ; i <= iRhs ; i += 2)
        {
            //get property name
            sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrProp);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddrProp, &pstProName))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i);
                return 1;
            }
            if (stricmp(pstProName, "dockable") != 0
                    && stricmp(pstProName, "toolbar") != 0
                    && stricmp(pstProName, "menubar") != 0
                    && stricmp(pstProName, "default_axes") != 0
                    && stricmp(pstProName, "visible") != 0 )
            {
                freeAllocatedSingleString(pstProName);
                continue;
            }
            //get address of value on stack
            sciErr = getVarAddressFromPosition(pvApiCtx, i + 1, &piAddrData);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
                return 1;
            }
            if (getAllocatedSingleString(pvApiCtx, piAddrData, (char**)&pstPropVal))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 3);
                return 1;
            }
            //check property value to compatibility
            if (stricmp(pstProName, "dockable") == 0)
            {
                if (stricmp(pstPropVal, "on") == 0)
                {
                    bDockable = TRUE;
                }
                else if (stricmp(pstPropVal, "off") == 0)
                {
                    bDockable = FALSE;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "dockable", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstPropVal);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "toolbar") == 0)
            {
                if (stricmp(pstPropVal, "none") == 0)
                {
                    iToolbarType = 0;
                }
                else if (stricmp(pstPropVal, "figure") == 0)
                {
                    iToolbarType = 1;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "toolbar", "none", "figure");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstPropVal);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "menubar") == 0)
            {
                if (stricmp(pstPropVal, "none") == 0)
                {
                    iMenubarType = 0;
                }
                else if (stricmp(pstPropVal, "figure") == 0)
                {
                    iMenubarType = 1;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "menubar", "none", "figure");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstPropVal);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "default_axes") == 0)
            {
                if (stricmp(pstPropVal, "on") == 0)
                {
                    bDefaultAxes = TRUE;
                }
                else if (stricmp(pstPropVal, "off") == 0)
                {
                    bDefaultAxes = FALSE;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "default_axes", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstPropVal);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "visible") == 0)
            {
                if (stricmp(pstPropVal, "on") == 0)
                {
                    bVisible = TRUE;
                }
                else if (stricmp(pstPropVal, "off") == 0)
                {
                    bVisible = FALSE;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "visible", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstPropVal);
                    return 1;
                }
            }
            freeAllocatedSingleString(pstPropVal);
        }
        iFig = createFigure(bDockable, iMenubarType, iToolbarType, bDefaultAxes, bVisible);
        setGraphicObjectProperty(iFig, __GO_ID__, &iNewId, jni_int, 1);
        setDefaultProperties(iFig, bDefaultAxes);
    }

    //set(iFig, iPos, iPos + 1)
    for (i = iPos + 1 ; i <= iRhs ; i += 2)
    {
        int isMatrixOfString = 0;
        int* piAddrProp = NULL;
        char* pstProName = NULL;
        int* piAddrData = NULL;
        int iRows = 0;
        int iCols = 0;
        void* _pvData = NULL;
        int iType = 0;

        //get property name
        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrProp);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddrProp, &pstProName))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i);
            return 1;
        }

        if (bDoCreation &&
                (stricmp(pstProName, "dockable") == 0 ||
                 stricmp(pstProName, "menubar") == 0 ||
                 stricmp(pstProName, "toolbar") == 0))
        {
            // Already set creating new figure
            // but let the set_ function fail if figure already exists
            continue;
        }

        //get address of value on stack
        sciErr = getVarAddressFromPosition(pvApiCtx, i + 1, &piAddrData);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
            return 1;
        }

        getVarType(pvApiCtx, piAddrData, &iType);

        if ((strcmp(pstProName, "user_data") == 0) || (stricmp(pstProName, "userdata") == 0))
        {
            /* in this case set_user_data_property
             * directly uses the  third position in the stack
             * to get the variable which is to be set in
             * the user_data property (any data type is allowed) S. Steer */
            _pvData = (void*)piAddrData;         /*position in the stack */
            iRows = -1;   /*unused */
            iCols = -1;   /*unused */
            iType = -1;
        }
        else
        {
            switch (iType)
            {
                case sci_matrix :
                    getMatrixOfDouble(pvApiCtx, piAddrData, &iRows, &iCols, (double**)&_pvData);
                    break;
                case sci_boolean :
                    getMatrixOfBoolean(pvApiCtx, piAddrData, &iRows, &iCols, (int**)&_pvData);
                    break;
                case sci_handles :
                    getMatrixOfHandle(pvApiCtx, piAddrData, &iRows, &iCols, (long long**)&_pvData);
                    break;
                case sci_strings :
                    if (   strcmp(pstProName, "tics_labels") != 0 && strcmp(pstProName, "auto_ticks") != 0 &&
                            strcmp(pstProName, "axes_visible") != 0 && strcmp(pstProName, "axes_reverse") != 0 &&
                            strcmp(pstProName, "text") != 0 && stricmp(pstProName, "string") != 0 &&
                            stricmp(pstProName, "tooltipstring") != 0) /* Added for uicontrols */
                    {
                        if (getAllocatedSingleString(pvApiCtx, piAddrData, (char**)&_pvData))
                        {
                            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 3);
                            return 1;
                        }
                        iRows = (int)strlen((char*)_pvData);
                        iCols = 1;
                    }
                    else
                    {
                        isMatrixOfString = 1;
                        getAllocatedMatrixOfString(pvApiCtx, piAddrData, &iRows, &iCols, (char***)&_pvData);
                    }
                    break;
                case sci_list :
                    iCols = 1;
                    getListItemNumber(pvApiCtx, piAddrData, &iRows);
                    _pvData = (void*)piAddrData;         /* In this case l3 is the list position in stack */
                    break;
                default :
                    _pvData = (void*)piAddrData;         /* In this case l3 is the list position in stack */
                    break;
            }
        }

        callSetProperty(pvApiCtx, iFig, _pvData, iType, iRows, iCols, pstProName);

        // If backgroundcolor is set :
        // * add it to colormap => performed by callSetProperty
        // * set background to index => performed by callSetProperty
        // * copy value into axes background property
        if (stricmp(pstProName, "backgroundcolor") == 0 && iAxes > 0)
        {
            int iBackground = 0;
            int *piBackground = &iBackground;

            getGraphicObjectProperty(iFig, __GO_BACKGROUND__, jni_int, (void **)&piBackground);
            setGraphicObjectProperty(iAxes, __GO_BACKGROUND__, piBackground, jni_int, 1);
        }

        if (iType == sci_strings)
        {
            //free allacted data
            if (isMatrixOfString == 1)
            {
                freeAllocatedMatrixOfString(iRows, iCols, (char**)_pvData);
            }
            else
            {
                freeAllocatedSingleString((char*)_pvData);
            }
        }
    }

    //return new created fig
    createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
int setDefaultProperties(int _iFig, BOOL _bDefaultAxes)
{
    //get figure axes
    int iAxes = -1;
    int iDrawing = 0;
    int iColorIndex = 0;
    int iFilled = 0;
    int iAxesVisible = 0;
    double pdblNewColor[COLOR_COMPONENT] = {0.8, 0.8, 0.8};

    setGraphicObjectProperty(_iFig, __GO_IMMEDIATE_DRAWING__, &iDrawing, jni_bool, 1);

    iColorIndex = addColor(_iFig, pdblNewColor);

    setGraphicObjectProperty(_iFig, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);
    if (_bDefaultAxes)
    {
        iAxes = getOrCreateDefaultSubwin();
        //set background in figure and axes to new ( or existting ) color
        setGraphicObjectProperty(iAxes, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);

        //a.filled = "off"
        setGraphicObjectProperty(iAxes, __GO_FILLED__, &iFilled, jni_bool, 1);

        //a.axes_visible = "off"
        setGraphicObjectProperty(iAxes, __GO_X_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
        setGraphicObjectProperty(iAxes, __GO_Y_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
        setGraphicObjectProperty(iAxes, __GO_Z_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
    }
    //f.immediate_drawing = "on"
    iDrawing = 1;
    setGraphicObjectProperty(_iFig, __GO_IMMEDIATE_DRAWING__, &iDrawing, jni_bool, 1);

    return iAxes;
}
/*--------------------------------------------------------------------------*/
