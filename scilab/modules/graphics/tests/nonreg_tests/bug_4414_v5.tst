// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4236 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4236
//
// <-- Short Description -->
// Complex grayplots are not saved correctly in scilab 5.1
// 
warning("off");
x = [1:2]';
y = [1:4];
z = cos(x)*cos(y);
grayplot(x,y,z);
f = gcf();
save(TMPDIR + "/save.scg", f);
close
load(TMPDIR + "/save.scg");
e = gce();
if or(e.data.x <> x) then pause; end
if or(e.data.y <> y') then pause; end
if or(e.data.z <> z) then pause; end

