/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GW_OUTPUT_STREAM_H__
#define __GW_OUTPUT_STREAM_H__

#include "dynlib_output_stream.h"
#include "c_gateway_prototype.h"

/*--------------------------------------------------------------------------*/
OUTPUT_STREAM_IMPEXP int gw_output_stream(void);
/*--------------------------------------------------------------------------*/
OUTPUT_STREAM_IMPEXP int sci_print(char *fname,unsigned long fname_len);

//YaSp
OUTPUT_STREAM_IMPEXP C_GATEWAY_PROTOTYPE(sci_diary);
/*--------------------------------------------------------------------------*/
#endif /* __GW_OUTPUT_STREAM_H__ */
/*--------------------------------------------------------------------------*/

