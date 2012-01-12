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
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

cd(SCI);

for i = 1 : size(tab_ref,'*')
  pathnam = TMPDIR + filesep() + 'dir_' + tab_ref(i);
  filenam = pathnam + filesep() + 'file_' + tab_ref(i);
  if deletefile(filenam) <> %t then pause,end
  if ls(pathnam) <> [] then pause,end
end
