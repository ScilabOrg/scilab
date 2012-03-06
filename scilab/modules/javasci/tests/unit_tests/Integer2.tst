// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2008 - INRIA - Allan CORNET
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// jdk must be installed on your pc
//
java_file_to_test = "Integer2.java";

my_test_path = pathconvert(SCI+"/modules/javasci/tests/unit_tests",%f,%f);
my_file_path = pathconvert(my_test_path+"/"+java_file_to_test,%f,%f);
exec (my_test_path+"/test_java.sci",-1);
assert_checktrue(test_java(my_file_path));