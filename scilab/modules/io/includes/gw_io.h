/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GW_IO_H__
#define __GW_IO_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_io.h"
/*--------------------------------------------------------------------------*/
IO_IMPEXP int gw_io(void);
/*--------------------------------------------------------------------------*/

//YaSp
int sci_getenv(char *fname, void* pvApiCtx);
int sci_setenv(char *fname, void* pvApiCtx);
//IO_IMPEXP int sci_load(char *fname, void* pvApiCtx); Now in C++ in io_gw/sci_gateway/cpp/sci_load.cpp

int sci_read(char *fname, unsigned long fname_len);
int sci_oldsave(char *fname, unsigned long fname_len);
//int sci_write(char *fname, unsigned long fname_len);
int sci_readb(char *fname, unsigned long fname_len);
int sci_writb(char *fname, unsigned long fname_len);
int sci_getpid(char *fname, unsigned long fname_len);
int sci_read4b(char *fname, unsigned long fname_len);
int sci_write4b(char *fname, unsigned long fname_len);
int sci_save(char *fname, void *pvApiCtx);
int sci_percent_load(char *fname, unsigned long fname_len);
int sci_getio(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_IO_H__ */
/*--------------------------------------------------------------------------*/

