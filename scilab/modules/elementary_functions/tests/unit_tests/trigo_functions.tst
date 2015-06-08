// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for trigonometric functions
// =============================================================================

realvalues1 = [1.1  1.5  2  4;
-1.1 -1.5 -2 -4];
realvalues2 = [1.1:0.1:10;
-[1.1:0.1:10]];
realvalues3 = [0.1:0.1:0.9;
-[0.1:0.1:0.9]];

realpartvalues = [0  1  %pi/2  %pi;-0 -1 -%pi/2  -%pi];

cplxvalues1 = realvalues1.*%i;
cplxvalues2 = realvalues2.*%i;
cplxvalues3 = realvalues3.*%i;

cplxpartvalues = realpartvalues.*%i;

// ============
// acos
// ============
// acos(z) = pi/2 + i *log(i*z + sqrt(1 - z²))
function r = FormuleAcos1(z)
    r = 0.5 * %pi + %i * log(%i * z + sqrt(1 - z.^2));
endfunction

// acos(z) = pi - acos(-z)
function r = FormuleAcos2(z)
    r = %pi - acos(-z);
endfunction

// acos(z) = pi/2 + asin(-z)
function r = FormuleAcos3(z)
    r = 0.5 * %pi + asin(-z);
endfunction

// acos(z) = pi/2 - asin(z)
function r = FormuleAcos4(z)
    r = 0.5 * %pi - asin(z);
endfunction

// acos(z) = asec(1/z)
function r = FormuleAcos5(z)
    r = asec(1 ./ z);
endfunction

// acos(z) = -i*log(z + i*sqrt(1 -z²))
function r = FormuleAcos6(z)
    r = -%i * log(z + %i*sqrt(1 - z.^2));
endfunction

// acos(z) = pi/2 +/- acos(sqrt(1 - x²))
function r = FormuleAcos7(x)
    r = 0.5 * %pi - sign(x) .* acos(sqrt(1 - x.^2));
endfunction

r1 = acos(realvalues1);
r2 = acos(realvalues2);
r3 = acos(realvalues3);

