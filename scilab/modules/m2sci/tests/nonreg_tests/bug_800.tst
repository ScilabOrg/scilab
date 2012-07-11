// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 800 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=800
//
// <-- Short Description -->
//    mfile2sci can not convert expressions such as
//    x=1/ -2;
//    (Blank space between / and - is important, it works if no
//    blank space)

MFILECONTENTS=["a=1/-2;";"b=1/-23;";"c=1/-25;";"d=1/ -c;";"e=1 /  -36;x=1/-2";"f=1 / (-36);"];

MFILE=TMPDIR+"/bug800.m";
SCIFILE=TMPDIR+"/bug800.sci";

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"a = 1/(-2);";
		"b = 1/(-23);";
		"c = 1/(-25);";
		"d = 1/(-c);";
		"e = 1/(-36);x = 1/(-2)";
		"f = 1/(-36);"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
