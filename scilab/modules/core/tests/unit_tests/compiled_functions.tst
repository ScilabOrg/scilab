// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// compiled functions

deff('y=foo(a)','y=a+1000')

a = 500;
y = foo(a);

if y<>a+1000 then pause,end
a=y;
deff('y=toto()','y=foo(a)')
y=toto();
if y<>a+1000 then pause,end
a=y;
exec(toto)
if y<>a+1000 then pause,end
a=y;

//in a loop
for k=1:3,
  y=toto();
  if y<>a+1000 then pause,end
  a=y;
end
