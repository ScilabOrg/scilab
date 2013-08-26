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

#include "geninsert2.h"

#define INSERT2(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
    i1 = *mj;\
    for (j = 1; j <= i1; ++j) {\
	ljj = (indxj[j] - 1) * (*mr);\
	jj = (j - 1) * (*m3);\
	i2 = *mi;\
	for (i = 1; i <= i2; ++i)\
	    TO[indxi[i] + ljj] = FROM[(i - 1 + jj) * (*inc3) + 1];\
    }\
}

int C2F(geninsert2)(int *typ, int *mj, int *mi, int *indxj, int *indxi, int *to, int *mr, int *from, int *m3, int *inc3)
{
    int i1, i2;
    static int i, j, ljj, jj;

    --indxi;
    --indxj;

    switch (*typ)
    {
        case 1:
            INSERT2(integer1);
            break;
        case 2:
            INSERT2(integer2);
            break;
        case 4:
            INSERT2(int) ;
            break;
        case 11:
            INSERT2(unsigned char);
            break;
        case 12:
            INSERT2(unsigned short);
            break;
        case 14:
            INSERT2(unsigned int);
            break;
    }
    return 0;
}

