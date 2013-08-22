// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2867 -->
//

function bug_function1(n);stacksize(n);endfunction

function bug_function2(n);gstacksize(n);endfunction

MEM_STACKSIZE =  1000000;
MEM_GSTACKSIZE = 5000000;

bug_function1(MEM_STACKSIZE);
bug_function2(MEM_GSTACKSIZE);

mem = stacksize();
memg = gstacksize();

if (mem(1) <> MEM_STACKSIZE) then pause,end
if (memg(1) <> MEM_GSTACKSIZE) then pause,end

bug_function1("max");
bug_function2("max");

bug_function1("min");
bug_function2("min");

