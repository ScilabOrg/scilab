// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2039 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2039
//
// <-- Short Description -->
//    non empty lasterror on Scilab startup
//
//
//    Startup execution:
//      loading initial environment
//
//    -->[str,n,line,func]=lasterror(%f)
//     func  =
//
//
//     line  =
//
//        0.
//     n  =
//
//        999.
//     str  =
//
//     Cannot query value of this type.
//
// ...

if lasterror(%F) <> [] then pause,end
