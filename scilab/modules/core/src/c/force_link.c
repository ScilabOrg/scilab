/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef _MSC_VER
/***********************************************************
* Unused function just here to force linker to load some
* functions
***********************************************************/
/*
extern int   Blas_contents(int);
extern int   Lapack_contents(int);
extern int   Calelm_contents(int);
extern int   Sun_contents(int);
extern int   System2_contents(int);
extern int   System_contents(int);
extern int   Intersci_contents(int);
extern int   Sparse_contents(int);
*/
int ForceLink(void);
int ForceLink(void)
{
    /*
    commented for test purpose by Sylvestre (no sure it is mandatory
    Blas_contents(0);
    Lapack_contents(0);
    Calelm_contents(0);
    Sun_contents(0);
    System2_contents(0);
    System_contents(0);
    Intersci_contents(0);
    Sparse_contents(0);
    */
    return 0;
}
#endif
/*--------------------------------------------------------------------------*/
