/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __YASPIO_HXX__
#define __YASPIO_HXX__

#include "dynlib_output_stream.h"

typedef void (*YASP_OUTPUT)(char const *text);
typedef char *(*YASP_INPUT)(void);

OUTPUT_STREAM_IMPEXP void setYaspOutputMethod(YASP_OUTPUT writer);

OUTPUT_STREAM_IMPEXP void setYaspInputMethod(YASP_INPUT reader);

OUTPUT_STREAM_IMPEXP char *YaspRead();

OUTPUT_STREAM_IMPEXP void YaspWrite(char const* text);

#endif /* __YASPIO_HXX__ */
