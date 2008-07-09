/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : interface for xs2svg routine 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SCI_EXPORTUI_H_
#define _SCI_EXPORTUI_H_

/**
 * interface function for the exportUI routine :
 *
 * @param[in] fname     name of the routine (ie exportUI)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_exportUI( char * fname, unsigned long fname_len ) ;

#endif /* _SCI_EXPORTUI_H_ */
