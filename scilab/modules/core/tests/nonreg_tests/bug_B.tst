// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug B -->
//
// <-- OLD BUG -->

if or((1|[1,0,1;1,1,0])<>[%T,%T,%T;%T,%T,%T]) then pause,end
