// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6902 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6902
// <-- Short Description -->
// ilib_gen_gateway did not regenerate gateway when we need.

cd(TMPDIR);

name = 'gw_example1';
table = ['sci_func1', 'func1'];

WITHOUT_AUTO_PUTLHSVAR = %t;
n1 = ilib_gen_gateway(name,table);
r1 = fileinfo(n1);

WITHOUT_AUTO_PUTLHSVAR = %f;
n2 = ilib_gen_gateway(name,table);
r2 = fileinfo(n2);

// there is a modification then gateway must be modified
if ~or(r1 <> r2) then pause,end

WITHOUT_AUTO_PUTLHSVAR = %f;
n3 = ilib_gen_gateway(name,table);
r3 = fileinfo(n3);
// there is no modification 
if ~and(r2 == r3) then pause,end
// =============================================================================
