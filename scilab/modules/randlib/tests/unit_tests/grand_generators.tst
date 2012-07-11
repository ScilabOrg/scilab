// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sabine Gaüzere
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

function checkMeanVariance2arg ( m , n , name , A , B , mu , va , rtol )
  // Check the mean and variance of random numbers.
  //
  // Parameters
  // m : a 1-by-1 matrix of floating point integers, the number of rows
  // n : a 1-by-1 matrix of floating point integers, the number of columns
  // name: a 1-by-1 string, the name of the distribution function
  // A : a 1-by-1 matrix of doubles, the value of the 1st parameter
  // B : a 1-by-1 matrix of doubles, the value of the 2nd parameter
  // mu : a 1-by-1 matrix of doubles, the expected mean
  // va : a 1-by-1 matrix of doubles, the expected variance. If va==[], then the variance is not checked.
  // rtol : a 1-by-1 matrix of doubles, the relative tolerance
  
  R=grand(m,n,name,A,B);
  assert_checkequal ( size(R) , [m,n] );
  assert_checkequal ( typeof(R) , "constant" );
  assert_checkalmostequal ( mean(R) , mu , rtol );
  if ( va<>[] ) then
    assert_checkalmostequal ( variance(R) , va , rtol );
  end
endfunction

function checkMeanVariance0arg ( m , n , name , mu , va , rtol )
  // Check the mean and variance of random numbers.
  //
  // Parameters
  // m : a 1-by-1 matrix of floating point integers, the number of rows
  // n : a 1-by-1 matrix of floating point integers, the number of columns
  // name: a 1-by-1 string, the name of the distribution function
  // mu : a 1-by-1 matrix of doubles, the expected mean
  // va : a 1-by-1 matrix of doubles, the expected variance. If va==[], then the variance is not checked.
  // rtol : a 1-by-1 matrix of doubles, the relative tolerance
  
  R=grand(m,n,name);
  assert_checkequal ( size(R) , [m,n] );
  assert_checkequal ( typeof(R) , "constant" );
  assert_checkalmostequal ( mean(R) , mu , rtol );
  if ( va<>[] ) then
    assert_checkalmostequal ( variance(R) , va , rtol );
  end
endfunction

function checkLaw0arg ( name , cdffun , N , NC , rtol )
  //
  // Check the random number generator for a continuous distribution function.
  //
  // Parameters
  // name: a 1-by-1 string, the name of the distribution function
  // cdffun : a function, the Cumulated Distribution Function
  // NC : a 1-by-1 matrix of floating point integers, the number of classes
  // N : a 1-by-1 matrix of floating point integers, the number of random values to test
  // rtol : a 1-by-1 matrix of doubles, the relative tolerance
  //
  // Description
  //  Compare the empirical histogram with the theoretical histogram.

  R = grand(1,N,name);
  [X,EmpiricalPDF] = histcompute(NC,R);
  CDF = cdffun(X)
  TheoricPDF = diff(CDF);
  assert_checktrue ( abs(EmpiricalPDF-TheoricPDF) < rtol );
    if ( %f ) then
      plot(X(1:$-1),EmpiricalPDF,"bo-"); // Empirical Histogram
      plot(X(1:$-1),TheoricPDF,"rox-"); // Theoretical Histogram
    end
endfunction

function checkPieceLaw0arg ( name , cdffun , N , NC , rtol )
  //
  // Check the random number generator for a piecewise distribution function.
  //
  // Parameters
  // name: a 1-by-1 string, the name of the distribution function
  // cdffun : a function, the Cumulated Distribution Function
  // NC : a 1-by-1 matrix of floating point integers, the number of classes
  // N : a 1-by-1 matrix of floating point integers, the number of random values to test
  // rtol : a 1-by-1 matrix of doubles, the relative tolerance
  //
  // Description
  //  Compare the empirical histogram with the theoretical histogram.
  //  The classes of the histogram are computed from the random samples, 
  //  from which the unique entries are extracted.

  R=grand(N,1,name);
  X = unique(R);
  for k = 1 : size(X,"*")
    EmpiricalPDF(k) = length(find(R==X(k)));
  end
  EmpiricalPDF = EmpiricalPDF./N;
  CDF = cdffun(X)
  TheoricPDF=[CDF(1);diff(CDF)];
  assert_checktrue( abs(EmpiricalPDF-TheoricPDF) < rtol );
  if ( %f ) then
    plot(X,EmpiricalPDF,"bo-"); // Empirical Histogram
    plot(X,TheoricPDF,"rox-"); // Theoretical Histogram
  end
