// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1805 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1805
//
// <-- Short Description -->
//    avaragout oddities

// <-- ENGLISH IMPOSED -->

funcprot(0);

function varargout=test(),varargout="";a=1,endfunction
if execstr("test()","errcatch")<>140 then pause,end


function varargout=test();a=1,endfunction
if execstr("test()","errcatch")<>21 then pause,end

