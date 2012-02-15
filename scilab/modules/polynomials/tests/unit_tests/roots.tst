// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - 2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// 
// sort_merge_comparison --
//   Returns -1 if x < y, 
//   returns 0 if x==y,
//   returns +1 if x > y
//
function order = sort_merge_comparison ( x , y )
    if x < y then
        order = -1
    elseif x==y then
        order = 0
    else 
        order = 1
    end
endfunction

//
// sort_merge --
//   Returns the sorted array x.
// Arguments
//   x : the array to sort
//   compfun : the comparison function
// Bruno Pincon
// "quelques tests de rapidit´e entre diff´erents logiciels matriciels"
// Modified by Michael Baudin to manage a comparison function
//
function [x] = sort_merge ( varargin )
    [lhs,rhs]=argn();
    if rhs<>1 & rhs<>2 then
        errmsg = sprintf("Unexpected number of arguments : %d provided while 1 or 2 are expected.",rhs);
        error(errmsg)
    end
    // Get the array x
    x = varargin(1);
    // Get the comparison function compfun
    if rhs==1 then
        compfun = sort_merge_comparison;
    else
        compfun = varargin(2);
    end
    // Proceed...
    n = length(x)
    if n > 1 then
        m = floor(n/2); 
        p = n-m
        x1 = sort_merge ( x(1:m) , compfun )
        x2 = sort_merge ( x(m+1:n) , compfun )
        i = 1; 
        i1 = 1;
        i2 = 1;
        if ( typeof(compfun)=="list" ) then
            __compfun_fun__ = compfun(1)
        end
        for i = 1:n
            if ( typeof(compfun)=="function" ) then
                order = compfun ( x1(i1) , x2(i2) );
            else
                order = __compfun_fun__ ( x1(i1) , x2(i2), compfun(2:$) );
            end
            if order<=0 then
                x(i) = x1(i1)
                i1 = i1+1
                if (i1 > m) then
                    x(i+1:n) = x2(i2:p)
                    break
                end
            else
                x(i) = x2(i2)
                i2 = i2+1
                if (i2 > p) then
                    x(i+1:n) = x1(i1:m)
                    break
                end
            end
        end
    end
endfunction

// 
// compare_complexrealimag --
//   Returns -1 if a < b, 
//   returns 0 if a==b,
//   returns +1 if a > b
// Compare first by real parts, then by imaginary parts.
//
function order = compare_complexrealimag ( a , b )
    ar = real(a)
    br = real(b)
    if ar < br then
        order = -1
    elseif ar > br then
        order = 1
    else
        ai = imag(a)
        bi = imag(b)
        if ai < bi then
            order = -1
        elseif ai == bi then
            order = 0
        else
            order = 1
        end
    end
endfunction

