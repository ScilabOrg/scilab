// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- JVM NOT MANDATORY -->
// <-- NO CHECK REF -->
// =============================================================================
assert_checkequal (csv_isnum('%pi'), %t);
assert_checkequal (csv_isnum('1'), %t);
assert_checkequal (csv_isnum('1.2'), %t);
assert_checkequal (csv_isnum('+1.2'), %t);
assert_checkequal (csv_isnum('-1.2'), %t);
assert_checkequal (csv_isnum('1e2'), %t);
if getos() == "Windows" then
   assert_checkequal (csv_isnum('1d2'), %t);
end
assert_checkequal (csv_isnum('1E2'), %t);
if getos() == "Windows" then
   assert_checkequal (csv_isnum('1D2'), %t);
end
assert_checkequal (csv_isnum('1e+2'), %t);
assert_checkequal (csv_isnum('1e-2'), %t);
assert_checkequal (csv_isnum('+1e+2'), %t);
assert_checkequal (csv_isnum('-1e+2'), %t);
assert_checkequal (csv_isnum('-12e+3'), %t);
assert_checkequal (csv_isnum('-1.2e+3'), %t);
assert_checkequal (csv_isnum('-1.2e+345'), %t);
assert_checkequal (csv_isnum('a'), %f);
assert_checkequal (csv_isnum('e'), %f);
assert_checkequal (csv_isnum('d'), %f);
assert_checkequal (csv_isnum('E'), %f);
assert_checkequal (csv_isnum('e'), %f);
assert_checkequal (csv_isnum('-1.2a+345'), %f);
assert_checkequal (csv_isnum('-1.2e+3+45'), %f);
assert_checkequal (csv_isnum('-1.2e+3-45'), %f);
// =============================================================================



