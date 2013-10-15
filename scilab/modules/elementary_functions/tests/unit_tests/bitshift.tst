// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

// Doubles
assert_checkequal(bitshift(2^10,  1), 2^11);
assert_checkequal(bitshift(2^10, -1), 2^9);

assert_checkequal(bitshift([2^10 2^10], [3 -3]), [2^13 2^7]);
assert_checkequal(bitshift([2^10 2^4], 3), [2^13 2^7]);
assert_checkequal(bitshift(2^10, [3 -3]), [2^13 2^7]);
assert_checkequal(bitshift([2^10 2^10; 3 4], [3 -3; -1 5]), [2^13 2^7; 1 2^7]);


// Integers, with default 'Wrap mode'
assert_checkequal(bitshift(int8(2^10-1),  int8(1)), int8(2^11-2));
assert_checkequal(bitshift(int8(2^10-1), int8(-1)), int8(2^9-1));

assert_checkequal(bitshift(int16([2^10 2^10]), int16([3 -3])), int16([2^13 2^7]));
assert_checkequal(bitshift(int16([2^10 2^4]), int16(3)), int16([2^13 2^7]));
assert_checkequal(bitshift(int32(2^10), int32([3 -3])), int32([2^13 2^7]));
assert_checkequal(bitshift(int32([2^10 2^10; 3 4]), int32([3 -3; -1 5])), int32([2^13 2^7; 1 2^7]));

assert_checkequal(bitshift(uint8(2^10),  uint8(1)), uint8(2^11));
assert_checkequal(bitshift(uint8(2^10), uint8(-1)), uint8(2^9));

assert_checkequal(bitshift(uint16([2^10 2^10]), uint16([3 2])), uint16([2^13 2^12]));
assert_checkequal(bitshift(uint16([2^10 2^4]), uint16(3)), uint16([2^13 2^7]));
assert_checkequal(bitshift(uint32(2^10), uint32([3 2])), uint32([2^13 2^12]));
assert_checkequal(bitshift(uint32([2^10 2^10; 3 4]), uint32([3 2; 1 5])), uint32([2^13 2^12; 6 2^7]));


// 'Drop' mode
assert_checkequal(bitshift(2^10,  1, "drop"), 2^11);
assert_checkequal(bitshift(2^10, -1, "drop"), 2^9);

assert_checkequal(bitshift([2^10 2^10], [3 -3], "drop"), [2^13 2^7]);
assert_checkequal(bitshift([2^10 2^4], 3, "drop"), [2^13 2^7]);
assert_checkequal(bitshift(2^10, [3 -3], "drop"), [2^13 2^7]);
assert_checkequal(bitshift([2^10 2^10; 3 4], [3 -3; -1 5], "drop"), [2^13 2^7; 1 2^7]);

assert_checkequal(bitshift(int8(65),  int8(1), "drop"), int8(127));
assert_checkequal(bitshift(-int8(65), int8(1), "drop"), -int8(128));

assert_checkequal(bitshift(int16([2e4 2e4]), int16([3 2]), "drop"), int16([32767 32767]));
assert_checkequal(bitshift(int16([-2e4 -2e4]), int16(3), "drop"), int16([-32768 -32768]));
assert_checkequal(bitshift(int32(-2^20), int32([3 -3]), "drop"), -int32([2^23 2^17]));
assert_checkequal(bitshift(int32([2^20 2^20; 3 4]), int32([3 -3; -1 5]), "drop"), int32([2^23 2^17; 1 2^7]));


// Error checks
refMsg = msprintf(_("%s: Wrong size for argument: Incompatible dimensions.\n"), "bitshift");
assert_checkerror("bitshift([2 2], [1 2 3])", refMsg);
assert_checkerror("bitshift([2 2; 2 2], [1 2])", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Same type as argument #%d expected.\n"), "bitshift", 2, 1);
assert_checkerror("bitshift([2 2], int32([1 2]))", refMsg);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: An integer value between %s and %s expected.\n"), "bitshift", 1, "-2147483648", "2147483647");
assert_checkerror("bitshift(2^33, 1)", refMsg);
