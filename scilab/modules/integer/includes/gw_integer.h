
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __GW_INTEGER__
#define __GW_INTEGER__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_integer_gw.h"
#include "dynlib_integer.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
INTEGER_IMPEXP int C2F(sciinttype)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(scimgeti)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimput)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciiabs)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciidiag)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciitriu)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciitril)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciisum)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciicumsum)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimax)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimin)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimaxi)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimini)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciip)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimatrix)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciiprod)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciicumprod)(char *fname,unsigned long fname_len);

//YaSp
INTEGER_GW_IMPEXP int sci_int8(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_uint8(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_int16(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_uint16(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_int32(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_uint32(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_int64(char *fname,void* pvApiCtx);
INTEGER_GW_IMPEXP int sci_uint64(char *fname,void* pvApiCtx);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_INTEGER__ */
/*--------------------------------------------------------------------------*/

