// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Antoine Elias - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


ilib_verbose(0);

chdir(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);

fileC = ['void test(){}'];

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"프로그램",
"תוכנית"];

PathOrigin = getenv("PATH");

if getos() == 'Windows' then

  for i = 1 : size(tab_ref,'*')
    sz = "dir_" + tab_ref(i);
    a = chdir(sz);
    assert_checkequal(a, %T);
    szTemp = getshortpathname(TMPDIR + filesep() + sz);
    NewPath = PathOrigin + ";" + szTemp;
    setenv("PATH", NewPath);
    mputl(fileC,'test.c');
    ilib_for_link('test','test.c',[],"c");
    chdir(SCI);
    commandstr = "link(" + """libtest" + getdynlibext() +""")";
    ierr = execstr(commandstr, "errcatch");
    assert_checkequal(ierr, 0);
    ulink();
    setenv("PATH", PathOrigin);
    chdir(TMPDIR);
  end

end
sleep(2000);
chdir(TMPDIR);

