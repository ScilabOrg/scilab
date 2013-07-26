// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9821 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9821
//
// <-- Short Description -->
// getrelativename does not manage matrix of strings

computed = getrelativefilename([SCI+'/bin',SCI+'/bin'], [SCI+'/ACKNOWLEDGMENTS',SCI+'/ACKNOWLEDGMENTS']);
expected = pathconvert(["../ACKNOWLEDGMENTS", "../ACKNOWLEDGMENTS"], %f);
assert_checkequal(computed,expected);
