// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2414 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2414
//
// <-- Short Description -->
//    assignment of empty string an empty variable has disastrous results. Probably a
//    parser bug, for statements of the form a=string(a), with a=[]

a = [];
a = string(a);

if type(a) <> 1            then pause,end
if typeof(a) <> "constant" then pause,end
if or(size(a) <> [0 0])    then pause,end
if a($,1)<>[]              then pause,end
