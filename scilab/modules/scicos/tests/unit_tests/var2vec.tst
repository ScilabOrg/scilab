// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = list("cos", [1.1 2]);
b = var2vec(a);
assert_checkequal(type(b), 1);
assert_checkequal(size(b), [12 1]);

// Double
a = [];
assert_checkequal(var2vec(a), [12; 2; 0; 0; 0]);
mat = [10 11 12 13; 14 15 16 17];
assert_checkequal(var2vec(mat), [12; 2; 2; 4; 0; 10; 14; 11; 15; 12; 16; 13; 17]);
hyperMat = hypermat([2, 2, 2], 1:8);
assert_checkequal(var2vec(hyperMat), [12; 3; 2; 2; 2; 0; (1:8)']);
cplx = hyperMat+hyperMat*%i;
assert_checkequal(var2vec(cplx), [12; 3; 2; 2; 2; 1; (1:8)'; (1:8)']);

// Integers
// int8
mat8 = int8(mat);
assert_checkequal(vec2var(var2vec(mat8)), mat8);
hyperMat8 = int8(hyperMat);
assert_checkequal(vec2var(var2vec(hyperMat8)), hyperMat8);
// uint8
umat8 = uint8(mat);
assert_checkequal(vec2var(var2vec(umat8)), umat8);
uhyperMat8 = uint8(hyperMat);
assert_checkequal(vec2var(var2vec(uhyperMat8)), uhyperMat8);
// int16
mat16 = int16(mat);
assert_checkequal(vec2var(var2vec(mat16)), mat16);
hyperMat16 = int16(hyperMat);
assert_checkequal(vec2var(var2vec(hyperMat16)), hyperMat16);
// uint16
umat16 = uint16(mat);
assert_checkequal(vec2var(var2vec(umat16)), umat16);
uhyperMat16 = uint16(hyperMat);
assert_checkequal(vec2var(var2vec(uhyperMat16)), uhyperMat16);
// int32
mat32 = int32(mat);
assert_checkequal(vec2var(var2vec(mat32)), mat32);
hyperMat32 = int32(hyperMat);
assert_checkequal(vec2var(var2vec(hyperMat32)), hyperMat32);
// uint32
umat32 = uint32(mat);
assert_checkequal(vec2var(var2vec(umat32)), umat32);
uhyperMat32 = uint32(hyperMat);
assert_checkequal(vec2var(var2vec(uhyperMat32)), uhyperMat32);

// Bool
bmat = (mat > 15);
assert_checkequal(vec2var(var2vec(bmat)), bmat);
bhyperMat = (hyperMat > 4);
assert_checkequal(vec2var(var2vec(bhyperMat)), bhyperMat);

// String
smat = string(mat);
assert_checkequal(vec2var(var2vec(smat)), smat);
shyperMat = string(hyperMat);
assert_checkequal(vec2var(var2vec(shyperMat)), shyperMat);
