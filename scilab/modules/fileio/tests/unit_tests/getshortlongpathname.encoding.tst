// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
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
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"프로그램",
"תוכנית"];

for i = 1 : size(tab_ref,'*')
	dz = "dir_" + tab_ref(i);
	p = cd(dz);
	fz = "file_" + tab_ref(i);
	fullfz = TMPDIR + filesep() + dz + filesep() + fz;
	if getlongpathname(getshortpathname(fullfz)) <> fullfz then pause,end
	cd(TMPDIR);
end