endfunction

function checkPieceLaw2arg ( name , cdffun , N , NC , A , B , rtol )
  //
  // Check the random number generator for a piecewise distribution function.
  //
  // Parameters
  // name: a 1-by-1 string, the name of the distribution function
  // cdffun : a function, the Cumulated Distribution Function
  // NC : a 1-by-1 matrix of floating point integers, the number of classes
  // N : a 1-by-1 matrix of floating point integers, the number of random values to test
  // A : a 1-by-1 matrix of doubles, the value of the parameter
  // rtol : a 1-by-1 matrix of doubles, the relative tolerance
  //
  // Description
  //  Compare the empirical histogram with the theoretical histogram.
  //  The classes of the histogram are computed from the random samples, 
  //  from which the unique entries are extracted.

  R=grand(N,1,name,A,B);
  X = unique(R);
  for k = 1 : size(X,"*")
    EmpiricalPDF(k) = length(find(R==X(k)));
  end
  EmpiricalPDF = EmpiricalPDF./N;
  CDF = cdffun(X,A,B)
  TheoricPDF=[CDF(1);diff(CDF)];
  assert_checktrue( abs(EmpiricalPDF-TheoricPDF) < rtol );
  if ( %f ) then
    plot(X,EmpiricalPDF,"bo-"); // Empirical Histogram
    plot(X,TheoricPDF,"rox-"); // Theoretical Histogram
  end
endfunction

function [x,y] = histcompute(n,data)
   //
   // Computes the histogram of a data.
   //
   // Parameters
   // n : a 1-by-1 matrix of floating point integers, the number of classes.
   // data : a matrix of doubles, the data
   // x : 1-by-n+1 matrix of doubles, the classes of the histogram
   // y : 1-by-n+1 matrix of doubles, the empirical probability that one value in data which fall in each class
   x = linspace(min(data), max(data), n+1);
   [ind , y] = dsearch(data, x)
   y = y./length(data)
endfunction

function p = mycdfdef (X)
  p = X
endfunction

function p = mycdfuin (X,A,B)
  p = (floor(X)-A+1)/(B-A+1)
endfunction

function p = mycdflgi (X)
  // Here, A and B are read from the environment
  p = (floor(X)-A+1)/(B-A+1)
endfunction


//
// These tests makes checks that all uniform generators are correct.
// We run the following actions : 'setgen', 'getgen', 'setsd','getsd'
// We also check the first ten uniform numbers from each generator with a known seed.
// seedsize : size of the state for each generator
// MinInt : minimum of the uniform integer interval for random number generation
// MaxInt : maximum of the uniform integer interval for random number generation
//
NGen = 6;
generators = ["mt"   "kiss" "clcg2"    "clcg4" "fsultra" "urand"];
seedsize =   [625    4      2          4       40        1];
MinInt =     [0      0      0          0       0         0];
MaxInt =     [2^32-1 2^32-1 2147483561 2^31-2  2^32-1    2^31-1];

rtol = 1.e-2;

// The number of classes in the histogram
// NC must be even.
NC = 2*12;
N=10000;

//
// The default generator must be Mersenne-Twister
S=grand('getgen');
assert_checkequal ( S , "mt" );

// The maximum integer generable with uin option
UinMax = 2147483560;

