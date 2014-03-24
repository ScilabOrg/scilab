/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Functions to export files
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef _EXPORT_TO_FILE_HXX_
#define _EXPORT_TO_FILE_HXX_

#ifdef __cplusplus
extern "C" {
#endif

/** Enum listing the supported type of export files */
typedef enum
{
    NO_EXPORT = 0,
    BMP_EXPORT = 1,
    GIF_EXPORT = 2,
    JPG_EXPORT = 3,
    PNG_EXPORT = 4,
    PPM_EXPORT = 5,
    EPS_EXPORT = 6,
    PDF_EXPORT = 7,
    SVG_EXPORT = 8,
    PS_EXPORT = 9,
    EMF_EXPORT = 10,
}
ExportFileType;

typedef enum
{
    EXPORT_SUCCESS = 0,
    EXPORT_IOEXCEPTION_ERROR = 1,
    EXPORT_INVALID_FILE = 2,
} ExportError;

typedef enum
{
    EXPORT_PORTRAIT = 0,
    EXPORT_LANDSCAPE = 1
} ExportOrientation;

char * exportToFile(int iFigureUID,
                    const char * fileName,
                    ExportFileType fileType,
                    const float jpegCompressionQuality,
                    ExportOrientation orientation);

#ifdef __cplusplus
}
#endif

#endif /* _EXPORT_TO_FILE_HXX_ */

