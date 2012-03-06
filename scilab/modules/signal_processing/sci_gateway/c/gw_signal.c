/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "gw_signal.h"
#include "callFunctionFromGateway.h"
#include <stdlib.h>

#define GWTABLE_LENGTH(t) (sizeof(Tab) / sizeof(gw_generic_table))

static gw_generic_table Tab[] = {
    {NULL, ""}, /* placeholder */
    {NULL, ""}, //fft
    {NULL, ""}, /* placeholder */
    {NULL, ""}, //corr
    {NULL, ""}, //rpem
    {NULL, ""}, //amell
    {NULL, ""}, //delip
    {NULL, ""}, //remez
    {NULL, ""},  //syredi
    {NULL, ""} // conv2
};

int gw_signal_processing(void)
{
  return 0;

}
