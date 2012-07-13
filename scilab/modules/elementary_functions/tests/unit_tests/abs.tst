// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for abs() function (absolute value, magnitude)
// =============================================================================

if execstr("abs()"   ,"errcatch") == 0 then pause, end
if execstr("abs(1,2)","errcatch") == 0 then pause, end

if abs(10)     <> 10  then pause, end
if abs(-4.3)   <> 4.3 then pause, end
if abs(6+8*%i) <> 10  then pause, end
if abs(6-8*%i) <> 10  then pause, end

if or(abs([ 7.6 , 8*%i ; 6+8*%i 6-8*%i ])   <> [7.6 , 8 ; 10 , 10 ]) then pause, end
if or(abs([ 7.6 8*%i 6+8*%i 6-8*%i ])       <> [7.6 8 10 10 ])       then pause, end
if or(abs([ 7.6 ; 8*%i ; 6+8*%i ; 6-8*%i ]) <> [7.6 ; 8 ; 10 ; 10 ]) then pause, end


if ~isnan(abs(%nan)) then pause, end
if ~isnan(abs(-%nan)) then pause, end

if abs(%inf) <> %inf then pause, end
if abs(-%inf) <> %inf then pause, end
