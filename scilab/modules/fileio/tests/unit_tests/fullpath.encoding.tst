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
	sz = "dir_" + tab_ref(i);
	cd(sz);
	createdir('sub_' + sz);
	cd('sub_' + sz);
	r = fullpath('../file_' + tab_ref(i));
	if r <> TMPDIR + filesep() + sz + filesep() + 'file_' + tab_ref(i) then pause,end
	cd '../';
	removedir('sub_' + sz);
	cd(TMPDIR);
end
