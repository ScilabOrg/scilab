// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 3810 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3810
//
// <-- Short Description -->
// Insertion in string array and polynomial array may be very slow
w=warning('query');
warning('off');
T=[];
N=[ (5:9)*10000 (1:9)*100000];
for n=N
  clear t
  timer();t(1:n)=%s;T=[T timer()];
end
//check for quasi linear evolution of T(n)
abc=T/[N^2;N;ones(N)];
if abc(1)/norm(abc(2:3))>1d-3 then pause,end

T=[];
for n=N
  clear t
  timer();t(2,1:n)=%s;T=[T timer()];
end
//check for quasi linear evolution of T(n)
abc=T/[N^2;N;ones(N)];
if abc(1)/norm(abc(2:3))>1d-3 then pause,end

T=[];
for n=N
  clear t
  timer();t(1:n,2)=%s;T=[T timer()];
end
//check for quasi linear evolution of T(n)
abc=T/[N^2;N;ones(N)];
if abc(1)/norm(abc(2:3))>1d-3 then pause,end

T=[];
N=[ (5:9)*10000 (1:9)*100000];
for n=N
  clear t
  timer();t(n)=%s;T=[T timer()];
end
//check for quasi linear evolution of T(n)
abc=T/[N^2;N;ones(N)];
if abc(1)/norm(abc(2:3))>1d-3 then pause,end

T=[];
for n=N
  clear t
  timer();t(2,n)=%s;T=[T timer()];
end
//check for quasi linear evolution of T(n)
abc=T/[N^2;N;ones(N)];
if abc(1)/norm(abc(2:3))>1d-3 then pause,end

T=[];
for n=N
  clear t
  timer();t(n,2)=%s;T=[T timer()];
end
//check for quasi linear evolution of T(n)
abc=T/[N^2;N;ones(N)];
if abc(1)/norm(abc(2:3))>1d-3 then pause,end


warning(w);
