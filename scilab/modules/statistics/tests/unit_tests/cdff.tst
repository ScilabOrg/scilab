// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdff() function
// =============================================================================

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

// Dfd and Dfd should be integers
dfn      = [0.5:1.5];
dfd      = 2.5*dfn;
refMsg2 = msprintf(_("%s: Wrong value for input argument #%d: A matrix of integer value expected.\n"), "cdff", 2);
refMsg3 = msprintf(_("%s: Wrong value for input argument #%d: A matrix of integer value expected.\n"), "cdff", 3);
refMsg5 = msprintf(_("%s: Wrong value for input argument #%d: A matrix of integer value expected.\n"), "cdff", 5);
assert_checkerror("cdff(""PQ"",f,dfn,dfd);",refMsg3);
assert_checkerror("cdff(""F"",dfn,dfd,P,Q);",refMsg2);
assert_checkerror("cdff(""Dfn"",dfd,P,Q,f);",refMsg2);
assert_checkerror("cdff(""Dfd"",P,Q,f,dfn);",refMsg5);
