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
assert_checkequal(size(grep(variables_names_1,'SCI'), '*') <> [], %t);
assert_checkequal(size(grep(variables_names_1,'corelib'), '*'), 1);

clear
// we add 3 variables
toto_1 = 1;
toto_2 = 1;
toto_3 = 1;

predef("all"); // and protect
variables_names_2 = predef('names');
ref = ["toto_1";
       "toto_2";
       "toto_3"];
assert_checkequal(variables_names_2($-2:$), ref);
