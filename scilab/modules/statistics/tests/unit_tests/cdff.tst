// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdff() function
// =============================================================================

// Run with test_run('statistics','cdff',['no_check_error_output']);

prec = 1.e-5;

f       = [1:2];
dfn     = [1:2];
dfd     = 2*dfn;

[P,Q]   = cdff("PQ",f,dfn,dfd);
[f1]    = cdff("F",dfn,dfd,P,Q);
[dfn1]  = cdff("Dfn",dfd,P,Q,f);
[dfd1]  = cdff("Dfd",P,Q,f,dfn);

if norm(f1-f)     > prec then pause,end
if norm(dfn1-dfn) > prec then pause,end
if norm(dfd1-dfd) > prec then pause,end

// IEEE support
// See http://bugzilla.scilab.org/show_bug.cgi?id=7296
dfn      = 1;
dfd      = 2;

f        = %inf; // Inf
[P,Q]    = cdff("PQ", f, dfn, dfd);
assert_checkequal(P, 1);
assert_checkequal(Q, 0);

f        = %nan; // NaN
[P,Q]    = cdff("PQ", f, dfn, dfd);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);
