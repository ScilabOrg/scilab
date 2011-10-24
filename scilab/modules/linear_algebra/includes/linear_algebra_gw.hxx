/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LINEAR_ALGEBRA_GW_HXX__
#define __LINEAR_ALGEBRA_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_linear_algebra_gw.h"
}

class LinearAlgebraModule
{
private :
   LinearAlgebraModule(){};
   ~LinearAlgebraModule(){};
public :
   LINEAR_ALGEBRA_GW_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_chol, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_bdiag, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_inv, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_svd, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_rcond, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_qr, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_lu, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_lsq, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_hess, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_spec, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_det, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_balanc, LINEAR_ALGEBRA_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE(sci_schur, LINEAR_ALGEBRA_GW_IMPEXP);

#endif /* !__LINEAR_ALGEBRA_GW_HXX__ */
