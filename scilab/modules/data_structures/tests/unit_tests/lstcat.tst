// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

if or(lstcat(list(1,2),list(4))<>list(1,2,4))  then pause,end
if or(lstcat(list(1,2,4),list())<>list(1,2,4))  then pause,end
if or(lstcat(list(1),list(2,4))<>list(1,2,4))  then pause,end
if or(lstcat(list(),list(1,2,4))<>list(1,2,4))  then pause,end

if or(lstcat(list(1,2),4)<>list(1,2,4))  then pause,end
if or(lstcat(1,list(2,4))<>list(1,2,4)) then pause,end

if or(lstcat(list(1),list(2),list(4))<>list(1,2,4))  then pause,end

if or(lstcat(list(1,2),list(),4)<>list(1,2,4))  then pause,end

if or(lstcat(list([1 2],['my string';'matrix']),list('foo',%s))<>list([1 2],..
  ['my string';'matrix'],'foo',%s)) then pause,end
