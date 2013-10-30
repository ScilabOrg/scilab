/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "UIControl.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "MALLOC.h"
#include "localization.h"
#include "stricmp.h"
}

#include "CallScilabBridge.hxx"
#include "ScilabToJava.hxx"
#include "UserDataHandler.hxx"

#include <vector>

int setUICProperties(int objUID, int stackPos, int propertiesCount, char * fname, void * pvApiCtx)
{
    static int handlerId = org_scilab_modules_gui_bridge::CallScilabBridge::initUIControl(getScilabJavaVM());
    std::vector<int> indexes;
    int propertyIndex = 0;
    int userDataPos = -1;
    char ** pnames = (char **)MALLOC(sizeof(char *) * propertiesCount);

    for (int i = 0; i < propertiesCount; i++)
    { 
	int * piAddr = 0;
	char * propertyName = 0;
	SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 2 * i + stackPos, &piAddr);
	if (sciErr.iErr)
	{
	    printError(&sciErr, 0);
	    return 0;
	}

	if (getAllocatedSingleString(pvApiCtx, piAddr, &propertyName))
	{
	    freeAllocatedMatrixOfString(1, propertyIndex, pnames);
	    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2 * i + stackPos);
	    return 0;
	}
	
	if (stricmp(propertyName, "user_data") == 0 || stricmp(propertyName, "userdata") == 0)
	{
	    userDataPos = 2 * i + stackPos + 1;
	}
	else
	{
	    pnames[propertyIndex++] = propertyName;
	    sciErr = getVarAddressFromPosition(pvApiCtx, 2 * i + stackPos + 1, &piAddr);
	    if (sciErr.iErr)
	    {
		freeAllocatedMatrixOfString(1, propertyIndex, pnames);
		printError(&sciErr, 0);
		return 0;
	    }
	    org_modules_types::ScilabToJava::sendVariable("", piAddr, false, handlerId, pvApiCtx);
	}
    }

    org_modules_types::ScilabToJava::sendStringVariable("", indexes, propertyIndex, 1, pnames, false, false, handlerId);
    freeAllocatedMatrixOfString(1, propertyIndex, pnames);
    
    if (userDataPos != -1)
    {
	setUserData(objUID, userDataPos, pvApiCtx);
    }
    
    setUIControlProperties(objUID);
    
    return 1;
}

void getUICProperty(int objUID, char * pname, int stackPos)
{
    getUIControlProperty(objUID, pname, stackPos);
}

void getUserData(int objUID, int stackPos, void * pvApiCtx)
{
    org_scilab_modules_gui_uiwidget::UserDataHandler::get(objUID, stackPos, pvApiCtx);
}

void setUserData(int objUID, int stackPos, void * pvApiCtx)
{
    org_scilab_modules_gui_uiwidget::UserDataHandler::put(objUID, stackPos, pvApiCtx);
}
