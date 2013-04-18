// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 8162 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8162
//
// <-- Short Description -->
// Area of stability in continuous is wrong

s=poly(0,'s');
n=[1+s   2+3*s+4*s^2        5; 0        1-s             s];
d=[1+3*s   5-s^3           s+1;1+s     1+s+s^2      3*s-1];
h=syslin('c',n./d); 
assert_checktrue(execstr("plzr(h)","errcatch")==0);
fig=gcf();
ax=fig.children;
assert_checktrue(ax.children(2).type=='Segs');
assert_checktrue(ax.children(2).data-[0,-1.6289709;0,1.6289709]<1d-5);
close
