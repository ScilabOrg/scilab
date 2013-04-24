// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- JVM NOT MANDATORY -->

false = %f;
true = %t;


//scalar AND scalar in operation
assert_checkfalse(false & false);
assert_checkfalse(false & true);
assert_checkfalse(true & false);
assert_checktrue(true & true);

//scalar OR scalar in operation
assert_checkfalse(false | false);
assert_checktrue(false | true);
assert_checktrue(true | false);
assert_checktrue(true | true);

//scalar AND scalar in condition
if (false & false) <> false then pause; end
if (false & true) <> false then pause; end
if (true & false) <> false then pause; end
if (true & true) <> true then pause; end

//scalar OR scalar in condition
if (false | false) <> false then pause; end
if (false | true) <> true then pause; end
if (true | false) <> true then pause; end
if (true | true) <> true then pause; end


MF = [false,false;false,false];
MT = [true,true;true,true];

//scalar AND matrix in operation
assert_checkfalse(false & MF);
assert_checkfalse(false & MT);
assert_checkfalse(true & MF);
assert_checktrue(true & MT);

//scalar OR matrix in operation
assert_checkfalse(false | MF);
assert_checktrue(false | MT);
assert_checktrue(true | MF);
assert_checktrue(true | MT);

//scalar AND matrix in condition
if (false & MF) <> false then pause; end
if (false & MT) <> false then pause; end
if (true & MF) <> false then pause; end
if (true & MT) <> true then pause; end

//scalar OR matrix in condition
if (false | MF) <> false then pause; end
if (false | MT) <> true then pause; end
if (true | MF) <> true then pause; end
if (true | MT) <> true then pause; end

//matrix AND scalar in operation
assert_checkfalse(MF & false);
assert_checkfalse(MF & true);
assert_checkfalse(MT & false);
assert_checktrue(MT & true);

//matrix OR scalar in operation
assert_checkfalse(MF | false);
assert_checktrue(MF | true);
assert_checktrue(MT | false);
assert_checktrue(MT | true);

//matrix AND scalar in condition
if (MF & false) <> false then pause; end
if (MF & true) <> false then pause; end
if (MT & false) <> false then pause; end
if (MT & true) <> true then pause; end

//matrix OR scalar in condition
if (MF | false) <> false then pause; end
if (MF | true) <> true then pause; end
if (MT | false) <> true then pause; end
if (MT | true) <> true then pause; end

//matrix AND matrix in operation
assert_checkfalse(MF & MF);
assert_checkfalse(MF & MT);
assert_checkfalse(MT & MF);
assert_checktrue(MT & MT);

//matrix OR matrix in operation
assert_checkfalse(MF | MF);
assert_checktrue(MF | MT);
assert_checktrue(MT | MF);
assert_checktrue(MT | MT);

//matrix AND matrix in condition
if (MF & MF) <> MF then pause; end
if (MF & MT) <> MF then pause; end
if (MT & MF) <> MF then pause; end
if (MT & MT) <> MT then pause; end

//matrix OR matrix in condition
if (MF | MF) <> MF then pause; end
if (MF | MT) <> MT then pause; end
if (MT | MF) <> MT then pause; end
if (MT | MT) <> MT then pause; end


//int | int
b0 = int8(0);
b1 = int8(1);
b2 = int8(2);
b3 = int8(4);
b4 = int8(8);

b12 = int8(3);
b13 = int8(5);
b14 = int8(9);
b23 = int8(6);
b24 = int8(10);
b34 = int8(12);

b123 = int8(7);
b124 = int8(11);
b134 = int8(13);
b234 = int8(14);
b1234 = int8(15);

//scalar OR scalar in operation
assert_checktrue((b0 | b0) == b0);
assert_checktrue((b0 | b1) == b1);
assert_checktrue((b0 | b2) == b2);
assert_checktrue((b0 | b3) == b3);
assert_checktrue((b0 | b4) == b4);
assert_checktrue((b0 | b12) == b12);
assert_checktrue((b0 | b13) == b13);
assert_checktrue((b0 | b14) == b14);
assert_checktrue((b0 | b23) == b23);
assert_checktrue((b0 | b24) == b24);
assert_checktrue((b0 | b34) == b34);
assert_checktrue((b0 | b123) == b123);
assert_checktrue((b0 | b134) == b134);
assert_checktrue((b0 | b234) == b234);
assert_checktrue((b0 | b1234) == b1234);

