/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "gw_statistics.h"
#include "CdfBase.h"

extern int C2F(cdffnc)(int *,double *,double *,double*,double *,double *, int *,double *);

/**
 * Interface to dcdflib's cdffnc
 * SUBROUTINE CDFFNC( WHICH, P, Q, F, DFN, DFD, PNONC, STATUS, BOUND )
 * Cumulative Distribution Function, F distribution Non-Central 
 */
int
cdffncI(char* fname,unsigned long l)
{
  struct cdf_item items[] =
    {{"PQ"   , 4, 2, 2},
     {"F"    , 5, 1, 3},
     {"Dfn"  , 5, 1, 4},
     {"Dfd"  , 5, 1, 5},
     {"Pnonc", 5, 1, 0}};
  struct cdf_descriptor cdf = mkcdf(cdffnc, 5, 6, 1, 2, items);
  return cdf_generic(fname, &cdf); 
} 
