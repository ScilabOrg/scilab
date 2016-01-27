/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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

#ifndef __GETPIDC_H__
#define __GETPIDC_H__

#include "dynlib_io.h"
/**
* Retrieve the pid (process id)
* @param[out] id1 the id of the process
*/
IO_IMPEXP int getpidc(int *id1);


#endif /* __GETPIDC_H__ */