assert_checktrue((b1 | b1) == b1);
assert_checktrue((b1 | b2) == b12);
assert_checktrue((b1 | b3) == b13);
assert_checktrue((b1 | b4) == b14);
assert_checktrue((b1 | b12) == b12);
assert_checktrue((b1 | b13) == b13);
assert_checktrue((b1 | b14) == b14);
assert_checktrue((b1 | b23) == b123);
assert_checktrue((b1 | b24) == b124);
assert_checktrue((b1 | b34) == b134);
assert_checktrue((b1 | b123) == b123);
assert_checktrue((b1 | b134) == b134);
assert_checktrue((b1 | b234) == b1234);
assert_checktrue((b1 | b1234) == b1234);

assert_checktrue((b2 | b2) == b2);
assert_checktrue((b2 | b3) == b23);
assert_checktrue((b2 | b4) == b24);
assert_checktrue((b2 | b12) == b12);
assert_checktrue((b2 | b13) == b123);
assert_checktrue((b2 | b14) == b124);
assert_checktrue((b2 | b23) == b23);
assert_checktrue((b2 | b24) == b24);
assert_checktrue((b2 | b34) == b234);
assert_checktrue((b2 | b123) == b123);
assert_checktrue((b2 | b134) == b1234);
assert_checktrue((b2 | b234) == b234);
assert_checktrue((b2 | b1234) == b1234);

assert_checktrue((b3 | b3) == b3);
assert_checktrue((b3 | b4) == b34);
assert_checktrue((b3 | b12) == b123);
assert_checktrue((b3 | b13) == b13);
assert_checktrue((b3 | b14) == b134);
assert_checktrue((b3 | b23) == b23);
assert_checktrue((b3 | b24) == b234);
assert_checktrue((b3 | b34) == b34);
assert_checktrue((b3 | b123) == b123);
assert_checktrue((b3 | b134) == b134);
assert_checktrue((b3 | b234) == b234);
assert_checktrue((b3 | b1234) == b1234);

assert_checktrue((b4 | b4) == b4);
assert_checktrue((b4 | b12) == b124);
assert_checktrue((b4 | b13) == b134);
assert_checktrue((b4 | b14) == b14);
assert_checktrue((b4 | b23) == b234);
assert_checktrue((b4 | b24) == b24);
assert_checktrue((b4 | b34) == b34);
assert_checktrue((b4 | b123) == b1234);
assert_checktrue((b4 | b134) == b134);
assert_checktrue((b4 | b234) == b234);
assert_checktrue((b4 | b1234) == b1234);

assert_checktrue((b12 | b12) == b12);
assert_checktrue((b12 | b13) == b123);
assert_checktrue((b12 | b14) == b124);
assert_checktrue((b12 | b23) == b123);
assert_checktrue((b12 | b24) == b124);
assert_checktrue((b12 | b34) == b1234);
assert_checktrue((b12 | b123) == b123);
assert_checktrue((b12 | b134) == b1234);
assert_checktrue((b12 | b234) == b1234);
assert_checktrue((b12 | b1234) == b1234);

assert_checktrue((b13 | b13) == b13);
assert_checktrue((b13 | b14) == b134);
assert_checktrue((b13 | b23) == b123);
assert_checktrue((b13 | b24) == b1234);
assert_checktrue((b13 | b34) == b134);
assert_checktrue((b13 | b123) == b123);
assert_checktrue((b13 | b134) == b134);
assert_checktrue((b13 | b234) == b1234);
assert_checktrue((b13 | b1234) == b1234);

assert_checktrue((b14 | b14) == b14);
assert_checktrue((b14 | b23) == b1234);
assert_checktrue((b14 | b24) == b124);
assert_checktrue((b14 | b34) == b134);
assert_checktrue((b14 | b123) == b1234);
assert_checktrue((b14 | b134) == b134);
assert_checktrue((b14 | b234) == b1234);
assert_checktrue((b14 | b1234) == b1234);

