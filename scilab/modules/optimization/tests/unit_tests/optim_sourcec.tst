// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// optim.tst --
//   Test the optim command with the Rosenbrock test case
//   in the case where the cost function is provided as a Fortran 
//   routine.
//   The "genros" function is defined in optimization/sci_gateway/fortran/Ex-optim.f
//
// This is the precision measured with experiments.
Leps=10^12*%eps;
// n : dimension of the problem
n=3;
bs=10.*ones(n,1);
bi=-bs;
x0=1.2*ones(n,1);
epsx=1.e-15*x0;
xopt=ones(n,1);
// External function written in C (C compiler required)
// write down the C code (Rosenbrock problem)
C=['#include <math.h>'
'double sq(double x)'
'{ return x*x;}'
'void rosenc(int *ind, int *n, double *x, double *f, double *g, '
'                                int *ti, float *tr, double *td)'
'{'
'  double p;'
'  int i;'
'  p=td[0];'
'  if (*ind==2||*ind==4) {'
'    *f=1.0;'
'    for (i=1;i<*n;i++)'
'      *f+=p*sq(x[i]-sq(x[i-1]))+sq(1.0-x[i]);'
'  }'
'  if (*ind==3||*ind==4) {'
'    g[0]=-4.0*p*(x[1]-sq(x[0]))*x[0];'
'    for (i=1;i<*n-1;i++)'
'      g[i]=2.0*p*(x[i]-sq(x[i-1]))-4.0*p*(x[i+1]-sq(x[i]))*x[i]-2.0*(1.0-x[i]);'
'    g[*n-1]=2.0*p*(x[*n-1]-sq(x[*n-2]))-2.0*(1.0-x[*n-1]);'
'  }'
'}'];
mputl(C,TMPDIR+'/rosenc.c')
// compile the C code
libpath=ilib_for_link('rosenc','rosenc.o',[],'c',TMPDIR+'/Makefile');
// incremental linking
linkid=link(libpath,'rosenc','c');
//solve the problem
valtd=100;
// Test with default solver and default settings.
[f,x,g]=optim('rosenc',x0,'td',valtd)
if abs(f-1+norm(x-xopt) ) > Leps then pause,end
// Test with all solvers
solverlist=["gc" "qn" "nd"]
for solver=solverlist
  mprintf("Solver=%s\n",solver)
  [f,x,g]=optim('rosenc',x0,solver,'td',valtd);
  if abs(f-1+norm(x-xopt) ) > Leps then pause,end
end
// Test all verbose levels with all possible solvers
verboselevels=[0 1 2 3]
for verbose=verboselevels
  mprintf("Verbose=%d\n",verbose)
  for solver=solverlist
    mprintf("Solver=%s\n",solver)
    [f,x,g]=optim('rosenc',x0,solver,'td',valtd,imp=verbose);
    if abs(f-1+norm(x-xopt) ) > Leps then pause,end
  end
end
// Clean-up
ulink(linkid)

