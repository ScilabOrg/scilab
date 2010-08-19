// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

// unit tests for structs
// =============================================================================

date_st=struct('jour',25,'mois','DEC','annee',2006);

if date_st.jour <> 25 then pause, end
if date_st.mois <> 'DEC' then pause, end
if date_st.annee <> 2006 then pause, end

date_st.jour=19;
if date_st.jour <> 19 then pause, end

date_st.mois='AOU';
if date_st.mois <> 'AOU' then pause, end

date_st.annee=1973;
if date_st.annee <> 1973 then pause, end

date_st.semaine=32;
if date_st.semaine <> 32 then pause, end

// Example from bug #7244
clear;
foo(1) = 1;
foo(2) = 2;
foo(3) = 3;
foo = foo([1 3]);
foo(3) = 2;
if [1; 3; 2] <> foo then pause, end

clear;
foo(1) = 1;
foo(2) = 2;
foo(3) = 3;
foo = foo([%T %F %T]);
foo(3) = 2; 
if [1; 3; 2] <> foo then pause, end

clear;
foo(1).bar = 1;
foo(2).bar = 2;
foo(3).bar = 3;
foo = foo([1 3]);
foo(3).bar = 2;
if foo(1).bar <> 1 then pause, end
if foo(2).bar <> 3 then pause, end
if foo(3).bar <> 2 then pause, end

out.a=1;
out.b=2;
if size(out,'*') <> 1 then pause, end

out(2,3).b=2; 
if size(out) <> [2 3] then pause, end

s=struct("txt","Hello","num",%pi,"pol",%z^2+1);
if s.pol <> %z^2+1 then pause, end
if s.txt <> "Hello" then pause, end

s.txt=null();s.num=null();s.pol=null();
if isfield( s , "txt"  ) then pause, end
if isfield( s , "num"  ) then pause, end
if isfield( s , "pol"  ) then pause, end
if isfield( s , ["pol", "num", "txt"]  ) then pause, end
if size(s) <> [1 1] then pause, end
