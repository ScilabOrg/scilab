// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// <-- JVM NOT MANDATORY -->

function checkCallOverload(mat)
    assert_checkerror(mat, [], 144);
endfunction

// normal cases
assert_checkequal(1:3, [1 2 3]);
assert_checkequal(1:1:3, [1 2 3]);
assert_checkequal(1:2:3, [1 3]);

assert_checkequal(3:-1:1, [3 2 1]);
assert_checkequal(3:-2:1, [3 1]);

assert_checkequal(3:1, []);
assert_checkequal(3:2:1, []);
assert_checkequal(1:0:3, []);

// no finite cases
assert_checkequal(1:%inf, %nan);
assert_checkequal(1:%nan, %nan);
assert_checkequal(1:-%inf, []);
assert_checkequal(1:%inf:10, %nan);
assert_checkequal(1:%nan:10, %nan);
assert_checkequal(1:-%inf:10, []);

assert_checkequal(%inf:1, []);
assert_checkequal(%nan:1, %nan);
assert_checkequal(-%inf:1, %nan);
assert_checkequal(10:%inf:1, []);
assert_checkequal(10:%nan:1, %nan);
assert_checkequal(10:-%inf:1, %nan);

// int
i1 = int8(1);
i2 = int8(2);
i8 = int8(8);

assert_checkequal(i1:i2:i8, int8([1,3,5,7]));
checkCallOverload("i1:i2:8");
assert_checkequal(i1:2:i8,  int8([1,3,5,7]));
assert_checkequal(i1:2:8,   int8([1,3,5,7]));
assert_checkequal(1:i2:i8,  int8([1,3,5,7]));
assert_checkequal(1:i2:8,   int8([1,3,5,7]));
assert_checkequal(1:2:i8,   int8([1,3,5,7]));
assert_checkequal(1:2:8,    [1,3,5,7]);

assert_checkequal(i1:i8, int8(1:8));
assert_checkequal(i1:8,  int8(1:8));
assert_checkequal(1:i8,  int8(1:8));
assert_checkequal(1:8,   [1 2 3 4 5 6 7 8]);

ii1 = int16(1);
ii2 = int16(2);
ii8 = int16(8);

assert_checkequal(i1:i2:i8, int8([1,3,5,7]));
checkCallOverload("i1:i2:ii8");
checkCallOverload("i1:ii2:i8");
checkCallOverload("i1:ii2:ii8");
checkCallOverload("ii1:i2:i8");
checkCallOverload("ii1:i2:ii8");
checkCallOverload("ii1:ii2:i8");
assert_checkequal(ii1:ii2:ii8, int16([1,3,5,7]));

// bool
t = %t;
checkCallOverload("t:t:t");
checkCallOverload("t:t:8");
checkCallOverload("t:2:%t");
checkCallOverload("t:2:8");
checkCallOverload("1:t:%t");
checkCallOverload("1:t:8");
assert_checkerror("1:2:t", [], 10000);

// poly
assert_checktrue(execstr("$:$:$", "errcatch") == 0);
assert_checktrue(execstr("$:$:8", "errcatch") == 0);
assert_checktrue(execstr("$:2:$", "errcatch") == 0);
assert_checktrue(execstr("$:2:8", "errcatch") == 0);
assert_checktrue(execstr("1:$:$", "errcatch") == 0);
assert_checktrue(execstr("1:$:8", "errcatch") == 0);
assert_checktrue(execstr("1:2:$", "errcatch") == 0);
assert_checktrue(execstr("1:2:8", "errcatch") == 0);

assert_checktrue(execstr("$:$", "errcatch") == 0);
assert_checktrue(execstr("$:8", "errcatch") == 0);
assert_checktrue(execstr("2:$", "errcatch") == 0);
assert_checktrue(execstr("2:8", "errcatch") == 0);

// rational
t = 1/%s;
checkCallOverload("t:t:t");
checkCallOverload("t:t:8");
checkCallOverload("t:2:%t");
checkCallOverload("t:2:8");
checkCallOverload("1:t:%t");
checkCallOverload("1:t:8");
assert_checkerror("1:2:t", [], 10000);

// rational
t = list();
checkCallOverload("t:t:t");
checkCallOverload("t:t:8");
checkCallOverload("t:2:%t");
checkCallOverload("t:2:8");
checkCallOverload("1:t:%t");
checkCallOverload("1:t:8");
assert_checkerror("1:2:t", [], 10000);