assert_checktrue((b23 | b23) == b23);
assert_checktrue((b23 | b24) == b234);
assert_checktrue((b23 | b34) == b234);
assert_checktrue((b23 | b123) == b123);
assert_checktrue((b23 | b134) == b1234);
assert_checktrue((b23 | b234) == b234);
assert_checktrue((b23 | b1234) == b1234);

assert_checktrue((b24 | b24) == b24);
assert_checktrue((b24 | b34) == b234);
assert_checktrue((b24 | b123) == b1234);
assert_checktrue((b24 | b134) == b1234);
assert_checktrue((b24 | b234) == b234);
assert_checktrue((b24 | b1234) == b1234);

assert_checktrue((b34 | b34) == b34);
assert_checktrue((b34 | b123) == b1234);
assert_checktrue((b34 | b134) == b134);
assert_checktrue((b34 | b234) == b234);
assert_checktrue((b34 | b1234) == b1234);

assert_checktrue((b123 | b123) == b123);
assert_checktrue((b123 | b134) == b1234);
assert_checktrue((b123 | b234) == b1234);
assert_checktrue((b123 | b1234) == b1234);

assert_checktrue((b134 | b134) == b134);
assert_checktrue((b134 | b234) == b1234);
assert_checktrue((b134 | b1234) == b1234);

assert_checktrue((b234 | b234) == b234);
assert_checktrue((b234 | b1234) == b1234);

assert_checktrue((b1234 | b1234) == b1234);

//scalar AND scalar in operation
assert_checktrue((b0 & b0) == b0);
assert_checktrue((b0 & b1) == b0);
assert_checktrue((b0 & b2) == b0);
assert_checktrue((b0 & b3) == b0);
assert_checktrue((b0 & b4) == b0);
assert_checktrue((b0 & b12) == b0);
assert_checktrue((b0 & b13) == b0);
assert_checktrue((b0 & b14) == b0);
assert_checktrue((b0 & b23) == b0);
assert_checktrue((b0 & b24) == b0);
assert_checktrue((b0 & b34) == b0);
assert_checktrue((b0 & b123) == b0);
assert_checktrue((b0 & b134) == b0);
assert_checktrue((b0 & b234) == b0);
assert_checktrue((b0 & b1234) == b0);

assert_checktrue((b1 & b1) == b1);
assert_checktrue((b1 & b2) == b0);
assert_checktrue((b1 & b3) == b0);
assert_checktrue((b1 & b4) == b0);
assert_checktrue((b1 & b12) == b1);
assert_checktrue((b1 & b13) == b1);
assert_checktrue((b1 & b14) == b1);
assert_checktrue((b1 & b23) == b0);
assert_checktrue((b1 & b24) == b0);
assert_checktrue((b1 & b34) == b0);
assert_checktrue((b1 & b123) == b1);
assert_checktrue((b1 & b134) == b1);
assert_checktrue((b1 & b234) == b0);
assert_checktrue((b1 & b1234) == b1);

assert_checktrue((b2 & b2) == b2);
assert_checktrue((b2 & b3) == b0);
assert_checktrue((b2 & b4) == b0);
assert_checktrue((b2 & b12) == b2);
assert_checktrue((b2 & b13) == b0);
assert_checktrue((b2 & b14) == b0);
assert_checktrue((b2 & b23) == b2);
assert_checktrue((b2 & b24) == b2);
assert_checktrue((b2 & b34) == b0);
assert_checktrue((b2 & b123) == b2);
assert_checktrue((b2 & b134) == b0);
assert_checktrue((b2 & b234) == b2);
assert_checktrue((b2 & b1234) == b2);

assert_checktrue((b3 & b3) == b3);
assert_checktrue((b3 & b4) == b0);
assert_checktrue((b3 & b12) == b0);
assert_checktrue((b3 & b13) == b3);
assert_checktrue((b3 & b14) == b0);
assert_checktrue((b3 & b23) == b3);
assert_checktrue((b3 & b24) == b0);
assert_checktrue((b3 & b34) == b3);
assert_checktrue((b3 & b123) == b3);
assert_checktrue((b3 & b134) == b3);
assert_checktrue((b3 & b234) == b3);
assert_checktrue((b3 & b1234) == b3);

