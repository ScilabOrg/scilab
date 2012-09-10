// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
r = csv_stringtodouble("-Nani");
assert_checkequal ( r , complex(0,%nan) );
// =============================================================================
r = csv_stringtodouble("Nani");
assert_checkequal ( r , complex(0,%nan) );
// =============================================================================
r = csv_stringtodouble("+Nani");
assert_checkequal ( r , complex(0,%nan) );
// =============================================================================
r = csv_stringtodouble("-Infi");
assert_checkequal ( r , complex(0,-%inf) );
// =============================================================================
r = csv_stringtodouble("Infi");
assert_checkequal ( r , complex(0,%inf) );
// =============================================================================
r = csv_stringtodouble("+Infi");
assert_checkequal ( r , complex(0,%inf) );
// =============================================================================
r = csv_stringtodouble("Nan-Nani");
assert_checkequal ( r , complex(%nan,%nan) );
// =============================================================================
r = csv_stringtodouble("Nan-2i");
assert_checkequal ( r , complex(%nan,-2) );
// =============================================================================
r = csv_stringtodouble("-Nan-2i");
assert_checkequal ( r , complex(%nan,-2) );
// =============================================================================
r = csv_stringtodouble("+Nan-2i");
assert_checkequal ( r , complex(%nan,-2) );
// =============================================================================
r = csv_stringtodouble("Nan-Infi");
assert_checkequal ( r , complex(%nan,-%inf) );
// =============================================================================
r = csv_stringtodouble("-Inf-2i");
assert_checkequal ( r , complex(-%inf,-2) );
// =============================================================================
r = csv_stringtodouble("+Inf-2i");
assert_checkequal ( r , complex(%inf,-2) );
// =============================================================================
r = csv_stringtodouble("Inf-2i");
assert_checkequal ( r , complex(%inf,-2) );
// =============================================================================
r = csv_stringtodouble("Inf-Infi");
assert_checkequal ( r , complex(%inf,-%inf) );
// =============================================================================
r = csv_stringtodouble("Inf+Infi");
assert_checkequal ( r , complex(%inf,%inf) );
// =============================================================================
r = csv_stringtodouble("1+Nani");
assert_checkequal ( r , complex(1,%nan) );
// =============================================================================
r = csv_stringtodouble("1-Nani");
assert_checkequal ( r , complex(1,%nan) );
// =============================================================================
r = csv_stringtodouble("-1-Nani");
assert_checkequal ( r , complex(-1,%nan) );
// =============================================================================
r = csv_stringtodouble("1+Infi");
assert_checkequal ( r , complex(1,%inf) );
// =============================================================================
r = csv_stringtodouble("1-Infi");
assert_checkequal ( r , complex(1,-%inf) );
// =============================================================================
assert_checkequal ( csv_stringtodouble("Nan") , %nan );
assert_checkequal ( csv_stringtodouble("-Nan"), %nan );
assert_checkequal ( csv_stringtodouble("+Nan"), %nan );
assert_checkequal ( csv_stringtodouble("-Inf") , -%inf );
assert_checkequal ( csv_stringtodouble("Inf") , %inf );
assert_checkequal ( csv_stringtodouble("+Inf") , %inf );
assert_checkequal ( csv_stringtodouble("+i") , %i  );
assert_checkequal ( csv_stringtodouble("-i") , -%i  );
assert_checkequal ( csv_stringtodouble("+j") , %i  );
assert_checkequal ( csv_stringtodouble("-j") , -%i  );
assert_checkequal ( csv_stringtodouble("i") , %i  );
assert_checkequal ( csv_stringtodouble("j") , %i  );
assert_checkequal ( csv_stringtodouble(" 3 + 2i") , complex(3,2) );
assert_checkequal ( csv_stringtodouble(" 3 - 2i") , complex(3,- 2) );
assert_checkequal ( csv_stringtodouble(" -3 + 2i") , complex(-3,2) );
assert_checkequal ( csv_stringtodouble(" 3 - 2i") , complex(3,-2) );
assert_checkequal ( csv_stringtodouble("- 3 - 2i") , complex(-3,-2) );
assert_checkequal ( csv_stringtodouble("+3 - 2i") , complex(+3,-2) );
assert_checkequal ( csv_stringtodouble("+3 + 2i") , complex(3,2) );
assert_checkequal ( csv_stringtodouble("+3 - 2i") , complex(3,-2) );
assert_checkequal ( csv_stringtodouble("4+i") , complex(4,1) );
assert_checkequal ( csv_stringtodouble("4-i") , complex(4,-1) );
assert_checkequal ( csv_stringtodouble("-4+i") , complex(-4,1) );
assert_checkequal ( csv_stringtodouble("-4-i") , complex(-4,-1) );
assert_checkequal ( csv_stringtodouble("+4+i") , complex(4,1) );
assert_checkequal ( csv_stringtodouble("+4-i") , complex(4,-1) );
assert_checkequal ( csv_stringtodouble("10")  , 10 );
assert_checkequal ( csv_stringtodouble("+10") , 10 );
assert_checkequal ( csv_stringtodouble("-10") , -10 );
assert_checkequal ( csv_stringtodouble("10.") , 10 );
assert_checkequal ( csv_stringtodouble("+10.") , 10 );
assert_checkequal ( csv_stringtodouble("-10.") , -10 );
assert_checkequal ( csv_stringtodouble(".4") , .4 );
assert_checkequal ( csv_stringtodouble("+.4") , .4 );
assert_checkequal ( csv_stringtodouble("-.4") , -.4 );
assert_checkequal ( csv_stringtodouble("10.4") , 10.4 );
assert_checkequal ( csv_stringtodouble("+10.4") , 10.4 );
assert_checkequal ( csv_stringtodouble("-10.4") , -10.4 );
assert_checkequal ( csv_stringtodouble("10+10j") , complex(10, 10) );
assert_checkequal ( csv_stringtodouble("+10+10j") , complex(10, 10) );
assert_checkequal ( csv_stringtodouble("-10+10j") , complex(-10, 10) );
assert_checkequal ( csv_stringtodouble("10.+10j") , complex(10, 10) );
assert_checkequal ( csv_stringtodouble("+10.+10j") , complex(10, 10) );
assert_checkequal ( csv_stringtodouble("-10.+10j") , complex(-10, 10) );
assert_checkequal ( csv_stringtodouble("10.4+10j") , complex(10.4, 10) );
assert_checkequal ( csv_stringtodouble("+10.4+10j") , complex(10.4, 10) );
assert_checkequal ( csv_stringtodouble("-10.4+10j") , complex(-10.4, 10) );
assert_checkequal ( csv_stringtodouble(".4+10j") , complex(.4, 10) );
assert_checkequal ( csv_stringtodouble("+.4+10j") , complex(.4, 10) );
assert_checkequal ( csv_stringtodouble("-.4+10j") , complex(-.4, 10) );
assert_checkequal ( csv_stringtodouble("10+10.4j") , complex(10, 10.4) );
assert_checkequal ( csv_stringtodouble("10.4+10.4j") , complex(10.4, 10.4) );
assert_checkequal ( csv_stringtodouble("+10.4+10.4j") , complex(10.4, 10.4) );
assert_checkequal ( csv_stringtodouble("+10.4-10.4j") , complex(10.4, -10.4) );
assert_checkequal ( csv_stringtodouble("1i") , %i );
assert_checkequal ( csv_stringtodouble("1j") , %i );
assert_checkequal ( csv_stringtodouble("+1j") , %i );
assert_checkequal ( csv_stringtodouble("+1i") , %i );
assert_checkequal ( csv_stringtodouble("-1j") , complex(0,-1) );
assert_checkequal ( csv_stringtodouble("-1i") , complex(0,-1) );
assert_checkequal ( csv_stringtodouble("10j") , complex(0,10) );
assert_checkequal ( csv_stringtodouble("+10j") , complex(0,10) );
assert_checkequal ( csv_stringtodouble("-10j") , complex(0, -10) );
assert_checkequal ( csv_stringtodouble("10.j") , complex(0,10) );
assert_checkequal ( csv_stringtodouble("+10.j") , complex(0,10) );
assert_checkequal ( csv_stringtodouble("-10.j") , complex(0,-10) );
assert_checkequal ( csv_stringtodouble(".4j") , complex(0,.4) );
assert_checkequal ( csv_stringtodouble("+.4j") , complex(0,.4) );
assert_checkequal ( csv_stringtodouble("-.4j") , complex(0,-.4) );
assert_checkequal ( csv_stringtodouble("10.4j") , complex(0,10.4) );
assert_checkequal ( csv_stringtodouble("+10.4j") , complex(0,10.4) );
assert_checkequal ( csv_stringtodouble("-10.4j") , complex(0,-10.4) );
assert_checkequal ( csv_stringtodouble("4e2") , 400 );
assert_checkequal ( csv_stringtodouble("-4e2") , -400 );
assert_checkequal ( csv_stringtodouble("+4e2") , 400 );
assert_checkequal ( csv_stringtodouble("0.4e2") , 40 );
assert_checkequal ( csv_stringtodouble("3e2 + 2e3i") , complex(300, 2000) );
assert_checkequal ( csv_stringtodouble("3e2 - 2e3i") , complex(300, -2000) );
if ( getos() == "Windows" ) then
    assert_checkequal ( csv_stringtodouble("3e2 - 2d3i") , complex(300, -2000) );
    assert_checkequal ( csv_stringtodouble("3e2 + 2d3i") , complex(300, 2000) );
    assert_checkequal ( csv_stringtodouble("3d2 + 2e3i") , complex(300, 2000) );
    assert_checkequal ( csv_stringtodouble("3d2 - 2e3i") , complex(300, -2000) );
    assert_checkequal ( csv_stringtodouble("3d2 + 2d3i") , complex(300, 2000) );
    assert_checkequal ( csv_stringtodouble("3d2 - 2d3i") , complex(300, -2000) );
