// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11876 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11876
//
// <-- Short Description -->
// ilib_include_flag returns a column vector of string when called
// with a column vector of strings

//Creating temporary directories
cd TMPDIR ;
mkdir('a');
mkdir('b');
mkdir('c');
mkdir('d');

matrix_error_msg = msprintf(...
    _("%s: Wrong size for input argument #%d: Vector expected.\n"), ...
    "ilib_include_flag", 1);

//Testing error handling of matrices of strings instead of vector
assert_checkerror(...
    "ilib_include_flag([''a'', ''b''; ''c'', ''d''])", matrix_error_msg)

//Testing row vector and column vector give the same answer
assert_checkequal(...
    ilib_include_flag(['a', 'b', 'c', 'd']), ...
    ilib_include_flag(['a'; 'b'; 'c'; 'd']))

