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

#include "genmin2.h"

#define MIN2(Xtype,Ytype) {\
Xtype *DX;\
Ytype *DY;\
    DY=(Ytype *)dy;\
    --DY;\
    DX=(Xtype *)dx;\
    --DX;\
    ix = 1;\
    for (i = 1; i <= *n; ++i) {\
      if ((Ytype)DX[ix] < DY[i]) {\
        DY[i] = (Ytype)DX[ix];\
	ind[i-1] = *no;}\
	ix = ix + *incx;\
    }\
}

void C2F(genmin2)(int *n, void *dx, int *incx, int *xtyp, void *dy, int *ytyp, int *ind, int *no)
{
    static int i, ix;
    /*    printf("%d %d\n",*xtyp,*ytyp);*/
    switch (*xtyp)
    {
        case 0:
            switch (*ytyp)
            {
                case 0:
                    MIN2(double, double);
                    break;
                case 4:
                    MIN2(double, integer);
                    break;
                case 2:
                    MIN2(double, integer2);
                    break;
                case 1:
                    MIN2(double, integer1);
                    break;
                case 14:
                    MIN2(double, unsigned int);
                    break;
                case 12:
                    MIN2(double, unsigned short);
                    break;
                case 11:
                    MIN2(double, unsigned char);
                    break;
            }
            break;
        case 1:
            switch (*ytyp)
            {
                case 0:
                    MIN2(integer1, double);
                    break;
                case 4:
                    MIN2(integer1, integer);
                    break;
                case 2:
                    MIN2(integer1, integer2);
                    break;
                case 1:
                    MIN2(integer1, integer1);
                    break;
                case 14:
                    MIN2(integer1, unsigned int);
                    break;
                case 12:
                    MIN2(integer1, unsigned short);
                    break;
                case 11:
                    MIN2(integer1, unsigned char);
                    break;
            }
            break;
        case 2:
            switch (*ytyp)
            {
                case 0:
                    MIN2(integer2, double);
                    break;
                case 4:
                    MIN2(integer2, integer);
                    break;
                case 2:
                    MIN2(integer2, integer2);
                    break;
                case 1:
                    MIN2(integer2, integer1);
                    break;
                case 14:
                    MIN2(integer2, unsigned int);
                    break;
                case 12:
                    MIN2(integer2, unsigned short);
                    break;
                case 11:
                    MIN2(integer2, unsigned char);
                    break;
            }
            break;
        case 4:
            switch (*ytyp)
            {
                case 0:
                    MIN2(integer, double);
                    break;
                case 2:
                    MIN2(integer, integer2);
                    break;
                case 4:
                    MIN2(integer, integer);
                    break;
                case 11:
                    MIN2(integer, unsigned char);
                    break;
                case 12:
                    MIN2(integer, unsigned short);
                    break;
                case 14:
                    MIN2(integer, unsigned int);
                    break;
            }
            break;
        case 11:
            switch (*ytyp)
            {
                case 0:
                    MIN2(unsigned char, double);
                    break;
                case 4:
                    MIN2(unsigned char, integer);
                    break;
                case 2:
                    MIN2(unsigned char, integer2);
                    break;
                case 1:
                    MIN2(unsigned char, integer1);
                    break;
                case 14:
                    MIN2(unsigned char, unsigned int);
                    break;
                case 12:
                    MIN2(unsigned char, unsigned short);
                    break;
                case 11:
                    MIN2(unsigned char, unsigned char);
                    break;
            }
            break;
        case 12:
            switch (*ytyp)
            {
                case 0:
                    MIN2(unsigned short, double);
                    break;
                case 4:
                    MIN2(unsigned short, integer);
                    break;
                case 2:
                    MIN2(unsigned short, integer2);
                    break;
                case 1:
                    MIN2(unsigned short, integer1);
                    break;
                case 14:
                    MIN2(unsigned short, unsigned int);
                    break;
                case 12:
                    MIN2(unsigned short, unsigned short);
                    break;
                case 11:
                    MIN2(unsigned short, unsigned char);
                    break;
            }
            break;
        case 14:
            switch (*ytyp)
            {
                case 0:
                    MIN2(integer, double);
                    break;
                case 4:
                    MIN2(integer, integer);
                    break;
                case 2:
                    MIN2(integer, integer2);
                    break;
                case 1:
                    MIN2(integer, integer1);
                    break;
                case 14:
                    MIN2(integer, unsigned int);
                    break;
                case 12:
                    MIN2(integer, unsigned short);
                    break;
                case 11:
                    MIN2(integer, unsigned char);
                    break;
            }
            break;
    }
}

