// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7998 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7998
//
// <-- Short Description -->
// Accessing to a shared property of the elements of a matrix of graphic handle, may crashed scilab.
//


cols = [1 2 3 4 5];
lgd = [];
for k = 1 : 5
  x = linspace(0,20,1000);
  y = sin ( k * x );
  plot(x,y)
  str = msprintf("k=%d",k);
  lgd($+1) = str;
end
h = gcf();
for k = 1 : 5
  h.children.children.children(k).foreground = cols(k);
end
legend(lgd);

if execstr("h.children.children.children", "errcatch") <> 0 then pause, end  // Should not crash.

