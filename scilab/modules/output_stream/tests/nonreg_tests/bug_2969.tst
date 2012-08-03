// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2969 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2969
//

x = uint32(170 * 256 * 256 * 256);
disp(x);
printf('(hex) x = %8x\n',x);
printf('(int) x = %12u\n',x);
printf('(float) x = %12.0f\n',x);

if sprintf('%8x',x) <> 'aa000000' then pause,end
if sprintf('%12u',x) <> '  2852126720' then pause,end
if sprintf('%12.0f',x) <> '  2852126720' then pause,end
     