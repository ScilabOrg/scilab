// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for hyperbolic trigonometric functions
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
// acosh
// ============
// acosh(z) = log(z + sqrt(z + 1) .* sqrt(z -1))
function r = FormuleAcosh(z)
    r = log(z + sqrt(z + 1) .* sqrt(z - 1));
endfunction

r1 = acosh(realvalues1);
r2 = acosh(realvalues2);
r3 = acosh(realvalues3);

assert_checkalmostequal(r1, FormuleAcosh(realvalues1), sqrt(%eps), 1e-15);
assert_checkalmostequal(r2, FormuleAcosh(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcosh(realvalues3), sqrt(%eps), 1e-15);

r = acosh(realpartvalues);
assert_checkalmostequal(r, FormuleAcosh(realpartvalues), sqrt(%eps), 1e-16);

r1 = acosh(cplxvalues1);
r2 = acosh(cplxvalues2);
r3 = acosh(cplxvalues3);

assert_checkalmostequal(r1, FormuleAcosh(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcosh(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAcosh(cplxvalues3), sqrt(%eps), 1e-16);

r = acosh(cplxpartvalues);
assert_checkalmostequal(r, FormuleAcosh(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// acoth
// ============
// acoth(z) = 1/2 * (log(1 + 1/z) - log(1 - 1/z))
function r = FormuleAcoth1(z)
    r = 0.5 * (log(1 + 1 ./z) - log(1 - 1 ./z)) - %pi*%i*bool2s(real(z)<0 & real(z)>-1& imag(z) == 0);
endfunction

// acoth(z) = 1/i * acot(-i * z)
function r = FormuleAcoth2(z)
    r = 1/%i * (-%pi*bool2s(real(z)>1 | imag(z) < 0) +acot(-%i*z));
endfunction

// acoth(x) = 1/2 * log((x + 1)/(x-1)) pour tout x < 1
function r = FormuleAcoth3(x)
    r = 0.5 * log((x + 1)./(x - 1)) - %pi*%i*bool2s(abs(x)>=0 & abs(x)<1);
endfunction

r1 = acoth(realvalues1);
r2 = acoth(realvalues2);
r3 = acoth(realvalues3);

assert_checkalmostequal(r1, FormuleAcoth1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcoth1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcoth1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcoth2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcoth2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcoth2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcoth3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcoth3(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcoth3(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0 %pi/2  %pi;-0 -%pi/2  -%pi];
r = acoth(realpartvalues);
assert_checkalmostequal(r(:,2:$), FormuleAcoth1(realpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcoth2(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcoth3(realpartvalues), sqrt(%eps), 1e-16);

r1 = acoth(cplxvalues1);
r2 = acoth(cplxvalues2);
r3 = acoth(cplxvalues3);

assert_checkalmostequal(r1, FormuleAcoth1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcoth1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcoth1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcoth2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcoth2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcoth2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .* %i;
r = acoth(cplxpartvalues);
assert_checkalmostequal(r(:,2:$), FormuleAcoth1(cplxpartvalues(:,2:$)), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcoth2(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// acsch
// ============
// acsch(z) = log(sqrt(1 + 1/z) + 1/z)
function r = FormuleAcsch1(z)
    r = log(sqrt(1 + 1 ./z.^2) + 1 ./z);
endfunction

// acsch(x) = log((1 +/- sqrt(1 + x²)) /x) en fonction x
function r = FormuleAcsch2(x)
    r = log((1 + sign(x) .* sqrt(1 + x.^2)) ./x);
endfunction

r1 = acsch(realvalues1);
r2 = acsch(realvalues2);
r3 = acsch(realvalues3);

assert_checkalmostequal(r1, FormuleAcsch1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsch1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcsch1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAcsch2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsch2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcsch2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = acsch(realpartvalues);
assert_checkalmostequal(r, FormuleAcsch1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAcsch2(realpartvalues), sqrt(%eps), 1e-16);

r1 = acsch(cplxvalues1);
r2 = acsch(cplxvalues2);
r3 = acsch(cplxvalues3);

assert_checkalmostequal(r1, FormuleAcsch1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAcsch1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAcsch1(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .* %i;
r = acsch(cplxpartvalues);
assert_checkalmostequal(r, FormuleAcsch1(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// asech
// ============
// asech(z) = log(sqrt(1/z -1) * sqrt(1/z + 1) +1/z)
function r = FormuleAsech1(z)
    Z = 1 ./z;
    r = log(sqrt(Z -1) .* sqrt(Z + 1) + Z);
endfunction

// asech(x) = log((1 + sqrt(1 - x²))/x)
function r = FormuleAsech2(x)
    r = log((1 + sign(x).*sqrt(1-x.^2)) ./x);
endfunction

r1 = asech(realvalues1);
r2 = asech(realvalues2);
r3 = asech(realvalues3);

assert_checkalmostequal(r1, FormuleAsech1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsech1(realvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsech1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsech2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsech2(realvalues2), sqrt(%eps), 1e-15);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = asech(realpartvalues);
assert_checkalmostequal(r, FormuleAsech1(realpartvalues), sqrt(%eps), 1e-15);
assert_checkalmostequal(r, FormuleAsech2(realpartvalues), sqrt(%eps), 1e-16);

r1 = asech(cplxvalues1);
r2 = asech(cplxvalues2);
r3 = asech(cplxvalues3);

assert_checkalmostequal(r1, FormuleAsech1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsech1(cplxvalues2), sqrt(%eps), 1e-15);
assert_checkalmostequal(r3, FormuleAsech1(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .* %i;
r = asech(cplxpartvalues);
assert_checkalmostequal(r, FormuleAsech1(cplxpartvalues), sqrt(%eps), 1e-15);

// ============
// asinh
// ============
// asinh(z) = log(z + sqrt(1 + z²))
function r = FormuleAsinh1(z)
    r = log(z + sqrt(1 + z.^2));
endfunction

// asinh(z) = (1/i)*asin(i*z)
function r = FormuleAsinh2(z)
    r = 1/%i * asin(%i * z);
endfunction

r1 = asinh(realvalues1);
r2 = asinh(realvalues2);
r3 = asinh(realvalues3);

assert_checkalmostequal(r1, FormuleAsinh1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsinh1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAsinh1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsinh2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsinh2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAsinh2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0  -1 -%pi/2  -%pi];
r = asinh(realpartvalues);
assert_checkalmostequal(r, FormuleAsinh1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsinh2(realpartvalues), sqrt(%eps), 1e-16);

r1 = asinh(cplxvalues1);
r2 = asinh(cplxvalues2);
r3 = asinh(cplxvalues3);

assert_checkalmostequal(r1, FormuleAsinh1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsinh1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAsinh1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAsinh2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAsinh2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAsinh2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = asinh(cplxpartvalues);
assert_checkalmostequal(r, FormuleAsinh1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAsinh2(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// atanh
// ============
// atanh(z) = 0.5 * (log(1 + z) - log(1 -z))
function r = FormuleAtanh1(z)
    r = 0.5 * (log(1+z) - log(1-z));
endfunction

// atanh(z) = (1/i) * atan(i*z)
function r = FormuleAtanh2(z)
    r = 1/%i * atan(%i * z);
endfunction

// pour tout x < 1, atanh(x) = 0.5* log((1+x)./(1-x))
function r = FormuleAtanh3(x)
    r = 0.5* log((1+x)./(1-x));
endfunction

r1 = atanh(realvalues1);
r2 = atanh(realvalues2);
r3 = atanh(realvalues3);

assert_checkalmostequal(r1, FormuleAtanh1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtanh1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtanh1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtanh2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtanh2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtanh2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r3, FormuleAtanh3(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  %pi/2  %pi;-0  -%pi/2  -%pi];
r = atanh(realpartvalues);

assert_checkalmostequal(r, FormuleAtanh1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtanh2(realpartvalues), sqrt(%eps), 1e-16);

r1 = atanh(cplxvalues1);
r2 = atanh(cplxvalues2);
r3 = atanh(cplxvalues3);

assert_checkalmostequal(r1, FormuleAtanh1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtanh1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtanh1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleAtanh2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleAtanh2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleAtanh2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = [0  1  %pi/2  %pi;0  -1  -%pi/2  -%pi].*%i;
r = atanh(cplxpartvalues);

assert_checkalmostequal(r, FormuleAtanh1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleAtanh2(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// cosh
// ============
// cosh(z) = (exp(z) + exp(-z))/2
function r = FormuleCosh(z)
    r = 0.5 * (exp(z) +  exp(-z));
endfunction

r1 = cosh(realvalues1);
r2 = cosh(realvalues2);
r3 = cosh(realvalues3);

assert_checkalmostequal(r1, FormuleCosh(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCosh(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCosh(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0  -1 -%pi/2  -%pi];
r = cosh(realpartvalues);

assert_checkalmostequal(r, FormuleCosh(realpartvalues), sqrt(%eps), 1e-16);

r1 = cosh(cplxvalues1);
r2 = cosh(cplxvalues2);
r3 = cosh(cplxvalues3);

assert_checkalmostequal(r1, FormuleCosh(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCosh(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCosh(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(realvalues1), cos(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(realvalues2), cos(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(realvalues3), cos(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = cosh(cplxpartvalues);

assert_checkalmostequal(r, FormuleCosh(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(realpartvalues), cos(cplxpartvalues), sqrt(%eps), 1e-16);

// cosh(x).^2 - sinh(x).^2 = 1
assert_checkalmostequal(cosh(realvalues1).^2- sinh(realvalues1).^2, ones(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(realvalues2).^2- sinh(realvalues2).^2, ones(realvalues2), sqrt(%eps), 1e-7);
assert_checkalmostequal(cosh(realvalues3).^2- sinh(realvalues3).^2, ones(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(realpartvalues).^2- sinh(realpartvalues).^2, ones(realpartvalues), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(cplxvalues1).^2- sinh(cplxvalues1).^2, ones(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(cplxvalues2).^2- sinh(cplxvalues2).^2, ones(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(cplxvalues3).^2- sinh(cplxvalues3).^2, ones(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(cplxpartvalues).^2- sinh(cplxpartvalues).^2, ones(cplxpartvalues), sqrt(%eps), 1e-16);

// cosh(-x) = cosh(x)
assert_checkalmostequal(cosh(-realvalues1), cosh(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(-realvalues2), cosh(realvalues2), sqrt(%eps), 1e-7);
assert_checkalmostequal(cosh(-realvalues3), cosh(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(-realpartvalues), cosh(realpartvalues), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(-cplxvalues1), cosh(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(-cplxvalues2), cosh(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(cosh(-cplxvalues3), cosh(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(cosh(-cplxpartvalues), cosh(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// coth
// ============
// coth(z) = (exp(z) + exp(-z)) / (exp(z) - exp(-z))
function r = FormuleCoth1(z)
    r = (exp(z) + exp(-z))./(exp(z) - exp(-z));
endfunction

// coth(z) = 1/tanh(z)
function r = FormuleCoth2(z)
    r = 1 ./tanh(z);
endfunction

r1 = coth(realvalues1);
r2 = coth(realvalues2);
r3 = coth(realvalues3);

assert_checkalmostequal(r1, FormuleCoth1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCoth1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCoth1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCoth2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCoth2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCoth2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = coth(realpartvalues);

assert_checkalmostequal(r, FormuleCoth1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleCoth2(realpartvalues), sqrt(%eps), 1e-16);

r1 = coth(cplxvalues1);
r2 = coth(cplxvalues2);
r3 = coth(cplxvalues3);

assert_checkalmostequal(r1, FormuleCoth1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCoth1(cplxvalues2), sqrt(%eps), 1e-12);
assert_checkalmostequal(r3, FormuleCoth1(cplxvalues3), sqrt(%eps), 1e-14);

assert_checkalmostequal(r1, FormuleCoth2(cplxvalues1), sqrt(%eps), 1e-15);
assert_checkalmostequal(r2, FormuleCoth2(cplxvalues2), sqrt(%eps), 1e-12);
assert_checkalmostequal(r3, FormuleCoth2(cplxvalues3), sqrt(%eps), 1e-14);

cplxpartvalues = realpartvalues .*%i;
r = coth(cplxpartvalues);

assert_checkalmostequal(r, FormuleCoth1(cplxpartvalues), sqrt(%eps), 1e-15);
assert_checkalmostequal(r, FormuleCoth2(cplxpartvalues), sqrt(%eps), 1e-13);

// coth(x/2) - coth(x) = csch(x)
assert_checkalmostequal(coth(realvalues1./2) - coth(realvalues1), csch(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(coth(realvalues2./2) - coth(realvalues2), csch(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(coth(realvalues3./2) - coth(realvalues3), csch(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(coth(cplxvalues1./2) - coth(cplxvalues1), csch(cplxvalues1), sqrt(%eps), 1e-15);
assert_checkalmostequal(coth(cplxvalues2./2) - coth(cplxvalues2), csch(cplxvalues2), sqrt(%eps), 1e-12);
assert_checkalmostequal(coth(cplxvalues3./2) - coth(cplxvalues3), csch(cplxvalues3), sqrt(%eps), 1e-12);

// ============
// csch
// ============
// csch(z) = 1/sinh(z)
function r = FormuleCsch1(z)
    r = 1 ./sinh(z);
endfunction

// csch(z) = 2/(exp(z) - exp(-z))
function r = FormuleCsch2(z)
    r = 2 ./(exp(z) - exp(-z));
endfunction

r1 = csch(realvalues1);
r2 = csch(realvalues2);
r3 = csch(realvalues3);

assert_checkalmostequal(r1, FormuleCsch1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsch1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsch1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCsch2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsch2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsch2(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [1  %pi/2  %pi;-1 -%pi/2  -%pi];
r = csch(realpartvalues);

assert_checkalmostequal(r, FormuleCsch1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleCsch2(realpartvalues), sqrt(%eps), 1e-16);

r1 = csch(cplxvalues1);
r2 = csch(cplxvalues2);
r3 = csch(cplxvalues3);

assert_checkalmostequal(r1, FormuleCsch1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsch1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsch1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleCsch2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleCsch2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleCsch2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = csch(cplxpartvalues);

assert_checkalmostequal(r, FormuleCsch1(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// sech
// ============
// sech(z) = 1/cosh(z)
function r = FormuleSech1(z)
    r = 1 ./cosh(z);
endfunction

// sech(z) = 2/(exp(z) + exp(-z))
function r = FormuleSech2(z)
    r = 2 ./(exp(z) + exp(-z));
endfunction

r1 = sech(realvalues1);
r2 = sech(realvalues2);
r3 = sech(realvalues3);

assert_checkalmostequal(r1, FormuleSech1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSech1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSech1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleSech2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSech2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSech2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, sech(-realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, sech(-realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, sech(-realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0 -1 -%pi/2  -%pi];
r = sech(realpartvalues);

assert_checkalmostequal(r, FormuleSech1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleSech2(realpartvalues), sqrt(%eps), 1e-16);

r1 = sech(cplxvalues1);
r2 = sech(cplxvalues2);
r3 = sech(cplxvalues3);

assert_checkalmostequal(r1, FormuleSech1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSech1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSech1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleSech2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSech2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSech2(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = sech(cplxpartvalues);

assert_checkalmostequal(r, FormuleSech1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleSech2(cplxpartvalues), sqrt(%eps), 1e-16);

assert_checkalmostequal(sech(realvalues1), sec(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(sech(realvalues2), sec(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(sech(realvalues3), sec(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(sech(realvalues1).^2, 1 - tanh(realvalues1).^2, sqrt(%eps), 1e-16);
assert_checkalmostequal(sech(realvalues2).^2, 1 - tanh(realvalues2).^2, sqrt(%eps), 1e-16);
assert_checkalmostequal(sech(realvalues3).^2, 1 - tanh(realvalues3).^2, sqrt(%eps), 1e-16);

assert_checkalmostequal(sech(cplxvalues1).^2, 1 - tanh(cplxvalues1).^2, sqrt(%eps), 1e-16);
assert_checkalmostequal(sech(cplxvalues2).^2, 1 - tanh(cplxvalues2).^2, sqrt(%eps), 1e-16);
assert_checkalmostequal(sech(cplxvalues3).^2, 1 - tanh(cplxvalues3).^2, sqrt(%eps), 1e-16);

// ============
// sinh
// ============
// sinh(z) = (exp(z) - exp(-z))/2
function r = FormuleSinh(z)
    r = 0.5 * (exp(z) - exp(-z));
endfunction

// sinh(z) = i*sin(z)
function r = FormuleSinh2(z)
    r = %i* sin(z);
endfunction

r1 = sinh(realvalues1);
r2 = sinh(realvalues2);
r3 = sinh(realvalues3);

assert_checkalmostequal(r1, FormuleSinh(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSinh(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSinh(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0 -1 -%pi/2  -%pi];
r = sinh(realpartvalues);
assert_checkalmostequal(r, FormuleSinh(realpartvalues), sqrt(%eps), 1e-16);

r1 = sinh(cplxvalues1);
r2 = sinh(cplxvalues2);
r3 = sinh(cplxvalues3);

assert_checkalmostequal(r1, FormuleSinh(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSinh(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSinh(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleSinh2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleSinh2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleSinh2(realvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = sinh(cplxpartvalues);
assert_checkalmostequal(r, FormuleSinh(cplxpartvalues), sqrt(%eps), 1e-16);

// sinh(-x) = -sinh(x)
assert_checkalmostequal(sinh(-realvalues1), -sinh(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(sinh(-realvalues2), -sinh(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(sinh(-realvalues3), -sinh(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(sinh(-realpartvalues), -sinh(realpartvalues), sqrt(%eps), 1e-16);

assert_checkalmostequal(sinh(-cplxvalues1), -sinh(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(sinh(-cplxvalues2), -sinh(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(sinh(-cplxvalues3), -sinh(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(sinh(-cplxpartvalues), -sinh(cplxpartvalues), sqrt(%eps), 1e-16);

// ============
// tanh
// ============
// tanh(z) = sinh(z)/cosh(z)
function r = FormuleTanh1(z)
    r = sinh(z) ./cosh(z);
endfunction

// tanh(z) = (exp(z) - zxp(-z))/(exp(z) + exp(-z))
function r = FormuleTanh2(z)
    r = (exp(z) - exp(-z))./(exp(z) + exp(-z));
endfunction

function r = FormuleTanh3(z)
    r = (exp(2*z) - 1)./(exp(2*z) + 1);
endfunction

// tanh(z) = -i *tan(i*z)
function r = FormuleTanh4(z)
    r = -%i * tan(%i*z);
endfunction

r1 = tanh(realvalues1);
r2 = tanh(realvalues2);
r3 = tanh(realvalues3);

assert_checkalmostequal(r1, FormuleTanh1(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleTanh1(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleTanh1(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleTanh2(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleTanh2(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleTanh2(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleTanh3(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleTanh3(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleTanh3(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleTanh4(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleTanh4(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleTanh4(realvalues3), sqrt(%eps), 1e-16);

realpartvalues = [0  1  %pi/2  %pi;-0 -1 -%pi/2  -%pi];
r = tanh(realpartvalues);

assert_checkalmostequal(r, FormuleTanh1(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleTanh2(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleTanh3(realpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleTanh4(realpartvalues), sqrt(%eps), 1e-16);

r1 = tanh(cplxvalues1);
r2 = tanh(cplxvalues2);
r3 = tanh(cplxvalues3);

assert_checkalmostequal(r1, FormuleTanh1(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleTanh1(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleTanh1(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleTanh2(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(r2, FormuleTanh2(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(r3, FormuleTanh2(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleTanh3(cplxvalues1), sqrt(%eps), 1e-14);
assert_checkalmostequal(r2, FormuleTanh3(cplxvalues2), sqrt(%eps), 1e-12);
assert_checkalmostequal(r3, FormuleTanh3(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(r1, FormuleTanh4(cplxvalues1), sqrt(%eps), 1e-14);
assert_checkalmostequal(r2, FormuleTanh4(cplxvalues2), sqrt(%eps), 1e-12);
assert_checkalmostequal(r3, FormuleTanh4(cplxvalues3), sqrt(%eps), 1e-16);

cplxpartvalues = realpartvalues .*%i;
r = tanh(cplxpartvalues);

assert_checkalmostequal(r, FormuleTanh1(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleTanh2(cplxpartvalues), sqrt(%eps), 1e-16);
assert_checkalmostequal(r, FormuleTanh4(cplxpartvalues), sqrt(%eps), 1e-16);

// tanh(-x) = -tanh(x)
assert_checkalmostequal(tanh(-realvalues1), -tanh(realvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(tanh(-realvalues2), -tanh(realvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(tanh(-realvalues3), -tanh(realvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(tanh(-realpartvalues), -tanh(realpartvalues), sqrt(%eps), 1e-16);

assert_checkalmostequal(tanh(-cplxvalues1), -tanh(cplxvalues1), sqrt(%eps), 1e-16);
assert_checkalmostequal(tanh(-cplxvalues2), -tanh(cplxvalues2), sqrt(%eps), 1e-16);
assert_checkalmostequal(tanh(-cplxvalues3), -tanh(cplxvalues3), sqrt(%eps), 1e-16);

assert_checkalmostequal(tanh(-cplxpartvalues), -tanh(cplxpartvalues), sqrt(%eps), 1e-16);
