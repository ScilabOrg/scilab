/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 -  INRIA - Serge Steer 
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "gensum.h"

#define SUM_DOUBLE(Type) {\
Type *DX;\
 DX=(Type *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) dtemp += (double) DX[i];\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) dtemp += (double) DX[i];\
 }\
}

double C2F(gensum_double)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i;
    static double dtemp;


    dtemp = 0.0;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      SUM(integer1);
      break;
    case 2:
      SUM(integer2);
      break;
    case 4:
      SUM(int) ;
      break;
    case 11:
      SUM(unsigned char);
      break;
    case 12:
      SUM(unsigned short);
      break;
    case 14:
      SUM(unsigned int);
      break;
    }
    return dtemp;
}


