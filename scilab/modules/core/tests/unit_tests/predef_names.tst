// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
variables_names_1 = predef('names');
assert_checkequal(size(grep(variables_names_1,'jnull'), '*'), 1);
assert_checkequal(size(grep(variables_names_1,'evoid'), '*'), 1);

clear
// we add 3 variables
toto_1 = 1;
toto_2 = 1;
toto_3 = 1;

predef("all"); // and protect
variables_names_2 = predef('names');
assert_checktrue(grep(variables_names_2,'toto_1') <> []);
assert_checktrue(grep(variables_names_2,'toto_2') <> []);
assert_checktrue(grep(variables_names_2,'toto_3') <> []);