////////////////////////////////////////////////////////////////////
//
// "mt"
//
kgen = 1;
gen = "mt";
sdsize = seedsize(kgen);
grand('setgen',gen);
S=grand('getgen');
assert_checkequal ( S , gen );
//
grand('setsd',0);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
grand('setsd',123456);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
// Check numbers
expected = [
0.5488135    0.6027634    0.4236548    0.4375872    0.9636628    0.7917250  
0.5928446    0.8579456    0.6235637    0.2975346    0.2726563    0.8121687  
0.7151894    0.5448832    0.6458941    0.891773     0.3834415    0.5288949  
0.8442657    0.8472517    0.3843817    0.0567130    0.4776651    0.4799772  
];
grand('setsd',0);
computed = grand(4,6,"def");
assert_checkalmostequal ( computed , expected, 1.e-6 );
//
// Check integers
expected = [
    2357136044.    2588848963.    1819583497.    1879422756.    4138900056.    3400433126.  
    2546248239.    3684848379.    2678185683.    1277901399.    1171049868.    3488238119.  
    3071714933.    2340255427.    2774094101.    3830135878.    1646868794.    2271586391.  
    3626093760.    3638918503.    1650906866.    243580376.     2051556033.    2061486254.  
];
grand('setsd',0);
computed = grand(4,6,"lgi");
assert_checkequal ( computed , expected );
//
// Check distribution of uniform numbers in [0,1[
checkMeanVariance0arg ( 400 , 800 , "def" , 1/2 , 1/12 , rtol );
checkLaw0arg ( "def" , mycdfdef , N , NC , rtol );
//
// Check distribution of uniform integers in [A,B]
A = MinInt(kgen);
B = MaxInt(kgen);
checkMeanVariance0arg ( 400 , 800 , "lgi" , (A+B)/2 , ((B-A+1)^2-1)/12 , rtol );
checkPieceLaw2arg ( "uin" , mycdfuin , N , NC , 0 , UinMax , rtol );
checkPieceLaw0arg ( "lgi" , mycdflgi , N , NC , rtol );

