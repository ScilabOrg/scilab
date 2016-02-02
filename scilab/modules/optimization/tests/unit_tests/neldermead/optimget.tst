// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

// 
// val = optimget ( options , key )
//
op = optimset ();
op = optimset(op,'TolX',1.e-12);
val = optimget(op,'TolX');
assert_checkequal ( val , 1.e-12 );

// 
// val = optimget ( options , key , value ) with non-empty value
//
op = optimset ();
op = optimset(op,'TolX',1.e-12);
val = optimget(op,'TolX' , 1.e-5);
assert_checkequal ( val , 1.e-12 );
// 
// val = optimget ( options , key , value ) with empty value
//
op = optimset ();
val = optimget(op,'TolX' , 1.e-5);
assert_checkequal ( val , 1.e-5 );

// 
// val = optimget ( options , key ) with ambiguous key
//
op = optimset ();
op = optimset(op,'TolX',1.e-12);
cmd = "optimget(op,''Tol'' )";
assert_checkerror(cmd,"%s: Ambiguous property name %s matches several fields : %s",[],"optimget","Tol","TolFun TolX");

//
// Test with wrong number of arguments
//
op = optimset ();
cmd = "optimget ( op )";
assert_checkerror(cmd,"%s: Wrong number of arguments : %d expected while %d given",[],"optimget",2,1);

//
// Test with wrong number of arguments
//
op = optimset ();
cmd = "optimget ( op , ""TolX"" , 1.e-12 , 1.e-13)";
assert_checkerror(cmd,"%s: Wrong number of arguments : %d expected while %d given",[],"optimget",2,4);
// 
// val = optimget ( options , key ) with leading characters only
//
op = optimset ();
op = optimset ( op , 'MaxFunEvals' , 1000 );
val = optimget ( op , 'MaxF' );
assert_checkequal ( val , 1000 );
// 
// val = optimget ( options , key , default )
//
default = optimset ( 'fminsearch' );
op = optimset ();
value = optimget(op,'TolX',default.TolX);
assert_checkequal ( value , 1.e-4 );

