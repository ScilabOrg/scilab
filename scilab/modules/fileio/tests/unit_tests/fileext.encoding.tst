// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

cd(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"حريات وحقوق",
"תוכנית"];

for i = 1 : size(tab_ref,'*')
	fz = TMPDIR + filesep() + "dir_" + tab_ref(i) + filesep() + 'file_' + tab_ref(i) +'.txt';
	fd = mopen(fz,'wt');
	mclose(fd);
	if fileext(fz) <> '.txt' then pause,end
end
