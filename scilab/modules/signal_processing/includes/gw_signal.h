/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__
/*--------------------------------------------------------------------------*/
#include "dynlib_signal_processing.h"
#include "dynlib_signal_processing_gw.h"
#include "machine.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
SIGNAL_PROCESSING_IMPEXP int gw_signal_processing(void);
/*--------------------------------------------------------------------------*/
EXTERN_SIGNAL_PROCESSING_GW STACK_GATEWAY_PROTOTYPE(sci_remez);
EXTERN_SIGNAL_PROCESSING_GW STACK_GATEWAY_PROTOTYPE(sci_amell);
EXTERN_SIGNAL_PROCESSING_GW STACK_GATEWAY_PROTOTYPE(sci_conv2);
#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