assert_checktrue((b4 & b4) == b4);
assert_checktrue((b4 & b12) == b0);
assert_checktrue((b4 & b13) == b0);
assert_checktrue((b4 & b14) == b4);
assert_checktrue((b4 & b23) == b0);
assert_checktrue((b4 & b24) == b4);
assert_checktrue((b4 & b34) == b4);
assert_checktrue((b4 & b123) == b0);
assert_checktrue((b4 & b134) == b4);
assert_checktrue((b4 & b234) == b4);
assert_checktrue((b4 & b1234) == b4);

assert_checktrue((b12 & b12) == b12);
assert_checktrue((b12 & b13) == b1);
assert_checktrue((b12 & b14) == b1);
assert_checktrue((b12 & b23) == b2);
assert_checktrue((b12 & b24) == b2);
assert_checktrue((b12 & b34) == b0);
assert_checktrue((b12 & b123) == b12);
assert_checktrue((b12 & b134) == b1);
assert_checktrue((b12 & b234) == b2);
assert_checktrue((b12 & b1234) == b12);

assert_checktrue((b13 & b13) == b13);
assert_checktrue((b13 & b14) == b1);
assert_checktrue((b13 & b23) == b3);
assert_checktrue((b13 & b24) == b0);
assert_checktrue((b13 & b34) == b3);
assert_checktrue((b13 & b123) == b13);
assert_checktrue((b13 & b134) == b13);
assert_checktrue((b13 & b234) == b3);
assert_checktrue((b13 & b1234) == b13);

assert_checktrue((b14 & b14) == b14);
assert_checktrue((b14 & b23) == b0);
assert_checktrue((b14 & b24) == b4);
assert_checktrue((b14 & b34) == b4);
assert_checktrue((b14 & b123) == b1);
assert_checktrue((b14 & b134) == b14);
assert_checktrue((b14 & b234) == b4);
assert_checktrue((b14 & b1234) == b14);

assert_checktrue((b23 & b23) == b23);
assert_checktrue((b23 & b24) == b2);
assert_checktrue((b23 & b34) == b3);
assert_checktrue((b23 & b123) == b23);
assert_checktrue((b23 & b134) == b3);
assert_checktrue((b23 & b234) == b23);
assert_checktrue((b23 & b1234) == b23);

assert_checktrue((b24 & b24) == b24);
assert_checktrue((b24 & b34) == b4);
assert_checktrue((b24 & b123) == b2);
assert_checktrue((b24 & b134) == b4);
assert_checktrue((b24 & b234) == b24);
assert_checktrue((b24 & b1234) == b24);

assert_checktrue((b34 & b34) == b34);
assert_checktrue((b34 & b123) == b3);
assert_checktrue((b34 & b134) == b34);
assert_checktrue((b34 & b234) == b34);
assert_checktrue((b34 & b1234) == b34);

assert_checktrue((b123 & b123) == b123);
assert_checktrue((b123 & b134) == b13);
assert_checktrue((b123 & b234) == b23);
assert_checktrue((b123 & b1234) == b123);

assert_checktrue((b134 & b134) == b134);
assert_checktrue((b134 & b234) == b34);
assert_checktrue((b134 & b1234) == b134);

assert_checktrue((b234 & b234) == b234);
assert_checktrue((b234 & b1234) == b234);

assert_checktrue((b1234 & b1234) == b1234);

//scalar OR scalar in condition
if b0 | b0 then pause; end

//others case returns true and we do not want to test with '=='

//scalar AND scalar in condition
if b0 & b0 then pause; end
if b0 & b1 then pause; end
if b0 & b2 then pause; end
if b0 & b3 then pause; end
if b0 & b4 then pause; end
if b0 & b12 then pause; end
if b0 & b13 then pause; end
if b0 & b14 then pause; end
if b0 & b23 then pause; end
if b0 & b24 then pause; end
if b0 & b34 then pause; end
if b0 & b123 then pause; end
if b0 & b124 then pause; end
if b0 & b134 then pause; end
if b0 & b234 then pause; end
if b0 & b1234 then pause; end
//others case returns true and we do not want to test with '=='