end
// =============================================================================
r = csv_stringtodouble("3e2 - blabli");
assert_checkequal ( r , %nan);
// =============================================================================
r = csv_stringtodouble("3e2 - blabli", %t);
assert_checkequal ( r , %nan);
// =============================================================================
assert_checkequal ( execstr("r = csv_stringtodouble(""3e2 - blabli"", %f);", "errcatch") , 999 );
assert_checkequal ( lasterror() , msprintf(_("%s: can not convert data.\n"), "csv_stringtodouble") );
// =============================================================================
r = csv_stringtodouble("-Nan*%i");
assert_checkequal ( isnan(imag(r)) , %t);
assert_checkequal ( csv_stringtodouble("+%i") , %i  );
assert_checkequal ( csv_stringtodouble("-%i") , -%i  );
assert_checkequal ( csv_stringtodouble("%i") , %i  );
assert_checkequal ( csv_stringtodouble(" 3 + 2*%i") , complex(3,2) );
assert_checkequal ( csv_stringtodouble(" 3 - 2*%i") , complex(3,- 2) );
assert_checkequal ( csv_stringtodouble(" -3 + 2*%i") , complex(-3,2) );
assert_checkequal ( csv_stringtodouble(" 3 - 2*%i") , complex(3,-2) );
assert_checkequal ( csv_stringtodouble("- 3 - 2*%i") , complex(-3,-2) );
assert_checkequal ( csv_stringtodouble("+3 - 2*%i") , complex(+3,-2) );
assert_checkequal ( csv_stringtodouble("+3 + 2*%i") , complex(3,2) );
assert_checkequal ( csv_stringtodouble("+3 - 2*%i") , complex(3,-2) );
assert_checkequal ( csv_stringtodouble("4+%i") , complex(4,1) );
assert_checkequal ( csv_stringtodouble("4-%i") , complex(4,-1) );
assert_checkequal ( csv_stringtodouble("-4+%i") , complex(-4,1) );
assert_checkequal ( csv_stringtodouble("-4-%i") , complex(-4,-1) );
assert_checkequal ( csv_stringtodouble("+4+%i") , complex(4,1) );
assert_checkequal ( csv_stringtodouble("+4-%i") , complex(4,-1) );
assert_checkequal ( csv_stringtodouble("1*%i") , %i );
assert_checkequal ( csv_stringtodouble("+1*%i") , %i );
assert_checkequal ( csv_stringtodouble("-1*%i") , complex(0,-1) );
assert_checkequal ( csv_stringtodouble("3e2 + 2e3*%i") , complex(300, 2000) );
assert_checkequal ( csv_stringtodouble("3e2 - 2e3*%i") , complex(300, -2000) );
// =============================================================================
assert_checkequal ( csv_stringtodouble('a'), %nan );
assert_checkequal ( csv_stringtodouble('e'), %nan );
assert_checkequal ( csv_stringtodouble('d'), %nan );
assert_checkequal ( csv_stringtodouble('E'), %nan );
assert_checkequal ( csv_stringtodouble('e'), %nan );
// =============================================================================
assert_checkequal ( csv_stringtodouble("%pi") , %pi );
assert_checkequal ( csv_stringtodouble("-%pi") , -%pi );
assert_checkequal ( csv_stringtodouble("%nan") , %nan );
assert_checkequal ( csv_stringtodouble("%inf") , %inf );
assert_checkequal ( csv_stringtodouble("-%inf") , -%inf );
assert_checkequal ( csv_stringtodouble("+%inf") , %inf );
// =============================================================================
