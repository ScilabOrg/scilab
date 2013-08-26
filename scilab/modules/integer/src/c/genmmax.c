/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "genmmax.h"

#define MMAX(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
      if (*job==2) {\
        for (l = 1; l <= *m; ++l){\
          DY[l] = DX[l];DK[l] = 1;\
	  for (k = 1; k < *n; ++k) {\
            if (DX[l+k*ix] > DY[l]) {\
              DY[l] = DX[l+k*ix];DK[l] = k+1;}}}}\
      if (*job==1) {\
        for (k = 0; k < *n; ++k){\
          kk = k*ix;\
          DY[1+k*iy] = DX[1+kk];DK[1+k*iy] = 1;\
	  for (l = 2; l <= *m; ++l) {\
            if (DX[l+kk] > DY[1+k*iy]) {\
              DY[1+k*iy] = DX[l+kk];DK[1+k*iy] = l;}}}}\
      if (*job==0) {\
        DY[1] = DX[1];DK[1] = 1;\
        for (k = 0; k < *n; ++k){\
          kk = k*ix;\
	  for (l = 1; l <= *m; ++l) {\
            if (DX[l+kk] > DY[1]) {\
	      DY[1] = DX[l+kk];DK[1] = l + kk;}}}}\
}


int C2F(genmmax)(int *typ, int *job, int *m, int *n, int *dx, int *incx, int *dy, int *dk, int *incy)
{
    static int ix, iy, l, k, kk, *DK;

    DK = --dk;
    ix = *incx;
    iy = *incy;

    switch (*typ)
    {
        case 1:
            MMAX(integer1);
            break;
        case 2:
            MMAX(integer2);
            break;
        case 4:
            MMAX(int) ;
            break;
        case 11:
            MMAX(unsigned char);
            break;
        case 12:
            MMAX(unsigned short);
            break;
        case 14:
            MMAX(unsigned int);
            break;
    }
    return 0;
}

