// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1597 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1597
//
// <-- Short Description -->
// Parser incorrect behaviour. The parser neither handles correctly syntax like
//  gce().children; sin(1)(1), nor produces an error

function r=foo,r=1:3,endfunction
if execstr("foo().a","errcatch")<>250 then pause,end
if execstr("foo()(1)","errcatch")<>250 then pause,end

if execstr("who().a","errcatch")<>250 then pause,end
if execstr("who()(1)","errcatch")<>250 then pause,end

if execstr("sin(1:3).a","errcatch")<>250 then pause,end
if execstr("sin(1:3)(1)","errcatch")<>250 then pause,end


function r=foo1(a),r=1,endfunction
if execstr("foo1(2).a","errcatch")<>250 then pause,end
if execstr("foo(2)(1)","errcatch")<>250 then pause,end
