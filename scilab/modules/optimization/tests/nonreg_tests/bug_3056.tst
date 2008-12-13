// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3056 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3056
//
// <-- Short Description -->
// scilab crash with the following test and the nd algorithm.

// <-- ENGLISH IMPOSED -->

global xn_1;
global fn_1;
global gn_1;
global iter;
global to_record;

// Rastrigin test function

function [f,df,ind] = myf(x,ind)
  global xn_1;
  global iter;
  global to_record;
  global fn_1;
  global gn_1;

  if to_record then
    xn_1($+1) = x;
    printf("myf: ||xn(1) - xn_1(1)|| = %f - ||xn(2) - xn_1(2)|| = %f - iter = %d",norm(xn_1($)(1)-xn_1($-1)(1)),norm(xn_1($)(2)-xn_1($-1)(2)),iter);
  end

  iter = iter + 1;
  f = x(1)^2+x(2)^2-cos(12*x(1))-cos(18*x(2));
  df(1,1) = 2*x(1) - 12*sin(12*x(1));
  df(2,1) = 2*x(2) - 18*sin(18*x(1));
  if to_record then
    fn_1($+1) = f;
    gn_1($+1) = df;
    printf(" - ||fn - fn_1|| = %f - ||gn - gn_1|| = %f\n",norm(fn_1($)-fn_1($-1)),norm(gn_1($)-gn_1($-1)));
  end
endfunction

Min = [-1;-1];
Max = [1;1];

x0 = [0.25;-0.25];

iter = 0;

to_record = %F;
xn_1 = list();
xn_1($+1) = x0;
fn_1 = list();
gn_1 = list();
[f_tmp, df_tmp, ind] = myf(x0,1);
fn_1($+1) = f_tmp;
gn_1($+1) = df_tmp

to_record = %T;

//[fopt,xopt,gopt]=optim(myf,x0,algo='gc','ar',nap=20,iter=10,epsg=1e-1,epsf=1e-1,epsx=[1e-2;1e-2],imp=2);
[fopt,xopt,gopt]=optim(myf,x0,algo="nd","ar",nap=20,iter=10,epsg=1e-1,epsf=1e-1,epsx=[1e-2;1e-2],imp=2);

list_to_plot = xn_1;
xn_1 = list();
to_record = %F;

x = Min(1):(Max(1)-Min(1))/20:Max(1);
y = Min(2):(Max(2)-Min(2))/20:Max(2);

[X,Y]=meshgrid(x,y);

for i=1:size(X,1)
  for j=1:size(X,2)
    [Z(i,j),tmp1,tmp2] = myf([X(i,j);Y(i,j)],1);
  end
end

scf();
drawlater;
xset("fpf"," ");
contour(x,y,Z', 10);
_axes = get("current_axes");
_axes.data_bounds = [Min(1) Max(1) Min(2) Max(2)];
xtitle("myf","x1","x2");

for i=1:length(list_to_plot)
  plot(list_to_plot(i)(1),list_to_plot(i)(2),"ko");
end

drawnow;
