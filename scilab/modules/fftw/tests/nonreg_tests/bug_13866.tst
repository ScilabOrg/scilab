// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13866 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13866
//
// <-- Short Description -->
// weird behaviour of fft(real(S))

s=zeros(100,1);
s(4)=1;
S=fft(s,-1);
CC=cos(2*%pi*[0:99]'*3/100);
CS=-sin(2*%pi*[0:99]'*3/100);
assert_checkalmostequal(real(S), CC, [], 10*%eps);

// With fresh plan creation
fft([1;0;0],-1); // Force the creation of a new plan at next line
rs=fft(real(S),-1);
is=fft(imag(S),-1);

assert_checkalmostequal(real(rs), real(fft(CC,-1)), [], 100*%eps);

rs2=fft(real(S),-1,dim=100,incr=1); // dim + incr forced
assert_checkequal(real(rs), real(rs2));

assert_checkalmostequal(imag(is), imag(fft(CS,-1)), [], 100*%eps);


// Without fresh plan creation (1)
fft(eye(100,1),-1); // Plan with same size => no new plan creation at next line
rs=fft(real(S),-1);
is=fft(imag(S),-1);

assert_checkalmostequal(real(rs), real(fft(CC,-1)), [], 100*%eps);

rs2=fft(real(S),-1,dim=100,incr=1); // dim + incr forced
assert_checkequal(real(rs), real(rs2));

assert_checkalmostequal(imag(is), imag(fft(CS,-1)), [], 100*%eps);


// Without fresh plan creation (2)
rs=fft(real(S),-1);
is=fft(imag(S),-1);

assert_checkalmostequal(real(rs), real(fft(CC,-1)), [], 100*%eps);

rs2=fft(real(S),-1,dim=100,incr=1); // dim + incr forced
assert_checkequal(real(rs), real(rs2));

assert_checkalmostequal(imag(is), imag(fft(CS,-1)), [], 100*%eps);

// convol test
t=rand(1,100);
r=convol(eye(50,1)',t);
assert_checkalmostequal(t(1:10),r(1:10), [], 100*%eps);