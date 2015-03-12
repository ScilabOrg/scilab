// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Computed non-numbers for integer values
//
// Rational : double operations should be preserved
//   v + %nan == %nan
//   v * %nan == %nan
//   v + %inf == %inf
//   v - %inf == -%inf
//   %inf - %inf == %nan
//   v * %inf == %inf
//   %inf / %inf error or %nan with ieee(2) ; there is no valid results for int

assert_checkequal(double(int8(%nan)), 0);
assert_checkequal(double(int16(%nan)), 0);
assert_checkequal(double(int32(%nan)), 0);
assert_checkequal(double(uint8(%nan)), 0);
assert_checkequal(double(uint16(%nan)), 0);
assert_checkequal(double(uint32(%nan)), 0);

assert_checkequal(double(int8(%inf)), 0);
assert_checkequal(double(int16(%inf)), 0);
assert_checkequal(double(int32(%inf)), 0);
assert_checkequal(double(uint8(%inf)), 0);
assert_checkequal(double(uint16(%inf)), 0);
assert_checkequal(double(uint32(%inf)), 0);

assert_checkequal(double(int8(-%inf)), 0);
assert_checkequal(double(int16(-%inf)), 0);
assert_checkequal(double(int32(-%inf)), 0);
assert_checkequal(double(uint8(-%inf)), 0);
assert_checkequal(double(uint16(-%inf)), 0);
assert_checkequal(double(uint32(-%inf)), 0);

