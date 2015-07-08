// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2009 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- CLI SHELL MODE -->

// unit tests for fieldnames function
// =============================================================================

assert_checkequal(fieldnames(1), []);

my_struct = struct("field_1", 123, "field_2", 456);
assert_checkequal(fieldnames(my_struct), ["field_1"; "field_2"]);

clear T;
T.a = rand(2, 2);
T.b = "s";
assert_checkequal(fieldnames(T), ["a"; "b"]);


assert_checkequal(fieldnames(1/%s), ["num"; "den"; "dt"]);

M = mlist(["foo" "A" "B" "C"], [], [], []);
assert_checkequal(fieldnames(M), ["A"; "B"; "C"]);


// Scicos objects
d = scicos_diagram();
assert_checkequal(fieldnames(d), ["props"; "objs"; "version"; "contrib"]);
b = scicos_block();
assert_checkequal(fieldnames(b), ["graphics"; "model"; "gui"; "doc"]);
l = scicos_link();
assert_checkequal(fieldnames(l), ["xx"; "yy"; "id"; "thick"; "ct"; "from"; "to"]);