function order = assert_mycompcompl ( varargin )
    // Copyright (C) 2008-2009 - INRIA - Michael Baudin
    // Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
    // Compare complex numbers with a tolerance.
    // order = assert_mycompcompl(a,b)
    // order = assert_mycompcompl(a,b,rtol)
    // order = assert_mycompcompl(a,b,rtol,atol)

    [lhs,rhs]=argn()
    if ( and(rhs <> [2 3 4] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_mycompcompl" , 2 , 4 )
        error(errmsg)
    end
    //
    // Get arguments
    a = varargin(1)
    b = varargin(2)
    reltol = argindefault ( rhs , varargin , 3 , sqrt(%eps) )
    abstol = argindefault ( rhs , varargin , 4 , 0 )
    //
    // Check types of variables
    if ( typeof(a) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_mycompcompl" , 1 )
        error(errmsg)
    end
    if ( typeof(a) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_mycompcompl" , 2 )
        error(errmsg)
    end
    if ( typeof(reltol) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_mycompcompl" , 3 )
        error(errmsg)
    end  
    if ( typeof(abstol) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_mycompcompl" , 4 )
        error(errmsg)
    end  
    //
    // Check sizes of variables
    if ( size(a,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_mycompcompl" , 1 , 1 , 1 )
        error(errmsg)
    end
    if ( size(b,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_mycompcompl" , 2 , 1 , 1 )
        error(errmsg)
    end
    if ( size(reltol,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_mycompcompl" , 3 , 1 , 1 )
        error(errmsg)
    end
    if ( size(abstol,"*") <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_mycompcompl" , 4 , 1 , 1 )
        error(errmsg)
    end
    //
    // Check values of variables
    if ( reltol < 0 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Must be > %d.\n") , "assert_mycompcompl" , 3 , 0 )
        error(errmsg)
    end
    if ( abstol < 0 ) then
        errmsg = sprintf ( gettext ( "%s: Wrong value for input argument #%d: Must be > %d.\n") , "assert_mycompcompl" , 4 , 0 )
        error(errmsg)
    end
    //
    ord_re = assert_compdata ( real(a) , real(b) , reltol , abstol )
    if ( ord_re == 0 ) then
        // Tie on the real part: compare imaginary parts
        ord_im = assert_compdata ( imag(a) , imag(b) , reltol , abstol )
        if ( ord_im == 0 ) then
            // Tie on imaginary parts too: two numbers are "equal"
            order = 0
        elseif ( ord_im == -1 ) then
            order = -1
        else
            order = 1
        end
    elseif ( ord_re == -1 ) then
        order = -1
    else
        order = 1
    end
endfunction

function order = assert_compdata ( a , b , reltol , abstol )
    if ( a == %inf ) then
        if ( isnan(b) ) then
            order = -1
        elseif ( b == %inf ) then
            order = 0
        else
            order = 1
        end
    elseif ( a == -%inf ) then
        if ( b == -%inf ) then
            order = 0
        else
            order = -1
        end
    elseif ( isnan(a) ) then
        if ( isnan(b) ) then
            order = 0
        else
            order = 1
        end
    else
        if ( isnan(b) ) then
            order = -1
        elseif ( b == -%inf ) then
            order = 1
        elseif ( b == %inf ) then
            order = -1
        else
            areequal = abs ( a - b ) <= reltol * max ( abs(a) , abs(b) ) + abstol
            if ( areequal ) then
                order = 0
            elseif ( a < b ) then
                order = -1
            else
                order = 1
            end
        end
    end
endfunction


function argin = argindefault ( rhs , vararglist , ivar , default )
    // Returns the value of the input argument #ivar.
    // If this argument was not provided, or was equal to the 
    // empty matrix, returns the default value.
    if ( rhs < ivar ) then
        argin = default
    else
        if ( vararglist(ivar) <> [] ) then
            argin = vararglist(ivar)
        else
            argin = default
        end
    end
endfunction

function y = sortmyroots(varargin)
    // Sort the roots of a polynomial with a customized
    // complex-aware sorting algorithm.
    // y = sortmyroots(x)
    // y = sortmyroots(x,rtol)
    // y = sortmyroots(x,rtol,atol)
    [lhs,rhs]=argn()
    if ( and(rhs <> [1 2 3] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "sortmyroots" , 1 , 3 )
        error(errmsg)
    end
    x = varargin(1)
    reltol = argindefault ( rhs , varargin , 2 , sqrt(%eps) )
    abstol = argindefault ( rhs , varargin , 3 , 0 )
    y = sort_merge ( x , list(assert_mycompcompl,reltol,abstol) );
endfunction

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
    if expected==0.0 then
        shift = norm(computed-expected);
    else
        den = norm(expected)
        if ( den==0 ) then
            shift = norm(computed-expected);
        else
            shift = norm(computed-expected)/den;
        end
    end
    if shift <= epsilon then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction

//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
    if computed==expected then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction

function checkroots(p,expectedroots,rtol)
    // Checks the roots function against given roots.
    //
    // 1. Check default algorithm
    myroots=roots(p);
    computedroots = sortmyroots(myroots);
    expectedroots  = sortmyroots(expectedroots);
    assert_close(computedroots,expectedroots,rtol);
    //
    // 2. Check "e" algorithm
    myroots=roots(p,"e");
    computedroots = sortmyroots(myroots);
    expectedroots  = sortmyroots(expectedroots);
    assert_close(computedroots,expectedroots,rtol);
    //
    // 3. Check "f" algorithm
    if ( isreal(p) ) then
        myroots=roots(p,"f");
        computedroots = sortmyroots(myroots);
        expectedroots  = sortmyroots(expectedroots);
        assert_close(computedroots,expectedroots,rtol);
    end
endfunction

//   Check the computation of the roots of a polynomial
//   with different kinds of polynomials and different 
//   kinds of roots :
//   - real poly,
//   - complex poly,
//   - real roots,
//   - complex roots.

//roots : 3 real roots
p=-6+11*%s-6*%s^2+%s^3;
expectedroots  = [1; 2; 3];
checkroots(p,expectedroots,400*%eps);
//roots : 3 real roots + polynomials algebra
p=-6+11*%s-6*%s^2+%s^3;
q = p+0;
expectedroots  = [1; 2; 3];
checkroots(q,expectedroots,400*%eps);
//roots : 3 complex roots
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
expectedroots  = [1+%i; 2 ; 3];
checkroots(p,expectedroots,400*%eps);
//roots : 3 complex roots + polynomials algebra
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
q = p+0;
expectedroots  = [1+%i; 2 ; 3];
checkroots(p,expectedroots,400*%eps);
// roots : no root at all
p=1;
v=[];
checkroots(p,[],0);
q = p+0;
checkroots(q,[],0);
//roots : 2 complex roots
p=1+%s+%s^2;
expectedroots  = [-0.5 - sqrt(3.)/2.*%i; -0.5 + sqrt(3.)/2.*%i ];
checkroots(p,expectedroots,400*%eps);
//roots : 2 roots equals 0
p=%s^2;
expectedroots  = [0. ; 0. ];
// 2 real roots with a zero derivate at the root
p=(%s-%pi)^2;
expectedroots  = [%pi;%pi];
checkroots(p,expectedroots,400*%eps);
//
// Caution !
// The following are difficult root-finding problems
// with expected precision problems.
// See "Principles for testing polynomial 
// zerofinding programs"
// Jenkins, Traub
// 1975
// p.28
// "The accuracy which one may expect to achieve in calculating
// zeros is limited by the condition of these zeros. In particular,
// for multiple zeros perturbations of size epsilon in the 
// coefficients cause perturbations of size epsilon^(1/m)
// in the zeros."
// 
//
// 3 real roots with a zero derivate at the root
// Really difficult problem : only simple precision computed, instead of double precision ***
p=(%s-%pi)^3;
expectedroots  = [%pi;%pi;%pi];
checkroots(p,expectedroots,10^11*%eps);
// 4 real roots with a zero derivate at the root
// Really difficult problem : only simple precision
p=(%s-%pi)^4;
expectedroots  = [%pi;%pi;%pi;%pi];
checkroots(p,expectedroots,10^12*%eps);
// 10 real roots with a zero derivate at the root
// Really difficult problem : only one correct digit
p=(%s-%pi)^10;
expectedroots  = [%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi];
checkroots(p,expectedroots,10^15*%eps);

// "Numerical computing with Matlab", Cleve Moler.
A = diag(1:20);
p = poly(A,'x');
e = [1:20]';
checkroots(p,e,1.e-2);

// Tests from CPOLY
// M. A. Jenkins and J. F. Traub. 1972. 
// Algorithm 419: zeros of a complex polynomial. 
// Commun. ACM 15, 2 (February 1972), 97-99. 
// 
// EXAMPLE 1.  POLYNOMIAL WITH ZEROS 1,2,...,10.
P=[];
PI=[];
P(1)=1;
P(2)=-55;
P(3)=1320;
P(4)=-18150;
P(5)=157773;
P(6)=-902055;
P(7) = 3416930;
P(8)=-8409500;
P(9)=12753576;
P(10)=-10628640;
P(11)=3628800;
PI(1:11) = 0;
P = complex(P,PI);
E = (1:10)';
R = roots(P);
E = sortmyroots(E);
R = sortmyroots(R);
assert_close ( R, E, 1.e-10 );

// EXAMPLE 2. ZEROS ON IMAGINARY AXIS DEGREE 3.
// x^3-10001.0001*i*x^2-10001.0001*x+i
P = [];
PI=[];
P(1)=1;
P(2)=0;
P(3)=-10001.0001;
P(4)=0;
PI(1)=0;
PI(2)=-10001.0001;
PI(3)=0;
PI(4)=1;
P = complex(P,PI);
E = [
0.0001*%i
%i
10000*%i
];
R = roots(P);
E = sortmyroots(E,[],1.e-10);
R = sortmyroots(R,[],1.e-10);
assert_close ( R, E, 1.e-14 );

// plot(real(R),imag(R),"bo")
// xtitle("Roots","Real","Imaginary")

// EXAMPLE 3. ZEROS AT 1+I,1/2*(1+I)....1/(2**-9)*(1+I)
P = [];
PI=[];
P(1)=1.0;
P(2)=-1.998046875;
P(3)=0.0;
P(4)=.7567065954208374D0;
P(5)=-.2002119533717632D0;
P(6)=1.271507365163416D-2;
P(7)=0;
P(8)=-1.154642632172909D-5;
P(9)=1.584803612786345D-7;
P(10)=-4.652065399568528D-10;
P(11)=0;
PI(1)=0;
PI(2)=P(2);
PI(3)=2.658859252929688D0;
PI(4)=-7.567065954208374D-1;
PI(5)=0;
PI(6)=P(6);
PI(7)=-7.820779428584501D-4;
PI(8)=-P(8);
PI(9)=0;
PI(10)=P(10);
PI(11)=9.094947017729282D-13;
P = complex(P,PI);
R = roots(P);
E = (1+%i)*2.^((0:-1:-9)');
E = sortmyroots(E);
R = sortmyroots(R);
assert_close ( R, E, 1.e-14 );

// EXAMPLE 4. MULTIPLE ZEROS
// Real part:
// 288 - 1344*x + 2204*x^2 - 920*x^3 - 1587*x^4 + 2374*x^5 - 1293*x^6 + 284*x^7 + 3*x^8 - 10*x^9 + x^10
// Imaginary part:
// 504*x - 2352*x^2 + 4334*x^3 - 3836*x^4 + 1394*x^5 + 200*x^6 - 334*x^7 + 100*x^8 - 10*x^9
P = [];
PI=[];
P(1)=1;
P(2)=-10;
P(3)=3;
P(4)=284;
P(5)=-1293;
P(6)=2374;
P(7)=-1587;
P(8)=-920;
P(9)=2204;
P(10)=-1344;
P(11)=288;
PI(1)=0;
PI(2)=-10;
PI(3)=100;
PI(4)=-334;
PI(5)=200;
PI(6)=1394;
PI(7) =-3836;
PI(8)=4334;
PI(9)=-2352;
PI(10)=504;
PI(11)=0;
P = complex(P,PI);
R = roots(P);
E = [
1
1
1
1
2*%i
2*%i
2*%i
3
3
4*%i
];
E = sortmyroots(E,[],1.e-3);
R = sortmyroots(R,[],1.e-3);
assert_close ( R, E, 1.e-4 );

// EXAMPLE 5. 12 ZEROS EVENLY DISTRIBUTE ON A CIRCLE OF 
// RADIUS 1. CENTERED AT 0+2I
// Real part:
// 4095 - 67584*x^2 + 126720*x^4 - 59136*x^6 + 7920*x^8 - 264*x^10 + x^12
// Imaginary part:
// 24576*x - 112640x^3 + 101376x^5 - 25344x^7 + 1760x^9 - 24x^11

P = [];
PI=[];
P(1)=1;
P(2)=0;
P(3)=-264;
P(4)=0;
P(5)=7920;
P(6)=0;
P(7)=-59136;
P(8)=0;
P(9)=126720;
P(10)=0;
P(11)=-67584;
P(12)=0;
P(13)=4095;
PI(1)=0;
PI(2)=-24;
PI(3)=0;
PI(4)=1760;
PI(5)=0;
PI(6)=-25344;
PI(7)=0;
PI(8)=101376;
PI(9)=0;
PI(10)=-112640;
PI(11)=0;
PI(12)=24576;
PI(13)=0;
P = complex(P,PI);
R = roots(P);
S3=sqrt(3);
E = [
-1 + 2*%i
%i
3*%i
1+2*%i
(1/2)*(-S3+3*%i)
(1/2)*(-S3+5*%i)
-(1/2)*%i*(S3+(-4-%i))
(1/2)*((1+4*%i)-%i*S3)
(1/2)*%i*(S3+(4+%i))
(1/2)*((1+4*%i)+%i*S3)
(1/2)*(S3+3*%i)
(1/2)*(S3+5*%i)
];
E = sortmyroots(E);
R = sortmyroots(R, sqrt(%eps), 1.e-11);
assert_close ( R, E, 1.e-8 );
