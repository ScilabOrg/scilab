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
#include "genmsum.h"

static int c__1 = 1;
#define MSUM_DOUBLE(Type) {\
Type *A;\
double *V;\
    A=(Type *)a;\
    V=(Type *)v;\
    iv = 0;\
    if (*job == 0) {\
	t = 0;\
	for (j = 0; j < *n; ++j) \
	    t +=  C2F(gensum_double)(typ,m, &A[j * (*na)], &c__1);\
	v[0] = (Type)t;}\
    else if (*job == 1) {\
	for (j = 0; j < *n; ++j) {\
	    t =  C2F(gensum_double)(typ,m, &A[j * (*na) ], &c__1);\
	    v[iv] = (Type)t;iv += *nv;\
	}}\
    else if (*job == 2) {\
	for (i = 0; i < *m; ++i) {\
	    t =  C2F(gensum_double)(typ,n, &A[i], m);\
	    v[iv] = (Type)t;iv += *nv;\
	}\
    }\
}

int C2F(genmsum_double)(int *typ,int *job, int *a, int *na, int *m, int *n, double *v, int *nv)
{
  static int  i, j, t, iv;

  switch (*typ) {
  case 1:
    MSUM(integer1);
    break;
  case 2:
    MSUM(integer2);
    break;
  case 4:
    MSUM(int) ;
    break;
  case 11:
    MSUM(unsigned char);
    break;
  case 12:
    MSUM(unsigned short);
    break;
  case 14:
    MSUM(unsigned int);
    break;
  }
  return 0;
}
