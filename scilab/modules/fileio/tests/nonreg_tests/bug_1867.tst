// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1867 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1867
//
// <-- Short Description -->
//    msscanf fails to parse formats with a large number of entries (larger 
//    than 30 it
//    seems).

str1 = '1 2 3 4 5 6 7 8 9 10';
L1 = msscanf(str1,"%d %d %d %d %d %d %d %d %d %d");
if or(L1<>[1:10]) then pause,end

str2 = '..
1 2 3 4 5 6 7 8 9 10 ..
11 12 13 14 15 16 17 18 19 20 ..
21 22 23 24 25 26 27 28 29 30 ..
31 32 33 34 35 36 37 38 39 40 ..
41 42 43 44 45 46 47 48 49 50';

L2 = msscanf(str2,"..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d");
if or(L2<>[1:50]) then pause,end

str3 = '..
1 2 3 4 5 6 7 8 9 10 ..
11 12 13 14 15 16 17 18 19 20 ..
21 22 23 24 25 26 27 28 29 30 ..
31 32 33 34 35 36 37 38 39 40 ..
41 42 43 44 45 46 47 48 49 50 ..
51 52 53 54 55 56 57 58 59 60..
';

StrExec="..
L3 = msscanf(str3,""..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d ..
%d %d %d %d %d %d %d %d %d %d..
"");";

errmsg=execstr(StrExec,'errcatch');
if errmsg<>0 then pause,end
