// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 10782 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14077
//
// <-- Short Description -->
// int8, int16 and int32 does not convert properly big integers
//

big_double = 2^32 + 1; // This is 1 as int8 int16,int32 signed or unsigned
assert_checkequal(int8(big_double), int8(1));
assert_checkequal(uint8(big_double), uint8(1));
assert_checkequal(int16(big_double), int16(1));
assert_checkequal(uint16(big_double), uint16(1));
assert_checkequal(int32(big_double), int32(1));
assert_checkequal(uint32(big_double), uint32(1));

int_calc = int8(3)^20;
int_ref = int8(3)^10 * int8(3)^10;
assert_checkequal(int_calc, int_ref);

int_calc = int16(3)^20;
int_ref = int16(3)^10 * int16(3)^10;
assert_checkequal(int_calc, int_ref);

int_calc = int32(3)^20;
int_ref = int32(3)^10 * int32(3)^10;
assert_checkequal(int_calc, int_ref);

int_calc = uint8(3)^20;
int_ref = uint8(3)^10 * uint8(3)^10;
assert_checkequal(int_calc, int_ref);

int_calc = uint16(3)^20;
int_ref = uint16(3)^10 * uint16(3)^10;
assert_checkequal(int_calc, int_ref);

int_calc = uint32(3)^20;
int_ref = uint32(3)^10 * uint32(3)^10;
assert_checkequal(int_calc, int_ref);
