/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#ifndef __GW_CSV_TOOLS_H__
#define __GW_CSV_TOOLS_H__

#ifdef __cplusplus
extern "C" {
#endif

    int sci_csv_default(char *fname);
    int sci_csv_isnum(char *fname);
    int sci_csv_read(char *fname);
    int sci_csv_stringtodouble(char *fname);
    int sci_csv_textscan(char *fname);
    int sci_csv_write(char *fname);

#ifdef __cplusplus
}
#endif

#endif /* __GW_CSV_TOOLS_H__ */
// =============================================================================

