/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Alan Layec
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2012 - INRIA - Serge STEER
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __FFTW_UTILITIES__
#define __FFTW_UTILITIES__

#include "stack-c.h"
#include <string.h>
#include <stdio.h>
#include "fftw3.h"

/* definiton of a guru_dim structure type */
typedef struct guru_dim_st
{
  int rank;
  fftw_iodim *dims;
  int howmany_rank;
  fftw_iodim *howmany_dims;
} guru_dim_struct;

enum Plan_Type {
  C2C_PLAN=0, 
  R2C_PLAN=1, 
  C2R_PLAN=2, 
  R2R_PLAN=3
};

/* definition of a FFTW_Plan structure type */
typedef struct fftw_plan_st
{
  /* stored parameters of fftw_plan_guru_split_dft function */
  enum Plan_Type plan_type;
  fftw_plan p;
  guru_dim_struct gdim;
  unsigned flags;
} FFTW_Plan_struct;

extern int C2F(dset)(int *n, double *dx, double *dy, int *incy);

/* prototypes of utilities functions */
fftw_plan GetFFTWPlan(enum Plan_Type type, guru_dim_struct *gdim,
                      double *ri, double *ii,
                      double *ro, double *io,
                      unsigned flags, int isn);

int FreeFFTWPlan(FFTW_Plan_struct *Sci_Plan);

int CheckGuruDims(guru_dim_struct *gdim1,
                  guru_dim_struct *gdim2);

void ExecuteFFTWPlan(enum Plan_Type type, const fftw_plan p, double *ri, double *ii,double *ro, double *io);

int is_real(double *Ar,double *Ai, int ndims, int *dims);
int check_array_symmetry(double *Ar,double *Ai, guru_dim_struct gdim);
int complete_array(double *Ar,double *Ai, guru_dim_struct gdim);
#endif /* __FFTW_UTILITIES__ */
/*--------------------------------------------------------------------------*/