////////////////////////////////////////////////////////////////////
//
// "kiss"
//
kgen = 2;
gen = "kiss";
sdsize = seedsize(kgen);
grand('setgen',gen);
S=grand('getgen');
assert_checkequal ( S , gen );
//
grand('setsd',0,0,0,0);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
grand('setsd',123456,123456,123456,123456);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
// Check numbers
expected = [
    2.874450D-04    9.423555D-01    7.707249D-02    2.078324D-02    4.746445D-01    1.895302D-01  
    8.538282D-01    5.493145D-01    3.200836D-01    4.775516D-01    2.245108D-01    6.637360D-01  
    5.815227D-02    6.006782D-01    8.569004D-01    1.235649D-02    7.357421D-01    5.837571D-01  
    5.196679D-01    2.448867D-01    2.568304D-01    4.503826D-01    9.680347D-01    5.214808D-01  
];
grand('setsd',0,0,0,0);
computed = grand(4,6,"def");
assert_checkalmostequal ( computed , expected, 1.e-6 );
//
// Check integers
expected = [
    1234567.       4047385867.    331023823.     89263315.      2038582807.    814026139.   
    3667164066.    2359287638.    1374748746.    2051068542.    964266482.     2850724518.  
    249762113.     2579893349.    3680359369.    53070701.      3159988049.    2507217781.  
    2231956628.    1051780200.    1103078268.    1934378448.    4157677412.    2239743032.  
];
grand('setsd',0,0,0,0);
computed = grand(4,6,"lgi");
assert_checkequal ( computed , expected );
//
// Check distribution of uniform numbers in [0,1[
checkMeanVariance0arg ( 400 , 800 , "def" , 1/2 , 1/12 , rtol );
checkLaw0arg ( "def" , mycdfdef , N , NC , rtol );
//
// Check distribution of uniform integers in [A,B]
A = MinInt(kgen);
B = MaxInt(kgen);
checkMeanVariance0arg ( 400 , 800 , "lgi" , (A+B)/2 , ((B-A+1)^2-1)/12 , rtol );
checkPieceLaw2arg ( "uin" , mycdfuin , N , NC , 0 , UinMax , rtol );
checkPieceLaw0arg ( "lgi" , mycdflgi , N , NC , rtol );
////////////////////////////////////////////////////////////////////
//
// "clcg2"
//
kgen = 3;
gen = "clcg2";
sdsize = seedsize(kgen);
grand('setgen',gen);
S=grand('getgen');
assert_checkequal ( S , gen );
//
grand('setsd',1,1);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
grand('setsd',123456,123456);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
// Check numbers
expected = [
0.9999997    0.0369445    0.2041364    0.9100817    0.6998243    0.9596867  
0.9745196    0.1617119    0.1673069    0.1117162    0.9502824    0.9149753  
0.6474839    0.6646450    0.6549574    0.2990212    0.0918107    0.4411791  
0.3330856    0.0846729    0.1288161    0.2654475    0.9023415    0.0735483  
];
grand('setsd',1,1);
computed = grand(4,6,"def");
assert_checkalmostequal ( computed , expected, 1.e-5 );
//
// Check integers
expected = [
    2147482884.    79337801.      438379562.     1954385533.    1502861091.    2060911403.  
    2092764894.    347273588.     359288887.     239908781.     2040715732.    1964894377.  
    1390461064.    1427314282.    1406510214.    642143055.     197161966.     947424871.   
    715295839.     181833602.     276630551.     570044126.     1937763493.    157943826.   
];
grand('setsd',1,1);
computed = grand(4,6,"lgi");
assert_checkequal ( computed , expected );
//
// Check distribution of uniform numbers in [0,1[
checkMeanVariance0arg ( 400 , 800 , "def" , 1/2 , 1/12 , rtol );
checkLaw0arg ( "def" , mycdfdef , N , NC , rtol );
//
// Check distribution of uniform integers in [A,B]
A = MinInt(kgen);
B = MaxInt(kgen);
checkMeanVariance0arg ( 400 , 800 , "lgi" , (A+B)/2 , ((B-A+1)^2-1)/12 , rtol );
checkPieceLaw2arg ( "uin" , mycdfuin , N , NC , 0 , UinMax , rtol );
checkPieceLaw0arg ( "lgi" , mycdflgi , N , NC , rtol );
////////////////////////////////////////////////////////////////////
//
// "clcg4"
//
kgen = 4;
gen = "clcg4";
sdsize = seedsize(kgen);
grand('setgen',gen);
S=grand('getgen');
assert_checkequal ( S , gen );
//
warning("off");
grand('setsd',1,1,1,1);
warning("on");
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
warning("off");
grand('setsd',123456,123456,123456,123456);
warning("on");
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
// Check numbers
expected = [
0.9999661    0.0552914    0.6345306    0.0640227    0.2885048    0.2781458  
0.6852419    0.1806991    0.8665501    0.0981421    0.2660715    0.4279616  
0.4370514    0.4956021    0.6870544    0.8501209    0.1271038    0.4554926  
0.4202952    0.2903676    0.5712601    0.4764120    0.1818799    0.3121748  
];
warning("off");
grand('setsd',1,1,1,1);
warning("on");
computed = grand(4,6,"def");
assert_checkalmostequal ( computed , expected, 1.e-6 );
//
// Check integers
expected = [
    2147410798.    118737467.     1362644105.    137487708.     619559402.    597313629.  
    1471545799.    388048305.     1860902184.    210758531.     571384155.    919040470.  
    938560647.     1064297484.    1475437993.    1825620628.    272953383.    978162913.  
    902576998.     623559632.     1226771622.    1023086907.    390584072.    670390361.  
];
warning("off");
grand('setsd',1,1,1,1);
warning("on");
computed = grand(4,6,"lgi");
assert_checkequal ( computed , expected );
//
// Check distribution of uniform numbers in [0,1[
checkMeanVariance0arg ( 400 , 800 , "def" , 1/2 , 1/12 , rtol );
checkLaw0arg ( "def" , mycdfdef , N , NC , rtol );
//
// Check distribution of uniform integers in [A,B]
A = MinInt(kgen);
B = MaxInt(kgen);
checkMeanVariance0arg ( 400 , 800 , "lgi" , (A+B)/2 , ((B-A+1)^2-1)/12 , rtol );
checkPieceLaw2arg ( "uin" , mycdfuin , N , NC , 0 , UinMax , rtol );
checkPieceLaw0arg ( "lgi" , mycdflgi , N , NC , rtol );
////////////////////////////////////////////////////////////////////
//
// "fsultra"
//
kgen = 5;
gen = "fsultra";
sdsize = seedsize(kgen);
grand('setgen',gen);
S=grand('getgen');
assert_checkequal ( S , gen );
//
grand('setsd',1,1);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
grand('setsd',123456,123456);
S=grand('getsd');
assert_checkequal ( typeof(S) , "constant" );
assert_checkequal ( size(S) , [sdsize 1] );
//
// Check numbers
expected = [
0.3314877    0.3699260    0.4383216    0.99706      0.0577929    0.4836669  
0.5826624    0.9600475    0.2037475    0.6774254    0.4450278    0.3082941  
0.1630857    0.2033307    0.4214824    0.6372521    0.0782678    0.4409892  
0.7211611    0.1833922    0.8065496    0.6375251    0.2572713    0.8039582  
];
grand('setsd',1,1);
computed = grand(4,6,"def");
assert_checkalmostequal ( computed , expected, 1.e-6 );
//
// Check integers
expected = [
    1423728774.    1588820113.    1882577034.    4282340079.    248218608.     2077333598.  
    2502516061.    4123372468.    875088704.     2909519830.    1911379739.    1324113135.  
    700447838.     873298853.     1810253313.    2736976953.    336157762.     1894034123.  
    3097363227.    787663378.     3464104206.    2738149622.    1104971606.    3452974260.  
];
grand('setsd',1,1);
computed = grand(4,6,"lgi");
assert_checkequal ( computed , expected );
//
// Check distribution of uniform numbers in [0,1[
checkMeanVariance0arg ( 400 , 800 , "def" , 1/2 , 1/12 , rtol );
checkLaw0arg ( "def" , mycdfdef , N , NC , rtol );
//
// Check distribution of uniform integers in [A,B]
A = MinInt(kgen);
B = MaxInt(kgen);
checkMeanVariance0arg ( 400 , 800 , "lgi" , (A+B)/2 , ((B-A+1)^2-1)/12 , rtol );
checkPieceLaw2arg ( "uin" , mycdfuin , N , NC , 0 , UinMax , rtol );
checkPieceLaw0arg ( "lgi" , mycdflgi , N , NC , rtol );
////////////////////////////////////////////////////////////////////
//
// "urand"
//
kgen = 6;
gen = "urand";
grand('setgen',gen);
S=grand('getgen');
assert_checkequal ( S , gen );
//
grand('setsd',1);
S=grand('getsd');
assert_checkequal ( S , 1 );
//
grand('setsd',123456);
S=grand('getsd');
assert_checkequal ( S , 123456 );
//
// Check numbers
expected = [
0.6040239    0.5321420    0.2276811    0.8979351    0.8925854    0.6928366  
0.0079647    0.4138784    0.6656067    0.8274020    0.0848163    0.6776849  
0.6643966    0.5036204    0.9694369    0.0664231    0.2566682    0.4284010  
0.9832111    0.6850569    0.0775390    0.1099766    0.6507795    0.3725794  
];
grand('setsd',1);
computed = grand(4,6,"def");
assert_checkalmostequal ( computed , expected, 1.e-5 );
//
// Check integers
expected = [
    1297131554.    1142766270.    488941338.     1928300854.    1916812562.    1487855278.  
    17103983.      888797147.     1429379591.    1776832243.    182141599.     1455317259.  
    1426780792.    1081516660.    2081849904.    142642604.     551190760.     919984100.   
    2111429773.    1471148505.    166513637.     236172977.     1397538365.    800108169.   
];
grand('setsd',1);
computed = grand(4,6,"lgi");
assert_checkequal ( computed , expected );
//
// Check distribution of uniform numbers in [0,1[
checkMeanVariance0arg ( 400 , 800 , "def" , 1/2 , 1/12 , rtol );
checkLaw0arg ( "def" , mycdfdef , N , NC , rtol );
//
// Check distribution of uniform integers in [A,B]
A = MinInt(kgen);
B = MaxInt(kgen);
checkMeanVariance0arg ( 400 , 800 , "lgi" , (A+B)/2 , ((B-A+1)^2-1)/12 , rtol );
checkPieceLaw2arg ( "uin" , mycdfuin , N , NC , 0 , UinMax , rtol );
checkPieceLaw0arg ( "lgi" , mycdflgi , N , NC , rtol );

