/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/* Copyright INRIA */
#include "../machine.h"

extern integer C2F(scierr)();
extern void C2F(itosci)();
extern void C2F(dtosci)();
extern void C2F(vvtosci)();
extern void C2F(scitovv)();
extern void C2F(skipvars)();
extern void C2F(scitod)();
extern void C2F(list2vars)();
extern void C2F(ltopadj)();
extern void C2F(scifunc)();
extern void C2F(mklist)();

void 
sciblk2(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)

integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[],*nin,outsz[],*nout;
double x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;

{
    int k;
    double *y;
    double *u;
    /*int nev,ic;*/
    integer one=1,skip;
    integer nu,ny;
    integer nu2,ny2;
    integer mlhs=5,mrhs=8;
    integer ltop;


    C2F(itosci)(flag,&one,&one);
    if (C2F(scierr)()!=0) goto err;
    C2F(itosci)(nevprt,&one,&one);
    if (C2F(scierr)()!=0) goto err;  
    C2F(dtosci)(t,&one,&one);
    if (C2F(scierr)()!=0) goto err;  
    C2F(dtosci)(x,nx,&one);
    if (C2F(scierr)()!=0) goto err;  
/*    C2F(dtosci)(z,nz,&one);*/
    C2F(vvtosci)(z,nz);
    if (C2F(scierr)()!=0) goto err; 
    C2F(vvtosci)(rpar,nrpar); 
/*    C2F(dtosci)(rpar,nrpar,&one);*/
    if (C2F(scierr)()!=0) goto err;  
    C2F(itosci)(ipar,nipar,&one);
    if (C2F(scierr)()!=0) goto err;
    for (k=0;k<*nin;k++) {
	u=(double *)inptr[k];
	nu=insz[k];
	nu2=insz[*nin+k];
//	C2F(dtosci)(u,&nu,&one);
	C2F(dtosci)(u,&nu,&nu2);
	if (C2F(scierr)()!=0) goto err;
    }
    C2F(mklist)(nin);


    C2F(scifunc)(&mlhs,&mrhs);
    if (C2F(scierr)()!=0) goto err;
    
    switch (*flag) {
    case 1 :
        /* y or z computation */
      {
	skip=2;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	/*C2F(scitod)(z,nz,&one);*/
	C2F(scitod)(x,nx,&one);
      }
      if (*nout==0) {
	skip=1;
	C2F(skipvars)(&skip);
      }
      else {
	C2F(list2vars)(nout,&ltop);
	if (C2F(scierr)()!=0) goto err; 
	for (k=*nout-1;k>=0;k--) {
	  y=(double *)outptr[k];
	  ny=outsz[k];
	  ny2=outsz[*nout+k];
//	  C2F(scitod)(y,&ny,&one);
	  C2F(scitod)(y,&ny,&ny2);
	  if (C2F(scierr)()!=0) goto err;
	}
	/* list2vars has changed the Lstk(top+1) value. 
	   reset the correct value */
	C2F(ltopadj)(&ltop);  
      }
      break;
    case 0 :
	/*  x'  computation */
      {
 	/*skip=4;
	  C2F(skipvars)(&skip);*/
	C2F(scitod)(xd,nx,&one);
	skip=4;
	C2F(skipvars)(&skip);
	break;
      }
    case 2 :
      {
	skip=2;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	/*C2F(scitod)(z,nz,&one);*/
	C2F(scitod)(x,nx,&one);
	skip=1;
	C2F(skipvars)(&skip);
      }
      break;
    case 3 :
	skip=1;
	C2F(skipvars)(&skip);
	C2F(scitod)(tvec,ntvec,&one);
        skip=3;
	C2F(skipvars)(&skip);
	break;
    case 4 :
	skip=2;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	/*C2F(scitod)(z,nz,&one);*/
	C2F(scitod)(x,nx,&one);
	skip=1;
	C2F(skipvars)(&skip);
	break;
    case 5 :
	skip=2;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	/*C2F(scitod)(z,nz,&one);*/
	C2F(scitod)(x,nx,&one);
        skip=1;
	C2F(skipvars)(&skip);
	break;
    case 6 :
	skip=2;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	/*C2F(scitod)(z,nz,&one);*/
	C2F(scitod)(x,nx,&one);
	if (*nout==0) {
	    skip=1;
	    C2F(skipvars)(&skip);
	}
	else {
	    C2F(list2vars)(nout,&ltop); 
	    if (C2F(scierr)()!=0) goto err;
	    for (k=*nout-1;k>=0;k--) {
		y=(double *)outptr[k];
		ny=outsz[k];
		C2F(scitod)(y,&ny,&one);
		if (C2F(scierr)()!=0) goto err;
	    }
	    /* list2vars has changed the Lstk(top+1) value. 
	       reset the correct value */
	   C2F(ltopadj)(&ltop);  
	}
	break;
    }
    return;
 err: 
    *flag=-1;
}
