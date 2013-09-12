// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//1-Creating interface source file (matusr.f)
// from ex10fi.desc file by call to intersci
// Making object files
//   Interface file '/tmp/ex10fi.o'
//   User's files '/tmp/ex10f.o';
//
files=G_make(["/tmp/ex10fi.o","/tmp/ex10f.o"],"ex10.dll");
//2-Link object files .o with addinter
//addinter(files,'ex10fi',matusr_funs);
exec("ex10fi.sce");
//Test Scilab functions:
// calc1: 3 matrix input variables and at most 3 output variables of
// types double, real, int
l=calc10(list(1,[2,3],[1,2;3,4]));
if norm(l(1)-2) > %eps then pause,end
if norm(l(2)-([2,3]+[1,2])) > %eps then pause,end
if norm(l(3)-([1,2;3,4]+[1,3;2,4])) > %eps then pause,end





