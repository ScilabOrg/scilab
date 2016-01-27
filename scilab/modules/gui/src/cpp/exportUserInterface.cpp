/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#include "exportUserInterface.hxx"
#include "CallScilabBridge.hxx"


extern "C"
{
#include "FigureList.h"
#include "getScilabJavaVM.h"
}

/*---------------------------------------------------------------------------------*/
int exportUserInterface(int figureId)
{
    return org_scilab_modules_gui_bridge::CallScilabBridge::newExportFileChooser(getScilabJavaVM(), getFigureFromIndex(figureId));
}
/*---------------------------------------------------------------------------------*/
