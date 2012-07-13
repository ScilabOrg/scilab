// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9686 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9686
//
// <-- Short Description -->
// error in an if expression inside a try makes Scilab froze
a=%f;b=%f;
try
  if error(999) then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  while error(999) then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  select error(999) 
  else  
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  select %t 
  case  error(999) then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  select %t 
  case  null() then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);



a=%f;b=%f;
try
  if %t then
    error(999)
    a=%t;  
  end
catch
  b=%t;
end
assert_checktrue(~a&b);


a=%f;b=%f;
try
  while %t then
    error(999)
    a=%t;  
  end
catch
  b=%t;
end
assert_checktrue(~a&b);


