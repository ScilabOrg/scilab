//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


err = unix(SCI + "/./bin/scilab-cli -e ""exit()"" -quit");
assert_checkequal(err, 0);
err = unix(SCI + "/./bin/scilab-cli -e ""1+1;"" -quit");
assert_checkequal(err, 0);
err = unix(SCI + "/./bin/scilab-cli -e ""1+1; exit(12)"" -quit");
assert_checkequal(err, 12);
err = unix(SCI + "/./bin/scilab-cli -e ""error(\""error_test\"");"" -quit");
assert_checkequal(err, 1);
err = unix(SCI + "/./bin/scilab-cli -e ""error(\""error_test\"");exit(12)"" -quit");
assert_checkequal(err, 1);
err = unix(SCI + "/./bin/scilab-cli -e ""try, error(\""error_test\""); catch, disp(lasterror()),end"" -quit");
assert_checkequal(err, 0);
err = unix(SCI + "/./bin/scilab-cli -e ""try, error(\""error_test\""); catch,disp(lasterror());exit(12), end"" -quit");
assert_checkequal(err, 12);
