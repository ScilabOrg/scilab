
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET - <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_LINEAR_ALGEBRA__
#define __GW_LINEAR_ALGEBRA__
#include "dynlib_linear_algebra.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
#define SCI_DOUBLE 1
#define STRING  10
#define REAL 0
#define COMPLEX 1
#define FUNCTION 13
#define STRINGREAL 27
#define STRINGCOMPLEX 12
#define YES 1
#define NO 0
/*--------------------------------------------------------------------------*/
LINEAR_ALGEBRA_IMPEXP int gw_linear_algebra(void);
/*--------------------------------------------------------------------------*/
LINEAR_ALGEBRA_IMPEXP int C2F(intbackslash)(char *fname,unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int C2F(invnew)(char *fname,unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int C2F(intslash)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
/* used in 'eig' */
LINEAR_ALGEBRA_IMPEXP int sci_dsyev(char *fname, unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int sci_dgeev(char *fname, unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int sci_zgeev(char *fname, unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int sci_zheev(char *fname, unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int sci_dggev(char *fname, unsigned long fname_len);
LINEAR_ALGEBRA_IMPEXP int sci_zggev(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
//LINEAR_ALGEBRA_IMPEXP int C2F(intschur)(char *fname, unsigned long fname_len); => C++ sci_schur

//YaSp
//LINEAR_ALGEBRA_IMPEXP int sci_chol(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_bdiagr(char *fname, void* pvApiCtx); => C++ => sci_bdiag
//LINEAR_ALGEBRA_IMPEXP int sci_svd(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_rcond(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_qr(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_lu(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_lsq(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_inv(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_hess(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_eig(char *fname, void* pvApiCtx); => C++ sci_spec
//LINEAR_ALGEBRA_IMPEXP int sci_det(char *fname, void* pvApiCtx); => C++
//LINEAR_ALGEBRA_IMPEXP int sci_balanc(char *fname, void* pvApiCtx); => C++

/*--------------------------------------------------------------------------*/
#endif /*  __GW_LINEAR_ALGEBRA__ */
/*--------------------------------------------------------------------------*/

