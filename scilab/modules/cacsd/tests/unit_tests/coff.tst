// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

M=[1,2;0,3];
[N,d]=coff(M);
if norm(coeff(N-[-3+%s,2;0,-1+%s]))>1d-14 then pause,end
if norm(coeff(d-(3-4*%s+%s^2)))>1d-14 then pause,end
[N,d]=coff([]);
if N<>[]|d<>1 then pause,end
