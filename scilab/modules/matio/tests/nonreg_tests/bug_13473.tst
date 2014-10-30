// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13473 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13473
//
// <-- Short Description -->
// loadmatfile could miss stack exceedings.

cd TMPDIR;

select getos()
case "Linux"
    unix_g("wget https://cloud.laas.fr/public.php?service=files\&t=2013e1e9593e5a0f75538aa3c73b3e70\&download -O a.mat");
else // Windows | MacOS X
    unix_g("curl https://cloud.laas.fr/public.php?service=files\&t=2013e1e9593e5a0f75538aa3c73b3e70\&download -o a.mat");
end

refMsg = msprintf(_("%s: Cannot open file %s."), "loadmatfile", "a.mat");

assert_checkequal(execstr("loadmatfile(""a.mat"");", "errcatch"), 10000);

errMsg = lasterror();
assert_checkequal(errMsg(1), refMsg);
