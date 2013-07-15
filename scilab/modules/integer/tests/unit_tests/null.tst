// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
A=int8(255*rand(23,47));
assert_checkequal(A, A(null()));

A=uint8(255*rand(23,47));
assert_checkequal(A, A(null()));

A=int16(65535*rand(23,47));
assert_checkequal(A, A(null()));

A=uint16(65535*rand(23,47));
assert_checkequal(A, A(null()));

A=int32(4294967295*rand(23,47));
assert_checkequal(A, A(null()));

A=uint32(4294967295*rand(23,47));
assert_checkequal(A, A(null()));