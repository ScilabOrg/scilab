/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#ifndef __SCILAB_READ_H__
#define __SCILAB_READ_H__

#include "dynlib_console.h"
#include "machine.h"

typedef char* (*SCILAB_INPUT_METHOD)(void);

CONSOLE_IMPEXP void C2F(scilabread)(char* strRead, int len);

CONSOLE_IMPEXP void setScilabInputMethod(SCILAB_INPUT_METHOD reader);
CONSOLE_IMPEXP char* scilabRead();

#endif /* !__SCILAB_READ_H__ */
