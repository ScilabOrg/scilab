// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3607 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3607
//
// <-- Short Description -->
// wrong value <option value="-Xmx1024m"/>


res = mgetl(SCI+'/etc/jvm_options.xml');
line = grep(res,'<option value=""-Xmx');
assert_checkequal(stripblanks(res(line)) ,'<option value=""-Xmx256m""/>');

stacksize('max');
memstack = stacksize();
if memstack < 1e8 then pause,end
