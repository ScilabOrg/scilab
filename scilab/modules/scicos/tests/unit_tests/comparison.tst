// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
//
// <-- CLI SHELL MODE -->

b = scicos_block();
assert_checktrue(and(b==b));

d = scicos_diagram();
assert_checktrue(and(d==d));

l = scicos_link();
assert_checktrue(and(l==l));

assert_checkfalse(b==l);
assert_checkfalse(b==d);
assert_checkfalse(d==l);

// Just modifying 'graphics'
b2 = b;
b2.graphics.sz = [50 50];
resRef = [%t %f %t %t %t];
assert_checkequal(b==b2, resRef);

// 'graphics', 'model' and 'gui' differ
Sum = BIGSOM_f("define");
resRef = [%t %f %f %f %t];
assert_checkequal(b==Sum, resRef);

// 'props' and 'objs' differ
d2 = d;
d2.props.tf = 50;
d2.objs = list(Sum);
resRef = [%t %f %f %t %t];
assert_checkequal(d==d2, resRef);
