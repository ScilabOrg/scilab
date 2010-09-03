/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CONCURRENCY_H__
#define __CONCURRENCY_H__

/**
* TODO : Add COMMENT
* @return 0: no -> concurrency, 1 -> threads, 2 -> processes, 3 -> both
*/
int parallelConcurrency(void);

/**
* TODO : Add COMMENT
*@return
*/
int parallelForbidden(char const* fname);

#endif /* __CONCURRENCY_H__ */

