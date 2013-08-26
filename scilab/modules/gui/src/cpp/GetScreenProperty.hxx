/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get the position of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GETSCREENPROPERTY_HXX__
#define __GETSCREENPROPERTY_HXX__

#include "CallScilabBridge.hxx"
#include "UnitsConversion.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "stricmp.h"
#include "returnProperty.h"
#include "GetScreenProperty.h"
#include "localization.h"
#include "Scierror.h"
}

#endif /* !__GETSCREENPROPERTY_HXX__ */
