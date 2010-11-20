/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "exportToFile.hxx"
#include "FileExporter.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "GetProperty.h"
}

/*---------------------------------------------------------------------------------*/
int exportToFile(sciPointObj * pFigure, const char *fileName, ExportFileType fileType, ExportOrientation orientation)
{

    return org_scilab_modules_graphic_export::FileExporter::fileExport(getScilabJavaVM(),
                                                                       sciGetNum(pFigure), (char *)fileName, fileType, orientation);
}

/*---------------------------------------------------------------------------------*/
