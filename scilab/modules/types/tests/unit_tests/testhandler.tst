// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

here=pwd();
compiledir=SCI+"/modules/types/tests/utilities/";
build="<project><target name=""compile""><javac srcdir="""+compiledir+""" destdir="""+compiledir+""" classpath="""+SCI+"/modules/types/jar/org.scilab.modules.types.jar"+""" /></target></project>"
mputl(build,compiledir+"build.xml")
cd(compiledir);
unix("ant compile");
cd(here);

ilib_verbose(0);
ierr = exec(SCI+"/modules/types/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(SCI+"/modules/types/tests/utilities/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

javaclasspath(SCI+"/modules/types/tests/utilities/");

M=11;
N=7;

// Double
A=rand(M,N);
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Complex
A=rand(M,N)+%i*rand(M,N);
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Double polynomial
A=rand(M,N)+%s*rand(M,N)+%s^2*rand(M,N)+%s^3*rand(M,N);
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Complex polynomial
A=rand(M,N)+%s*rand(M,N)+%s^2*rand(M,N)+%s^3*rand(M,N) + %i*(rand(M,N)+%s*rand(M,N)+%s^2*rand(M,N)+%s^3*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Int8
A=int8(10*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Int16
A=int16(10*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Int32
A=int32(10*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// UInt8
A=uint8(10*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// UInt16
A=uint16(10*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// UInt32
A=uint32(10*rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Boolean
A=rand(M,N) <= 0.5;
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Double sparse
A=sprand(M,N,0.2);
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Complex sparse
A=sprand(M,N,0.2)+%i*sprand(M,N,0.3);
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// Boolean sparse
A=sprand(M,N,0.2) <= 0.5;
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// String
A=string(rand(M,N));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// List
A=list(string(rand(M,N)),rand(M,N),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// MList
if ~isdef("%a_size")
   function y=%a_size(x), y=length(x), endfunction
end
A=mlist(["a" "b" "c" "d" "e"],string(rand(M,N)),mlist(["a" "b" "c" "d" "e"],string(rand(M,N)),rand(M,N),mlist(["a" "b" "c" "d" "e"],string(rand(M,N)),rand(M,N),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5)),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5)),mlist(["a" "b" "c" "d" "e"],string(rand(M,N)),mlist(["a" "b" "c" "d" "e"],string(rand(M,N)),rand(M,N),mlist(["a" "b" "c" "d" "e"],string(rand(M,N)),rand(M,N),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5)),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5)),rand(M,N),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5)),rand(M,N),int8(rand(M,N)),rand(M,N)+%s*rand(M,N)+%s^2*rand(M,N)+%s^3*rand(M,N) + %i*(rand(M,N)+%s*rand(M,N)+%s^2*rand(M,N)+%s^3*rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5));
execstr("B="+testHandler(A));
assert_checkequal(B,A);

// TList
if ~isdef("%a_size")
   function y=%a_size(x), y=length(x), endfunction
end
A=tlist(["a" "b" "c" "d" "e"],string(rand(M,N)),rand(M,N),int8(rand(M,N)),list(sprand(M,N,0.2),rand(M,N)<=0.5));
execstr("B="+testHandler(A));
assert_checkequal(B,A);
