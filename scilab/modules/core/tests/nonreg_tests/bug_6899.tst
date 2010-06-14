// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6899 -->
//
// <-- JVM NOT MANDATORY -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6899
//
// <-- Short Description -->
// if a "resume" output arg is a protected variable, Scilab may crash.


deff('foo','%i=resume(1)','n')
if execstr('foo()','errcatch') <>13 then pause,end
try
  foo()
  pause
catch
  [str,n]=lasterror() 
  if n<>13 then pause,end
end

comp(foo)
if execstr('foo()','errcatch') <>13 then pause,end

try
  foo()
  pause
catch
  [str,n]=lasterror() 
  if n<>13 then pause,end
end
