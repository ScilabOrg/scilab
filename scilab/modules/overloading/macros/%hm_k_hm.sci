// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// The Kronecker product of two hypermatrices is defined as such
// Let hypermatrix M1 be of size m1_1, m1_2, ... , m1_p (a p-dimension hypermatrix)
// Let hypermatrix M2 be of size m2_1, m2_2, ... , m2_q (a q-dimension hypermatrix)
// The resulting matrix is :
//    M(1:m2_1, 1:m2_2, ... , 1:m2_q) =  M1(1, 1, ... , 1) * M2
//    M((m2_1 + 1):(2 * m2_1), 1:m2_2, ... , 1:m2_q) = M1(2, 1, ... , 1) * M2
//    .
//    .
//    .
//    M((i*m2_1 + 1):((i + 1) * m2), 1:m2_2, ... 1:m2_q) = M1(i, 1, ..., 1) * M2
//    .
//    .
//    .
//    M(((m1_1 - 1) * m2_1 + 1):(m1_1 * m2_1), 1:m2_2, ... , 1:m2_q) = M1(m1_1, 1, ... , 1) * M2
//
// And so on for each dimension
// This implies that the size of M is (m1_1 * m2_1, ... , m1_p * m2_q) and thus p = q
// As hypermatrices will be of different dimensions p and q, they need to be resized to be of same dimension N = max(p, q)
// This resize is only important for M1, if p < q define M1(i_1, ... , i_(m1_p), 1, 1, ... 1) = M1(i_1, ... , i_(m1_p))
//
// If p > q there is no issue as M1(i_1, ... , i_(m1_p)) is always defined 

function M = %hm_k_hm(M1, M2)
    hm1size = double(M1.dims);
    hm2size = double(M2.dims);
    hm1size = [hm1size ones(1, length(hm2size)-length(hm1size))];
    hm2size = [hm2size ones(1, length(hm1size)-length(hm2size))];

    // M1 is reshaped to fit the dimension with ones added
    M1 = matrix(M1, hm1size);

    // The size of the resulting matrix must be the product of each dimension of the argument matrices
    hmMsize = hm1size .* hm2size;
    M = hypermat(hmMsize);

    // Iterate over elements of M1
    for i = 1:prod(hm1size)
        idx = ind2sub(hm1size, i); //returns the vector corresponding to position in M1
        idx_low = string((idx - 1) .* hm2size + 1);
        idx_high = string(idx .* hm2size);
        idx_strmatrix = idx_low + ":" + idx_high; // idx_strmatrix contains the ranges of size M2 at positions corresponding to M1
        idx_str = "";
        for j = 1:length(hm1size) - 1
            idx_str = idx_str + idx_strmatrix(j) + ", "; //creates the index range on matrix M
        end
        idx_str = idx_str + idx_strmatrix($);
        execstr("M(" + idx_str + ") = M1(i) * M2"); // Tensor product in the exact space occupied by M2
    end
endfunction
