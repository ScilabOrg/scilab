// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2616 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2616
//
// <-- Short Description -->
//    "eye variable undefined in this context" wrongly reported when a loop range is
//    defined concatenating ranges, inside a function

T=[];

//simple case uncompiled
s=0;for k=1:3,s=s+1;end
if s<>3 then pause,end


s=0;for k=(1:3)+1,s=s+1;end
if s<>3 then pause,end

s=0;for k=[(1:3) 1],s=s+1;end
if s<>4 then pause,end

function s=foo1(),s=0;for k=1:3,s=s+1;end,endfunction
if foo1()<>3 then pause,end

function s=foo2(),s=0;for k=(1:3)+1,s=s+1;end,endfunction
if foo2()<>3 then pause,end

function s=foo3(),s=0;for k=[(1:3) 1],s=s+1;end,endfunction
if foo3()<>4 then pause,end

//check if implicit  vector is created or not
old=stacksize();
stacksize(180000);

//should evaluate
ierr=execstr("s=0;for k=1:200000,s=s+1;end","errcatch");
if ierr<>0   then pause,end
if s<>200000 then pause,end

//should produce a stacksize error
ierr=execstr("s=0;for k=(1:200000)+1,s=s+1;end","errcatch");
if ierr<>17 then pause,end

ierr=execstr("s=0;for k=[(1:200000) 1],s=s+1;end","errcatch");
if ierr<>17 then pause,end

function s=foo4(),s=0;for k=1:200000,s=s+1;end,endfunction
ierr=execstr("s=foo4();","errcatch");
if ierr<>0   then pause,end
if s<>200000 then pause,end

function s=foo5(),s=0;for k=1+(1:200000),s=s+1;end,endfunction
ierr=execstr("s=foo5();","errcatch");
if ierr<>17 then pause,end

stacksize(old(1))
