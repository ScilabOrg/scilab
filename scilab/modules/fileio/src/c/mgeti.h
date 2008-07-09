
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __MGETI_H__
#define __MGETI_H__

#include "machine.h"

/**
 * <long-description>
 *
 * @param char type[]   
 * @param fd    
 * @return <ReturnValue>
 */
int SWAP(char type[],integer *fd); 

/**
 * <long-description>
 *
 * @param fd    
 * @param res   
 * @param n 
 * @param char type[]   
 * @param ierr  
 */
void C2F(mgeti) (integer *fd,integer *res,integer *n,char type[],integer *ierr);

#endif /* __MGETI_H__ */

