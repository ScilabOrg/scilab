// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3640 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3640
//
// <-- Short Description -->
// Calculating the mean value in the 3rd dimension "mean(A,3)", gives an error for input matrices of a certain size


test = rand(150,1,165);
ierr = execstr("r = mean(test,3);","errcatch");
if ierr <> 0 then pause,end
if size(r) <> [1 150] then pause,end

test = rand(150,1,166);
ierr = execstr("r = mean(test,3);","errcatch");
if ierr <> 0 then pause,end
if size(r) <> [1 150] then pause,end

test = rand(1000,1,165);
ierr = execstr("r = mean(test,3);","errcatch");
if ierr <> 0 then pause,end
if size(r) <> [1 1000] then pause,end

test = rand(1000,1,166);
ierr = execstr("r = mean(test,3);","errcatch");
if ierr <> 0 then pause,end
if size(r) <> [1 1000] then pause,end

test = rand(250,1,165);
ierr = execstr("r = mean(test,3);","errcatch");
if ierr <> 0 then pause,end
if size(r) <> [1 250] then pause,end

test = rand(250,1,166);
ierr = execstr("r = mean(test,3);","errcatch");
if ierr <> 0 then pause,end
if size(r) <> [1 250] then pause,end
