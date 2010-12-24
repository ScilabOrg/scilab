// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2181 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2181
//
// <-- Short Description -->
//    getrelativefilename crashes when the two arguments it receives refer to 
//    different drives.
//
//    Francois

if getos() == 'Windows' then
	
	test1 = getrelativefilename("D:\","C:\Program Files\scilab\readme.txt");
	test2 = getrelativefilename("C:\","C:\Program Files\scilab\readme.txt");
	test3 = getrelativefilename("C:\Documents and Settings","C:\Program Files\scilab\readme.txt");
	test4 = getrelativefilename("C:\PROGRAM FILES\toto","c:\program files\scilab\readme.txt");
	
	if test1 <> "C:\Program Files\scilab\readme.txt" then pause,end
	if test2 <> "C:\Program Files\scilab\readme.txt" then pause,end
	if test3 <> "..\Program Files\scilab\readme.txt" then pause,end
	if test4 <> "..\scilab\readme.txt"               then pause,end
	
	if isdir('d:') then
		cd("d:");
		test5 = getrelativefilename("D:\","C:\Program Files\scilab\readme.txt");
		if test5 <> "C:\Program Files\scilab\readme.txt" then pause,end
	end
	
end
