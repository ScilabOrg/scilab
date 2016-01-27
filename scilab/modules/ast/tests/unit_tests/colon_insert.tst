// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===
//
//

// <-- CLI SHELL MODE -->

scalar = 5;
vect = 1:5;
vect2 = 6:10;
mat = [1,2,3,4;5,6,7,8];
hm = zeros(3,4,2); hm(:) = 1:24;

// scalar
clear a; a(:) = scalar;
assert_checkequal(a, scalar);

clear a; a(:,:) = scalar;
assert_checkequal(a, scalar);

clear a; a(:,:,:) = scalar;
assert_checkequal(a, scalar);

clear a; a(:,:,2) = scalar;
assert_checkequal(a, matrix([0;scalar], 1, 1, 2));

//scalar insert
a=scalar; a(:) = scalar;
assert_checkequal(a, scalar);

a=scalar; a(:,:) = scalar;
assert_checkequal(a, scalar);

a=scalar; a(:,:,2) = scalar;
assert_checkequal(a, matrix([scalar; scalar], 1, 1, 2));

//vector
a=vect; a(:) = scalar;
assert_checkequal(a, [scalar scalar scalar scalar scalar]);

a=vect; a(:,:) = scalar;
assert_checkequal(a, [scalar scalar scalar scalar scalar]);

a=vect; a(:,:,:) = scalar;
assert_checkequal(a, [scalar scalar scalar scalar scalar]);

a=vect; a(:,:,2) = scalar;
assert_checkequal(a, matrix([vect, scalar,scalar,scalar,scalar,scalar], 1, 5, 2));

//matrix
a=mat; a(:) = scalar;
assert_checkequal(a, matrix([scalar,scalar,scalar,scalar,scalar,scalar,scalar,scalar], 2, 4));

a=mat; a(:,:) = scalar;
assert_checkequal(a, matrix([scalar,scalar,scalar,scalar,scalar,scalar,scalar,scalar], 2, 4));

a=mat; a(:,:,:) = scalar;
assert_checkequal(a, matrix([scalar,scalar,scalar,scalar,scalar,scalar,scalar,scalar], 2, 4));

a=mat; a(:,:,2) = scalar;
assert_checkequal(a, matrix([mat(:)' scalar scalar scalar scalar scalar scalar scalar scalar], 2, 4, 2));

//hypermatrix
a=hm; a(:) = scalar;
assert_checkequal(a, matrix([scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar], 3, 4, 2));

a=hm; a(:,:) = scalar;
assert_checkequal(a, matrix([scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar], 3, 4, 2));

a=hm; a(:,:,:) = scalar;
assert_checkequal(a, matrix([scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar], 3, 4, 2));

a=hm; a(:,:,2) = scalar;
assert_checkequal(a, matrix([(hm(:,:,1)(:))' scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar scalar], 3, 4, 2));

//vector new insert
clear a; a(:) = vect;
assert_checkequal(a, vect);

clear a; a(:,:) = vect;
assert_checkequal(a, vect);

clear a; a(:,:,:) = vect;
assert_checkequal(a, vect);

clear a; a(:,:,2) = vect;
assert_checkequal(a, matrix([0 0 0 0 0 vect], 1, 5, 2));

clear a; a(:,:,:,2) = vect;
assert_checkequal(a, matrix([0 0 0 0 0 vect], 1, 5, 1, 2));

clear a; a(:) = vect';
assert_checkequal(a, vect');

clear a; a(:,:) = vect';
assert_checkequal(a, vect');

clear a; a(:,:,:) = vect';
assert_checkequal(a, vect');

clear a; a(:,:,2) = vect';
assert_checkequal(a, matrix([0 0 0 0 0 vect], 5, 1, 2));

clear a; a(:,:,:,2) = vect';
assert_checkequal(a, matrix([0 0 0 0 0 vect], 5, 1, 1, 2));

clear a; a(:,1,:) = vect;
assert_checkequal(a, matrix(vect, 1, 1, 5));

clear a; a(:,1,:) = vect';
assert_checkequal(a, vect');

a=vect; a(:) = vect2;
assert_checkequal(a, vect2);

a=vect; a(:,:) = vect2;
assert_checkequal(a, vect2);

a=vect; a(:,:,:) = vect2;
assert_checkequal(a, vect2);

a=vect; a(:,:,2) = vect2;
assert_checkequal(a, matrix([vect vect2], 1, 5, 2));

a=vect; a(:,:,:,2) = vect2;
assert_checkequal(a, matrix([vect vect2], 1, 5, 1, 2));

a=mat; a(:,:,2) = 9:16;
assert_checkequal(a, matrix([mat(:)', 9:16], 2, 4, 2));

a=mat; a(:,:,:,2) = 9:16;
assert_checkequal(a, matrix([mat(:)', 9:16], 2, 4, 1, 2));

a=hm; a(:) = hm(:);
assert_checkequal(a, hm);

a=hm; a(:,:) = hm(:);
assert_checkequal(a, hm);

a=hm; a(:,:,2) = hm(:,:,1);
assert_checkequal(a, matrix([hm(:,:,1)(:);hm(:,:,1)(:)], 3, 4, 2));

a=hm; a(:,:,:,2) = hm(:);
assert_checkequal(a, matrix([hm(:);hm(:)], 3,4,2,2));


//matrix
clear a; a(:) = mat;
assert_checkequal(a, mat);

clear a; a(:,:) = mat;
assert_checkequal(a, mat);

clear a; a(:,:,:) = mat;
assert_checkequal(a, mat);

clear a; a(:,:,2) = mat;
assert_checkequal(a, matrix([zeros(mat(:));mat(:)], 2, 4, 2));

clear a; a(:,:,:,2) = mat;
assert_checkequal(a, matrix([zeros(mat(:));mat(:)], 2, 4, 1, 2));

clear a; a(:,1,:) = mat;
assert_checkequal(a, matrix(mat, 2, 1, 4));

a=mat; a(:) = mat(:);
assert_checkequal(a, mat);

a=mat; a(:,:) = mat(:);
assert_checkequal(a, mat);

a=mat; a(:,:,2) = mat(:);
assert_checkequal(a, matrix([mat(:);mat(:)], 2, 4, 2));

a=mat; a(:,:,:,2) = mat(:)
assert_checkequal(a, matrix([mat(:);mat(:)], 2, 4, 1, 2));

//hypermatrix
clear a; a(:) = hm;
assert_checkequal(a, hm);

clear a; a(:,:) = hm;
assert_checkequal(a, hm);

clear a; a(:,:,:) = hm;
assert_checkequal(a, hm);

clear a; a(:,:,:, 2) = hm;
assert_checkequal(a, matrix([zeros(hm)(:);hm(:)], 3, 4, 2, 2));

clear a; a(:,1,:,:) = hm;
assert_checkequal(a, matrix(hm, 3, 1, 4, 2));
