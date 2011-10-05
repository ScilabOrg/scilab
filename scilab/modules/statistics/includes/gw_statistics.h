/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __GW_STATISTICS__
#define __GW_STATISTICS__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "dynlib_statistics.h"
#include "api_scilab.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/ 
STATISTICS_IMPEXP int gw_statistics(void);
/*--------------------------------------------------------------------------*/ 

STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfbet);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfbin);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfchi);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfchn);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdff);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdffnc);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfgam);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfnbn);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfnor);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdfpoi);
STATISTICS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cdft);

/*
//YaSp
STATISTICS_IMPEXP int cdfchiI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdftI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfgamI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdffI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdffncI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfpoiI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfbinI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfbetI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfchnI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfnbnI(char* fname, int* _piKey);
STATISTICS_IMPEXP int cdfnorI(char* fname, int* _piKey);
*/
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_STATISTICS__ */
/*--------------------------------------------------------------------------*/