assert_checkalmostequal(r1, FormuleAcos1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos1(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos2(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos3(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos4(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos4(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos5(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos5(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos5(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos6(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos6(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos6(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos7(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos7(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos7(realvalues3), sqrt(%eps), 1e-16);

r = acos(realpartvalues);
assert_checkalmostequal(r, FormuleAcos1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos2(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos3(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos4(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcos5(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos6(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos7(realpartvalues), sqrt(%eps), 1e-16);

r1 = acos(cplxvalues1);
r2 = acos(cplxvalues2);
r3 = acos(cplxvalues3);

assert_checkalmostequal(r1, FormuleAcos1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos1(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos2(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos2(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos3(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos3(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos3(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos4(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos4(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos4(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos5(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos5(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos5(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcos6(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcos6(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcos6(cplxvalues3), sqrt(%eps), 1e-16);

r = acos(cplxpartvalues);
assert_checkalmostequal(r, FormuleAcos1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos2(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos3(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos4(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcos5(cplxpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcos6(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// acot
// ============
// acot(x) =  pi/2 - 1/2 *i * log((i + x)/(i - x))
function r = FormuleAcot1(x)
    r = %pi/2 - 0.5*%i *log((%i + x)./(%i - x));
endfunction

// acot(z) = atan(1/z) (+ pi/2 si z < 0)
function r = FormuleAcot2(z)
    r = atan(1 ./z) + %pi/2*(1 - csgn(z));
endfunction

// acot(z) = pi - acot(-z)
function r = FormuleAcot3(z)
    r = %pi - acot(-z);
endfunction

function r = FormuleAcot4(z)
    r = 0.5 * %i * log((%i + (1 ./z))./(%i - (1 ./z))) + %pi/2*(1 - csgn(z)) ;
endfunction

// acot(z) = i/2 *  (log((z-i)/z) - log((z+i)/z))  (+ pi/2 si z < 0)
function r = FormuleAcot5(z)
    r = 0.5 * %i * (log((z - %i) ./z) - log((z + %i) ./z)) + %pi/2*(1 - csgn(z)) ;
endfunction

function r = FormuleAcot6(x)
    r = asec(sqrt(x.^2 + 1) ./x);
endfunction

function r = FormuleAcot7(x)
    r = acos(x ./sqrt(x.^2 + 1));
endfunction

function r = FormuleAcot8(x)
    r = %pi/2 - asin(x ./sqrt(x.^2 + 1));
endfunction

function r = FormuleAcot9(x)
    r = %pi - %pi/2 * sign(x) - acot(1 ./x);
endfunction

function r = FormuleAcot10(x)
    r = %pi*bool2s(sign(x)<0) + sign(x).*acsc(sqrt(x.^2 + 1))
endfunction

function r = FormuleAcot11(x)
    r = %pi*bool2s(sign(x)<0) + sign(x).*asin(1 ./sqrt(x.^2 + 1))
endfunction

r1 = acot(realvalues1);
r2 = acot(realvalues2);
r3 = acot(realvalues3);

assert_checkalmostequal(r1, FormuleAcot1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot1(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot2(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot3(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot4(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot4(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot5(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot5(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot5(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot6(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot6(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot6(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot7(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot7(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot7(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot8(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot8(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot8(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot9(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot9(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot9(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot10(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot10(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot10(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot11(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot11(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot11(realvalues3), sqrt(%eps), 1e-16);

r = acot(realpartvalues);
assert_checkalmostequal(r, FormuleAcot1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot2(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot3(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot4(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot5(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot6(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcot7(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcot8(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot9(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot10(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot11(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);

r1 = acot(cplxvalues1);
r2 = acot(cplxvalues2);
r3 = acot(cplxvalues3);

assert_checkalmostequal(r1, FormuleAcot2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot2(cplxvalues2), sqrt(%eps), 1e-15);

assert_checkalmostequal(r1, FormuleAcot3(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot3(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot3(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot4(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot4(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcot4(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcot5(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcot5(cplxvalues2), sqrt(%eps), 1e-15);

cplxpartvalues = [0  %pi/2  %pi;-0 -%pi/2  -%pi].*%i;
r = acot(cplxpartvalues);
assert_checkalmostequal(r(:,2:$), FormuleAcot2(cplxpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcot3(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot4(cplxpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcot5(cplxpartvalues(:,2:$)), sqrt(%eps), 1e-16);

// ============
// acsc
// ============
// acsc(z) = -i * log(sqrt(1 - 1/z²) + i/z)
function r = FormuleAcsc1(z)
    r = -%i * log(sqrt(1 - 1 ./z.^2) + %i ./z);
endfunction

// acsc(z) = asin(1/z)
function r = FormuleAcsc2(z)
    r = asin(1 ./z);
endfunction

// acsc(z) = pi/2 - asec(z)
function r = FormuleAcsc3(z)
    r = 0.5 * %pi - asec(z);
endfunction

// acsc(z) = -pi/2 + asec(-z)
function r = FormuleAcsc4(z)
    r = -0.5 * %pi + asec(-z);
endfunction

function r = FormuleAcsc5(x)
    r = asec(x ./sqrt(x.^2-1)) - %pi*bool2s(sign(x)<0);
endfunction

function r = FormuleAcsc6(x)
    r = -%pi*bool2s(sign(x)<0) + acos(sqrt(x.^2-1) ./x);
endfunction

function r = FormuleAcsc7(x)
    r = sign(x) .* acot(sqrt(x.^2-1));
endfunction

r1 = acsc(realvalues1);
r2 = acsc(realvalues2);
r3 = acsc(realvalues3);

assert_checkalmostequal(r1, FormuleAcsc1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc1(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc2(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc3(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc4(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc4(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc5(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc5(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc5(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc6(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc6(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc6(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc7(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc7(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc7(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = acsc(realpartvalues);
assert_checkalmostequal(r, FormuleAcsc1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc2(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc3(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc4(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAcsc5(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc6(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc7(realpartvalues), sqrt(%eps), 1e-16);

r1 = acsc(cplxvalues1);
r2 = acsc(cplxvalues2);
r3 = acsc(cplxvalues3);

assert_checkalmostequal(r1, FormuleAcsc1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc1(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc2(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc2(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc3(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc3(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc3(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsc4(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsc4(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcsc4(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues.*%i;
r = acsc(cplxpartvalues);
assert_checkalmostequal(r, FormuleAcsc1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc2(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc3(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsc4(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// asec
// ============
// asec(z) = pi/2 + i*log(sqrt(1 - (1/z²)) + i/z)
function r = FormuleAsec1(z)
    r = 0.5 * %pi + %i * log(sqrt(1-(1 ./z.^2)) + %i ./z);
endfunction

// asec(z) = acos(1/z)
function r = FormuleAsec2(z)
    r = acos(1 ./z);
endfunction

// asec(z) = pi - asec(-z)
function r = FormuleAsec3(z)
    r = %pi - asec(-z);
endfunction

// asec(z) = pi/2 - acsc(z)
function r = FormuleAsec4(z)
    r = 0.5 * %pi - acsc(z);
endfunction

// asec(z) = pi/2 - acsc(z)
function r = FormuleAsec5(z)
    r = 0.5 * %pi + acsc(-z);
endfunction

// pour x reel, abs(x) > 1
function r = FormuleAsec6(x)
    r = acsc(x ./sqrt(x.^2-1)) + %pi*bool2s(x<1);
endfunction

function r = FormuleAsec7(x)
    r = %pi*bool2s(x<-1) + sign(x).*acot(1 ./sqrt(x.^2-1));
endfunction

function r = FormuleAsec8(x)
    r = asin(sqrt(x.^2-1) ./x) + %pi*bool2s(x<1);
endfunction

function r = FormuleAsec9(x)
    r = %pi*bool2s(x<1) + sign(x).*atan(sqrt(x.^2-1));
endfunction

r1 = asec(realvalues1);
r2 = asec(realvalues2);
r3 = asec(realvalues3);

assert_checkalmostequal(r1, FormuleAsec1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec1(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec2(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec3(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec4(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec4(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec5(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec5(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec5(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec6(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec6(realvalues2), sqrt(%eps), 1e-15);

assert_checkalmostequal(r1, FormuleAsec7(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec7(realvalues2), sqrt(%eps), 1e-15);

assert_checkalmostequal(r1, FormuleAsec8(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec8(realvalues2), sqrt(%eps), 1e-15);

assert_checkalmostequal(r1, FormuleAsec9(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec9(realvalues2), sqrt(%eps), 1e-15);

r = asec(realpartvalues);
assert_checkalmostequal(r, FormuleAsec1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec2(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec3(realpartvalues), sqrt(%eps), 1e-15);
assert_checkalmostequal(r, FormuleAsec4(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec5(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAsec6(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAsec7(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec8(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec9(realpartvalues), sqrt(%eps), 1e-16);

r1 = asec(cplxvalues1);
r2 = asec(cplxvalues2);
r3 = asec(cplxvalues3);

assert_checkalmostequal(r1, FormuleAsec1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec1(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec2(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec2(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec3(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec3(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec3(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec4(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec4(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec4(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsec5(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsec5(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsec5(cplxvalues3), sqrt(%eps), 1e-16);

r = asec(cplxpartvalues);
assert_checkalmostequal(r, FormuleAsec1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec2(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec3(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec4(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsec5(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// asin
// ============
// asin(z) = -i * log(i *z + sqrt(1-z²))
function r = FormuleAsin1(z)
    r = -%i * log(%i * z + sqrt(1 - z.^2));
endfunction

// asin(z) = acsc(1/z)
function r = FormuleAsin2(z)
    r = acsc(1 ./z);
endfunction

// asin(z) =  -asin(-z)
function r = FormuleAsin3(z)
    r = -asin(-z);
endfunction

// asin(z) = acos(-z) -pi/2
function r = FormuleAsin4(z)
    r = acos(-z) - 0.5 * %pi;
endfunction

// asin(z) = pi/2 - acos(z)
function r = FormuleAsin5(z)
    r = 0.5 * %pi - acos(z);
endfunction

function r = FormuleAsin6(x)
    r = sign(x).* (0.5*%pi - asin(sqrt(1 - x.^2)));
endfunction

function r = FormuleAsin7(x)
    r = sign(x) .* acos(sqrt(1 - x.^2));
endfunction

function r = FormuleAsin8(x)
    r = sign(x) .* asec(1 ./sqrt(1 - x.^2));
endfunction

r1 = asin(realvalues1);
r2 = asin(realvalues2);
r3 = asin(realvalues3);

assert_checkalmostequal(r1, FormuleAsin1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin1(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin2(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin3(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin4(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin4(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin5(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin5(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin5(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin6(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin6(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin6(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin7(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin7(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin7(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin8(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin8(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin8(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0 -1 -%pi/2  -%pi];
r = asin(realpartvalues);
assert_checkalmostequal(r, FormuleAsin1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin3(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin4(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin5(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin6(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin7(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(asin([1  %pi/2  %pi;-1 -%pi/2  -%pi]), FormuleAsin2([1  %pi/2  %pi;-1 -%pi/2  -%pi]), sqrt(%eps), 1e-16);
assert_checkalmostequal(asin([0  %pi/2  %pi;-0 -%pi/2  -%pi]), FormuleAsin8([0  %pi/2  %pi;-0 -%pi/2  -%pi]), sqrt(%eps), 1e-16);

r1 = asin(cplxvalues1);
r2 = asin(cplxvalues2);
r3 = asin(cplxvalues3);

assert_checkalmostequal(r1, FormuleAsin1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin1(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin2(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin2(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin3(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin3(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin3(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin4(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin4(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin4(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsin5(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsin5(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsin5(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .* %i;
r = asin(cplxpartvalues);
assert_checkalmostequal(r, FormuleAsin1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(asin([1  %pi/2  %pi;-1 -%pi/2  -%pi].*%i), FormuleAsin2([1  %pi/2  %pi;-1 -%pi/2  -%pi].*%i), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin3(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin4(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsin5(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// atan
// ============
// atan(z) = i/2 * (log(1 -i*z) - log(1 +i*z))
function r = FormuleAtan1(z)
    r = 0.5 * %i * (log(1 - %i * z) - log(1 + %i *z));
endfunction

// atan(z) = pi/2 - acto(z)
function r = FormuleAtan2(z)
    r = %pi/2 - acot(z);
endfunction

// atan(z) = -atan(-z)
function r = FormuleAtan3(z)
    r = -atan(-z);
endfunction

function r = FormuleAtan4(x)
    r = 0.5 * %pi - acos(x ./ sqrt(x.^2 + 1));
endfunction

function r = FormuleAtan5(x)
    r = asin(x ./ sqrt(x.^2 + 1));
endfunction

function r = FormuleAtan6(x)
    r = acsc(sqrt(x.^2 + 1) ./x);
endfunction

r1 = atan(realvalues1);
r2 = atan(realvalues2);
r3 = atan(realvalues3);

assert_checkalmostequal(r1, FormuleAtan1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan3(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan4(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan4(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan5(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan5(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan5(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan6(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan6(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan6(realvalues3), sqrt(%eps), 1e-16);

r = atan(realpartvalues);

assert_checkalmostequal(r, FormuleAtan1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtan2(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtan3(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtan4(realpartvalues), sqrt(%eps), 1e-15);
assert_checkalmostequal(r, FormuleAtan5(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r(:,2:$), FormuleAtan6(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);

r1 = atan(cplxvalues1);
r2 = atan(cplxvalues2);
r3 = atan(cplxvalues3);

assert_checkalmostequal(r1, FormuleAtan1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan2(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtan3(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtan3(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtan3(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = [0  %pi/2  %pi;0  -%pi/2  -%pi].*%i;
r = atan(cplxpartvalues);

assert_checkalmostequal(r, FormuleAtan1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtan2(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtan3(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// cotg
// ============
// cotg(z) = 1/ tan(z)
function r = FormuleCotg1(z)
    r = 1 ./tan(z);
endfunction

function r = FormuleCotg2(z)
    r = (%i * (exp(%i * z) + exp(-%i * z)))./(exp(%i * z) - exp(-%i * z));
endfunction

r1 = cotg(realvalues1);
r2 = cotg(realvalues2);
r3 = cotg(realvalues3);

assert_checkalmostequal(r1, FormuleCotg1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCotg1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCotg1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCotg2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCotg2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCotg2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = cotg(realpartvalues);

assert_checkalmostequal(r, FormuleCotg1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleCotg2(realpartvalues), sqrt(%eps), 1e-16);

r1 = cotg(cplxvalues1);
r2 = cotg(cplxvalues2);
r3 = cotg(cplxvalues3);

assert_checkalmostequal(r1, FormuleCotg1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCotg1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCotg1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCotg2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCotg2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCotg2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = cotg(cplxpartvalues);

assert_checkalmostequal(r, FormuleCotg1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleCotg2(cplxpartvalues), sqrt(%eps), 1e-16);

// 1 + cotg(x)^2 = csc(x)^2
assert_checkalmostequal(1 + cotg(realvalues1).^2, csc(realvalues1).^2, sqrt(%eps), 1e-16);
assert_checkalmostequal(1 + cotg(realvalues2).^2, csc(realvalues2).^2, sqrt(%eps), 1e-16);

assert_checkalmostequal(1 + cotg(cplxvalues1).^2, csc(cplxvalues1).^2, sqrt(%eps), 1e-16);
assert_checkalmostequal(1 + cotg(cplxvalues2).^2, csc(cplxvalues2).^2, sqrt(%eps), 1e-15);

// ============
// csc
// ============
// csc(z) = 1/sin(z)
function r = FormuleCsc1(z)
    r = 1 ./sin(z);
endfunction

// sin(z) = (exp(iz) - exp(-iz))/2 alors csc(z)= 2/(exp(iz) - exp(-iz))
function r = FormuleCsc2(z)
    r = (2 * %i) ./(exp(%i *z) - exp(-%i*z));
endfunction

r1 = csc(realvalues1);
r2 = csc(realvalues2);
r3 = csc(realvalues3);

assert_checkalmostequal(r1, FormuleCsc1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsc1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsc1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCsc2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsc2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsc2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = csc(realpartvalues);

assert_checkalmostequal(r, FormuleCsc1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleCsc2(realpartvalues), sqrt(%eps), 1e-16);

r1 = csc(cplxvalues1);
r2 = csc(cplxvalues2);
r3 = csc(cplxvalues3);

assert_checkalmostequal(r1, FormuleCsc1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsc1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsc1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCsc2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsc2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsc2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = csc(cplxpartvalues);

assert_checkalmostequal(r, FormuleCsc1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleCsc2(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// sec
// ============
// sec(z) = 1/cos(z)
function r = FormuleSec1(z)
    r = 1 ./cos(z);
endfunction

//cos(z) = (exp(iz) + exp(-iz))/2 alors sec(z) = 2/exp(iz) + exp(-iz))
function r = FormuleSec2(z)
    r = 2 ./(exp(%i * z) + exp(-%i * z));
endfunction

r1 = sec(realvalues1);
r2 = sec(realvalues2);
r3 = sec(realvalues3);

assert_checkalmostequal(r1, FormuleSec1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSec1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSec1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleSec2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSec2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSec2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0 -1 -%pi/2  -%pi];
r = sec(realpartvalues);

assert_checkalmostequal(r, FormuleSec1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleSec2(realpartvalues), sqrt(%eps), 1e-16);

r1 = sec(cplxvalues1);
r2 = sec(cplxvalues2);
r3 = sec(cplxvalues3);

assert_checkalmostequal(r1, FormuleSec1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSec1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSec1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleSec2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSec2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSec2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
ResSec = sec(cplxpartvalues);

assert_checkalmostequal(ResSec, FormuleSec1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(ResSec, FormuleSec2(cplxpartvalues), sqrt(%eps), 1e-16);

assert_checkalmostequal(sec(realvalues1).^2 - tan(realvalues1).^2, ones(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(sec(realvalues2).^2 - tan(realvalues2).^2, ones(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(sec(realvalues3).^2 - tan(realvalues3).^2, ones(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(sec(cplxvalues1).^2 - tan(cplxvalues1).^2, ones(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(sec(cplxvalues2).^2 - tan(cplxvalues2).^2, ones(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(sec(cplxvalues3).^2 - tan(cplxvalues3).^2, ones(cplxvalues3), sqrt(%eps), 1e-16);
